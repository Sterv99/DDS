#include <DDS/core/media/copypipe.hpp>

void media_copypipe::write_frame(AVFrame* f)
{
    for (auto& w : writers)
    {
        w->write_frame(f);
    }
}

void media_copypipe::add_writer(std::shared_ptr<AVFrameWriter> writer)
{
    writers.push_back(writer);
}

void media_copypipe::record(bool r)
{
    if(r)
    {
        if(rec_index == -1)
        {
            writers.emplace_back(new media_recorder());
            rec_index = writers.size()-1;
        }
    }
    else
    {
        if(rec_index != -1)
        {
            writers.erase(writers.cbegin() + rec_index);
            rec_index = -1;
        }
    }
}