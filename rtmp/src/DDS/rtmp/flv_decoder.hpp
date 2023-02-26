#ifndef FLV_DECODER_HPP
#define FLV_DECODER_HPP

#include <DDS/rtmp/utils/av/media_packet.hpp>
#include <DDS/core/media/decoder.hpp>
#include <DDS/core/client.hpp>


class AVPacket;
class AVFrame;
class flv_decoder : media_decoder
{
public:
    flv_decoder(int av_codec_id, ClientID_t cid);
	~flv_decoder();

    int decode(MEDIA_PACKET_PTR);
private:
    ClientID_t cid;
    AVPacket* pkt = nullptr;

    void on_decoded(AVFrame*);
};


class flv_decoder_wrap
{
    flv_decoder* dec = nullptr;
public:
    ~flv_decoder_wrap() { delete dec; }

    void write_packet(MEDIA_PACKET_PTR pkt_ptr);
};

#endif // !MEDIA_RECORDER_HPP
