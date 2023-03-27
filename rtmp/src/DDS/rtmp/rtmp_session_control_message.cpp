#include <DDS/rtmp/rtmp_session.hpp>
#include <DDS/core/logger.hpp>

template <typename T>
class swap_endian
{
public:
    template <typename U>
    swap_endian(U&& u) : eswap({ std::forward<U>(u) })
    {
        for (uint8_t i = 0; i < sizeof(T) / 2; ++i)
            std::swap(eswap.u8[i], eswap.u8[sizeof(T) - i - 1]);
    }
    operator T && () { return std::move(eswap.u); }
private:
    union
    {
        T u;
        uint8_t u8[sizeof(T)];
    }eswap;
};


int rtmp_session::handle_control_message()
{
    uint8_t type = cs_in[csid_]->type();
    if (type == 1)
    {
        max_chunk_size = *reinterpret_cast<uint32_t*>(cs_in[csid_]->data()->data());
        max_chunk_size = swap_endian<uint32_t>(max_chunk_size);

        LOG(DEBUG) << "<rtmp> " << "new chunk size: " << int(max_chunk_size);
    }
    else if (type == 3)
    {
        uint32_t seq_num = *reinterpret_cast<uint32_t*>(cs_in[csid_]->data()->data());
        seq_num = swap_endian<uint32_t>(seq_num);
        LOG(DEBUG) << "<rtmp> " << "seq num received: " << seq_num;
    }
    else if (type == 4)
    {
        uint8_t* data = cs_in[csid_]->data()->data();

        uint16_t et = *reinterpret_cast<uint16_t*>(data);
        et = swap_endian<uint16_t>(et);

        if (et == 3)
        {
            uint32_t et_stream_id = *reinterpret_cast<uint32_t*>(data + sizeof(uint16_t));
            et_stream_id = swap_endian<uint32_t>(et_stream_id);

            uint32_t buffer_len = *reinterpret_cast<uint32_t*>(data + sizeof(uint16_t) + sizeof(uint32_t));
            buffer_len = swap_endian<uint32_t>(buffer_len);

            LOG(DEBUG) << "<rtmp> " << "client buffer len request: " << int(buffer_len) << " for stream id: " << int(et_stream_id);
        }
        else
        {
            LOG(WARN) << "<rtmp> " << "unimplemented control type: " << int(type) << "-" << int(et);
        }
    }
    else if (type == 5)
    {
        window_size = *reinterpret_cast<uint32_t*>(cs_in[csid_]->data()->data());
        window_size = swap_endian<uint32_t>(window_size);

        LOG(DEBUG) << "<rtmp> " << "window size set: " << int(window_size);
    }
    else
    {
        LOG(WARN) << "<rtmp> " << "unimplemented control type: " << int(type);
    }

    cs_in[csid_]->clear();
    return 0;
}


void rtmp_session::send_set_chunk_size()
{
    uint32_t msg = max_chunk_size;
    msg = swap_endian<uint32_t>(msg);

    write_chunk(2, 0, 0, 1, 0, reinterpret_cast<uint8_t*>(&msg), sizeof(msg), true);
}

void rtmp_session::send_ack()
{
    uint32_t msg = ack_recv_total;
    msg = swap_endian<uint32_t>(msg);

    write_chunk(2, 0, 0, 3, 0, reinterpret_cast<uint8_t*>(&msg), sizeof(msg), true);
}

void rtmp_session::send_stream_begin(uint32_t stream_id)
{
    uint8_t msg[6];
    msg[0] = 0;
    msg[1] = 0;

    msg[2] = (stream_id >> 24) & 0xFF;
    msg[3] = (stream_id >> 16) & 0xFF;
    msg[4] = (stream_id >>  8) & 0xFF;
    msg[5] = (stream_id >>  0) & 0xFF;

    write_chunk(2, 0, 0, 4, 0, msg, sizeof(msg), true);
}

void rtmp_session::send_window_ack()
{
    uint32_t msg = window_size;
    msg = swap_endian<uint32_t>(msg);

    write_chunk(2, 0, 0, 5, 0, reinterpret_cast<uint8_t*>(&msg), sizeof(msg), true);
}

void rtmp_session::send_set_peer_bandwidth()
{
    uint8_t msg[6];
    msg[0] = (peer_bandwidth >> 24) & 0xFF;
    msg[1] = (peer_bandwidth >> 16) & 0xFF;
    msg[2] = (peer_bandwidth >>  8) & 0xFF;
    msg[3] = (peer_bandwidth >>  0) & 0xFF;

    msg[4] = 0;
    msg[5] = 0;

    write_chunk(2, 0, 0, 6, 0, msg, sizeof(msg), true);
}