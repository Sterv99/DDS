#include <DDS/rtmp/rtmp_session.hpp>
#include <DDS/core/logger.hpp>

struct C0S0
{
	uint8_t version;
};

struct C1S1
{
	uint32_t time;
	uint32_t zero;
	std::array<uint8_t, 1528> random;
};

struct C2S2
{
	uint32_t time;
	uint32_t time2;
	std::array<uint8_t, 1528> random;
};

void rtmp_session::on_connect()
{
	LOG(INFO) << "<rtmp> " << "new client trying to connect";
	C0S0 c0{}, s0{};

	size_t size = sizeof(C0S0);
	read(&c0, size);
	s0 = c0;

	size = sizeof(C0S0);
	write(&s0, size);

	C1S1 c1{}, s1{};

	size = sizeof(C1S1);
	write(&s1, size);

	size = sizeof(C1S1);
	read(&c1, size);

	C2S2 c2{}, s2{};
	s2.time = c1.time;
	s2.random = c1.random;

	size = sizeof(C2S2);
	write(&s2, size);

	size = sizeof(C2S2);
	read(&c2, size);

	state = CONNECT;
	LOG(INFO) << "<rtmp> " << "handshake successful - client connected";

	async_read();
}

void rtmp_session::on_write(size_t len)
{
	
}

void rtmp_session::on_error(boost::system::error_code ec)
{
	if(ec == boost::system::errc::broken_pipe)
	{
		//LOG(WARN) << ec.message();
		close();
	}
	else
	{
		LOG(ERROR) << "<rtmp> " << ec.message();
		close();
	}
}

void rtmp_session::write_chunk(uint16_t csid, uint32_t time, uint32_t dtime, uint8_t type_id, uint32_t stream_id, const uint8_t* msg, int msg_size, bool force_f0)
{
	if (cs_out.count(csid) == 0)
		cs_out[csid] = std::make_shared<rtmp_chunk_stream>(this, csid);

	cs_out[csid]->write_chunk(time, dtime, type_id, stream_id, msg, msg_size, force_f0);
}

void rtmp_session::write_chunk(uint16_t csid, uint32_t time, uint32_t dtime, uint8_t type_id, uint32_t stream_id, std::shared_ptr<std::vector<uint8_t>> fb, bool force_f0)
{
	if (cs_out.count(csid) == 0)
		cs_out[csid] = std::make_shared<rtmp_chunk_stream>(this, csid);

	cs_out[csid]->write_chunk(time, dtime, type_id, stream_id, fb, force_f0);
}

void rtmp_session::write_media_packet(std::shared_ptr<media_packet> pkt)
{
	uint16_t csid = 0;
	if (pkt->type_ == media_packet::Type::Video || pkt->type_ == media_packet::Type::Metadata)
		csid = 6;
	else if (pkt->type_ == media_packet::Type::Audio)
		csid = 4;

	if(csid)
		write_chunk(csid, pkt->dts_, 0, pkt->rtmp.type_, pkt->rtmp.stream_id_, pkt->data_p, true);
}

void rtmp_session::on_read(char* data, size_t len)
{
	recv_buf.insert(recv_buf.cend(), data, data + len);

	int ret = handle();
	if (ret < 0)
		close();
	else if (ret == 1)
		async_read();
}

int rtmp_session::handle()
{
	int ret = -1;
	if (state == INIT)
	{
		/*ret = hs.handle_c0c1();
		if (ret < 0 || ret == 1)
			return ret;
		recv_buf->clear();
		ret = hs.send_s0s1s2();
		state = HANDSHAKE_C2;*/
	}
	else if (state == HANDSHAKE_C2)
	{
		/*ret = hs.handle_c2();
		if (ret < 0 || ret == 1)
			return ret;*/
	}
	else if (state == CLOSE)
	{

	}
	else
	{
		while (recv_buf.size() > 0)
		{
			ret = read_chunk_stream();
			if (ret < 0 || ret == 1)
				return ret;

			if (!cs_in[csid_]->ready())
			{
				if (recv_buf.size() > 0)
					continue;
				return 1;
			}

			ack_recv += cs_in[csid_]->data()->size();
			if (ack_recv > window_size)
			{
				send_ack(ack_recv);
				ack_recv = 0;
			}

			ret = handle_chunk_stream();
			if (ret < 0 || ret == 1)
				return ret;
		}

		if (ret == 0)
			ret = 1;
	}
	return ret;
}

//#include <cstdio>

int rtmp_session::read_chunk_stream()
{
	int ret = -1;
	if (recv_buf.size() < 1)
		return 1;

	if (!header_read)
	{
		uint8_t bh = recv_buf.front();
		recv_buf.pop_front();

		fmt_ = (bh >> 6) & 0x3;
		csid_ = bh & 0x3f;
		
		header_read = true;
	}

	if (cs_in.count(csid_) == 0)
		cs_in[csid_] = std::make_shared<rtmp_chunk_stream>(this, csid_);

	auto cs = cs_in[csid_];

	ret = cs->read_header(fmt_, csid_);
	if (ret == 0)
	{
		//printf("basic header: fmt=%d, csid:%d; message header: timestamp=%lu, msglen=%u, typeid:%d, msg streamid:%u, remain:%ld, recv buffer len:%lu\n", fmt_, csid_, cs_in[csid_]->time(), cs_in[csid_]->len(), cs_in[csid_]->type(), cs_in[csid_]->stream(), cs_in[csid_]->rem(), recv_buf.size());
		ret = cs->read_payload();
		if (ret == 0)
		{
			header_read = false;
		}
	}
	return ret;
}

int rtmp_session::handle_chunk_stream()
{
	int ret = -1;

	uint8_t type = cs_in[csid_]->type();

	if (type < 7 && cs_in[csid_]->stream() == 0)
		ret = handle_control_message();
	else if (type == 8 || type == 9 || type == 18)
		ret = handle_media_message();
	else if (type == 20)
		ret = handle_command_message();
	else
		LOG(WARN) << "<rtmp> " << "unhandled chunk with type: " << int(type);

	return ret;
}
