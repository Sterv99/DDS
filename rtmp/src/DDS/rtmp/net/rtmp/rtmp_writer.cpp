#include <DDS/rtmp/net/rtmp/rtmp_writer.hpp>
#include <DDS/rtmp/net/rtmp/chunk_stream.hpp>
#include <DDS/rtmp/net/rtmp/rtmp_server_session.hpp>
#include <DDS/rtmp/format/flv/flv_pub.hpp>
#include <DDS/rtmp/utils/logger.hpp>
//#include <stdio.h>



rtmp_writer::rtmp_writer(rtmp_server_session* session):session_(session)
{
}

rtmp_writer::~rtmp_writer()
{
}
int rtmp_writer::write_packet(MEDIA_PACKET_PTR pkt_ptr) {
    uint16_t csid;
    uint8_t  type_id;

    if (pkt_ptr->av_type_ == MEDIA_VIDEO_TYPE) {
        csid = 6;
        type_id = RTMP_MEDIA_PACKET_VIDEO;
    } else if (pkt_ptr->av_type_ == MEDIA_AUDIO_TYPE) {
        csid = 4;
        type_id = RTMP_MEDIA_PACKET_AUDIO;
    } else if (pkt_ptr->av_type_ == MEDIA_METADATA_TYPE) {
        csid = 6;
        type_id = pkt_ptr->typeid_;
    } else {
        log_errorf("doesn't support av type:%d", (int)pkt_ptr->av_type_);
        return -1;
    }
    write_data_by_chunk_stream(session_, csid,
        pkt_ptr->dts_, type_id,
        pkt_ptr->streamid_, session_->get_chunk_size(),
        pkt_ptr->buffer_ptr_);
    return RTMP_OK;
}

std::string rtmp_writer::get_key() {
    return session_->req_.key_;
}

std::string rtmp_writer::get_writerid() {
    return session_->get_sesson_key();
}

void rtmp_writer::close_writer() {
    delete this;
}

bool rtmp_writer::is_inited() {
    return init_flag_;
}

void rtmp_writer::set_init_flag(bool flag) {
    init_flag_ = flag;
}
