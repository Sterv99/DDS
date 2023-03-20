#include <DDS/core/media/copypipe.hpp>

void media_copypipe::write_frame(AVFrame* f)
{
    if(rec)
        rec->write_frame(f);
    
}

void media_copypipe::add_operator_rtp(Client* client, uint16_t port)
{
    if(client->type != Client::Type::OPERATOR)
        return;

    if(operators.count(client) > 0)
        return;

    operators[client] = port;
}

void media_copypipe::record(bool r)
{
    if(r)
    {
        if(!rec)
        {
            rec = new media_recorder();
        }
    }
    else
    {
        if(rec)
        {
            delete rec;
            rec = nullptr;
        }
    }
}