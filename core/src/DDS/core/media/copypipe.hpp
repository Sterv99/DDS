#ifndef MEDIA_COPYPIPE_HPP
#define MEDIA_COPYPIPE_HPP

#include <list>
#include <DDS/core/client.hpp>
#include <DDS/core/media/recorder.hpp>

class AVFrame;
class media_copypipe
{
public:
    unsigned count() const { return record + operators.size(); }
    void write_frame(AVFrame*) {}
private:
    bool record = false;
    media_recorder* rec;
    
    std::list<Client*> operators;
};

#endif