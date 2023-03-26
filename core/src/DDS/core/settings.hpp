#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <map>
#include <string>

class settings
{
public:
	settings(settings const&) = delete;
	void operator=(settings const&) = delete;
	static settings& get()
	{
		static settings instance;
		return instance;
	}

	void set_loglevel(int);

	struct
	{
		unsigned width = 1920, height = 1080;
		int fps = 20;
		int av_codec_id;
		int av_pix_fmt_id;
		int gop_size = 12;
	} record;

	std::map<std::string, int> dint;
private:
	settings();
};




#endif // !SETTINGS_HPP
