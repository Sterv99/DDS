#include <DDS/core/settings.hpp>


extern "C"
{
#include <libavformat/avformat.h>
//#include <libavcodec/avcodec.h>
//#include <libavutil/imgutils.h>
//#include <libavutil/time.h>
//#include <libswscale/swscale.h>
}

settings::settings()
{
    av_codec_id = AVCodecID::AV_CODEC_ID_H264;
    av_pix_fmt_id = AVPixelFormat::AV_PIX_FMT_YUV420P;
}
