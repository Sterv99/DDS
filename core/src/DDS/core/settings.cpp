#include <DDS/core/settings.hpp>
#include <DDS/core/logger.hpp>

extern "C"
{
#include <libavformat/avformat.h>
//#include <libavcodec/avcodec.h>
//#include <libavutil/imgutils.h>
//#include <libavutil/time.h>
//#include <libswscale/swscale.h>
}

structlog LOGCFG{};

settings::settings()
{
    record.av_codec_id = AVCodecID::AV_CODEC_ID_H264;
    record.av_pix_fmt_id = AVPixelFormat::AV_PIX_FMT_YUV420P;
    
#ifndef NDEBUG
    LOGCFG.level = DEBUG;
#endif
}

void settings::set_loglevel(int l)
{
#ifdef NDEBUG
    LOGCFG.level = static_cast<typelog>(l);
#endif
}
