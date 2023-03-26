#ifndef AVFRAME_WRITER_HPP
#define AVFRAME_WRITER_HPP

class AVFrame;
class AVFrameWriter
{
public:
    virtual void write_frame(AVFrame*) {}
};

#endif