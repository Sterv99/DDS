#include <DDS/rtmp/flv_decoder.hpp>
#include <DDS/core/media/manager.hpp>

extern "C"
{
#include <libavcodec/avcodec.h>
}


static int flv_read_packet(MEDIA_PACKET_PTR pkt_ptr, AVCodecContext* cc, AVPacket* pkt)
{
    int ret;
    size_t size = pkt_ptr->buffer_ptr_->data_len();
    uint8_t* pb = reinterpret_cast<uint8_t*>(pkt_ptr->buffer_ptr_->data());

    pb++;
    size--;

    //if (pkt_ptr->is_key_frame_)
        //ret = av_add_index_entry(st, 0, pkt_ptr->dts_, size, 0, AVINDEX_KEYFRAME);

    pb += 4;
    size -= 4;

    if (pkt_ptr->is_seq_hdr_)
    {
        av_free(cc->extradata);
        cc->extradata = static_cast<uint8_t*>(av_mallocz(size + AV_INPUT_BUFFER_PADDING_SIZE));
        cc->extradata_size = size;
        memcpy(cc->extradata, pb, size);

        ret = AVERROR(EAGAIN);
    }
    else
    {
        if (!size)
            ret = AVERROR(EAGAIN);
        else
        {
            pkt->data = static_cast<uint8_t*>(av_realloc(pkt->data, size + AV_INPUT_BUFFER_PADDING_SIZE));
            memcpy(pkt->data, pb, size);
            ret = size;

            pkt->pos = -1;
            pkt->size = size;
            pkt->dts = pkt_ptr->dts_;
            pkt->pts = pkt_ptr->pts_;

            if (pkt_ptr->is_key_frame_)
                pkt->flags != AV_PKT_FLAG_KEY;

        }
    }

    return ret;
}

flv_decoder::flv_decoder(int av_codec_id, ClientID_t client_id)
    : media_decoder(av_codec_id), cid(client_id)
{
    pkt = av_packet_alloc();
}

flv_decoder::~flv_decoder()
{
    av_packet_unref(pkt);
    av_packet_free(&pkt);
}

int flv_decoder::decode(MEDIA_PACKET_PTR pkt_ptr)
{
    if (pkt_ptr->av_type_ != MEDIA_VIDEO_TYPE)
        return 0;
    int ret;

    if (!avcodec_is_open(cc_))
    {
        ret = flv_read_packet(pkt_ptr, cc_, pkt);
        open();
    }
    else
    {
        ret = flv_read_packet(pkt_ptr, cc_, pkt);
    }


    if (pkt->size)
        media_decoder::decode(pkt);

	return 0;
}

void flv_decoder::on_decoded(AVFrame* frame)
{
    auto pipe = media_manager::get().pipe(cid);
    //if(pipe)
        pipe.write_frame(frame);
}


void flv_decoder_wrap::write_packet(MEDIA_PACKET_PTR pkt_ptr)
{
    if (!dec)
    {
        ClientID_t cid = std::stoul(pkt_ptr->streamname_);
        if (pkt_ptr->codec_type_ == MEDIA_CODEC_H264)
            dec = new flv_decoder(AVCodecID::AV_CODEC_ID_H264, cid);
        else if (pkt_ptr->codec_type_ == MEDIA_CODEC_H265)
            dec = new flv_decoder(AVCodecID::AV_CODEC_ID_H265, cid);
    }
    dec->decode(pkt_ptr);
}
