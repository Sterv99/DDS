# DDS

### Dependencies
* CMake >= 3.17
* OpenSSL
* Boost
* [FFMPEG](https://git.ffmpeg.org/ffmpeg.git) build at least with libx264 enabled
* [websocketpp](https://github.com/zaphoyd/websocketpp)

``` $ sudo apt-get install make cmake yasm nasm pkg-config libssl-dev libboost-all-dev libx264-dev libvpx-dev ```

I configured ffmpeg with:

``` $ ./configure  --ld="g++" --enable-gpl --disable-programs --disable-static --enable-shared --enable-libx264 --enable-libvpx ```

### Building
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```
