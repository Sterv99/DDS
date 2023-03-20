#ifndef MEDIA_RECORDER_HPP
#define MEDIA_RECORDER_HPP

#include <cstdint>
#include <DDS/core/media/encoder.hpp>

class AVFormatContext;
class AVPacket;
class AVStream;
class AVFrame;
class SwsContext;
class media_recorder : media_encoder
{
public:
    media_recorder();
    ~media_recorder();

    void write_frame(AVFrame*);
private:
    AVFormatContext* oc = nullptr;
    AVStream* vs = nullptr;

    AVFrame* nf = nullptr;
    SwsContext* swsctx = nullptr;

    int64_t ts = 0;

    void on_encoded(AVPacket*);
};

#endif // !MEDIA_RECORDER
