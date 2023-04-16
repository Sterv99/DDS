#include <DDS/core/media/recorder.hpp>
#include <DDS/core/settings.hpp>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}


media_recorder::media_recorder()
    : media_encoder(settings::get().record.av_codec_id)
{
    auto& sett = settings::get();

    int ret;
    ret = avformat_alloc_output_context2(&oc, nullptr, nullptr, "abc.mp4");
    ret = avio_open(&oc->pb, "abc.mp4", AVIO_FLAG_WRITE);

    oc->video_codec = c_;
    oc->video_codec_id = c_->id;


    const AVRational dst_fps = { sett.record.fps, 1 };

    cc_->codec_tag = 0;
    cc_->codec_id = static_cast<AVCodecID>(sett.record.av_codec_id);
    cc_->codec_type = AVMEDIA_TYPE_VIDEO;
    cc_->width = sett.record.width;
    cc_->height = sett.record.height;
    cc_->gop_size = sett.record.gop_size;
    cc_->pix_fmt = static_cast<AVPixelFormat>(sett.record.av_pix_fmt_id);
    cc_->framerate = dst_fps;
    cc_->time_base = av_inv_q(dst_fps);
    //video_enc_c->bit_rate = 3500 * 1024;
    if (oc->oformat->flags & AVFMT_GLOBALHEADER) {
        cc_->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    //ret = avcodec_parameters_from_context(vs->codecpar, video_enc_c);

    //imgbuf.resize(height * width * 3 + 16);

    //AVDictionary* codec_options = nullptr;
    //ret = av_dict_set(&codec_options, "profile", "high444", 0);
    // av_dict_set(&codec_options, "preset", "superfast", 0);
    // av_dict_set(&codec_options, "tune", "zerolatency", 0);

    vs = avformat_new_stream(oc, c_);

    ret = open();

    ret = avcodec_parameters_from_context(vs->codecpar, cc_);
    

    nf = av_frame_alloc();

    av_dump_format(oc, 0, "abc.mp4", 1);
    ret = avformat_write_header(oc, nullptr);

    //vf = allocate_frame_buffer(video_dec_c, width, height);
    //vf->pts = 0;

    /*swsctx = sws_getContext(width, height, AV_PIX_FMT_BGR24, width, height, vcc->pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr); */
}



media_recorder::~media_recorder()
{
    av_write_trailer(oc);

    av_frame_free(&nf);

    if (oc && !(oc->flags & AVFMT_NOFILE))
        avio_closep(&oc->pb);
    avformat_free_context(oc);
}


void media_recorder::write_frame(const ClientID_t, AVFrame* frame)
{
    int ret;

    if(!swsctx)
        swsctx = sws_getContext(frame->width, frame->height, (AVPixelFormat)frame->format, frame->width, frame->height, cc_->pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);

    ret = av_frame_make_writable(nf);
    ret = sws_scale_frame(swsctx, nf, frame);
    nf->pts = ts++;

    encode(nf);
}

void media_recorder::on_encoded(AVPacket* pkt)
{
    av_packet_rescale_ts(pkt, cc_->time_base, vs->time_base);
    int ret = av_interleaved_write_frame(oc, pkt);
}


