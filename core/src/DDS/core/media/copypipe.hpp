#ifndef MEDIA_COPYPIPE_HPP
#define MEDIA_COPYPIPE_HPP

#include <vector>
#include <memory>
#include <DDS/core/media/recorder.hpp>
#include <DDS/core/media/avframe_writer.hpp>

class AVFrame;
class media_copypipe : public AVFrameWriter
{
public:
    unsigned count() const { return writers.size(); }
    void add_writer(std::shared_ptr<AVFrameWriter>);
    void record(bool);

    void write_frame(AVFrame*);
private:
    std::vector<std::shared_ptr<AVFrameWriter>> writers;

    int rec_index = -1;
};

#endif