# DDS

### Dependencies
* CMake >= 3.17
* Boost
* [FFMPEG](https://git.ffmpeg.org/ffmpeg.git) build with at least libx264 enabled
* [websocketpp](https://github.com/zaphoyd/websocketpp)
* OpenSSL
* libopencv

``` $ sudo apt-get install make cmake yasm nasm pkg-config libssl-dev libboost-all-dev libx264-dev```

I configured ffmpeg with:

``` $ ./configure  --ld="g++" --enable-gpl --disable-programs --disable-static --enable-shared --enable-libx264```

### Building
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```
---
rtmp module inspired by: https://github.com/grandi23/cpp_media_server-1
