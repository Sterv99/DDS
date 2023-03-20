#ifndef MEDIA_COPYPIPE_HPP
#define MEDIA_COPYPIPE_HPP

#include <unordered_map>
#include <DDS/core/client.hpp>
#include <DDS/core/media/recorder.hpp>

class AVFrame;
class media_copypipe
{
public:
    unsigned count() const { return (rec != nullptr) + operators.size(); }
    void write_frame(AVFrame*);

    void add_operator_rtp(Client*, uint16_t port);
    void record(bool);
private:
    media_recorder* rec = nullptr;
    
    std::unordered_map<Client*, uint16_t> operators;
};

#endif