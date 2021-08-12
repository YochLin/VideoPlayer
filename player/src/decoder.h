#ifndef DECODER_H
#define DECODER_H

#include <QObject>
#include <QMutex>
#include <list>
#include <thread>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QEventLoop>

#include "video_event_callback.h"

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/time.h>
    #include <libavutil/pixfmt.h>
    #include <libavutil/display.h>
    #include <libavutil/avstring.h>
    #include <libavutil/opt.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
    #include <libavutil/imgutils.h>
    #include <libavfilter/avfilter.h>
    #include <libavfilter/buffersink.h>
    #include <libavfilter/buffersrc.h>

    #include <SDL.h>
    #include <SDL_audio.h>
    #include <SDL_types.h>
    #include <SDL_name.h>
    #include <SDL_main.h>
    #include <SDL_config.h>
}

#define MAX_AUDIO_FRAME_SIZE 192000
#define MAX_AUDIO_SIZE (50, 50)
#define MAX_VIDEO_SIZE (50, 20)

class Decoder : public QObject
{
    Q_OBJECT

public:
    Decoder();
    ~Decoder();

//    void play();
//    void pause();
//    void stop();

    bool decode_loop;
    void SetVideoCallBack(VideoEventCallBack *pointer){videoCallBack=pointer;}

Q_SIGNALS:
    void SendVideoData(uint8_t *data, int width, int height);

public Q_SLOTS:
    void InitDecoder(QString path);


private:
//    unsigned char *frame;

    AVFormatContext *pFormatContext;

    void wait_mSec(int msec);
    int64_t videoTotalTime;

    // video
    AVCodecContext *vCodecContext;
    AVStream *videoStream;
    int video_index;
    bool videoFinish;
    QList<AVPacket> videoPackList;
    double video_clock;                   // audio frame time
    SDL_mutex *sdl_video_mutex;           //互斥锁
    SDL_cond *sdl_video_cond;             //条件变量

    void InputVideoPacket(AVPacket &p);
    AVPacket GetVideoPacket();
    void OpenVideoDecode();
    void VideoDecode();

    //audio
    AVCodecContext *aCodecContext;
    AVStream *audioStream;
    QMutex audio_mutex;
    SDL_AudioDeviceID audioID;             // 音響設備
    int audio_index;
    QList<AVPacket> audioPackList;
    uint8_t audio_buff[(MAX_AUDIO_FRAME_SIZE*3)/2];
    SwrContext *swrCtx;
    double audio_clock;                   // audio frame time
    SDL_mutex *sdl_audio_mutex;           //互斥锁
    SDL_cond *sdl_audio_cond;             //条件变量

    void InputAudioPacket(AVPacket &p);
    AVPacket GetAudioPacket();
    bool OpenSDL();
    void CloseSDL();
    void SDLAudioCallBack(Uint8 *stream, int len);
    static void SDLAudioCallBackFunc(void *userdata, Uint8 *stream, int len);
    int AudioDecode();
    void OpenAudioDecode();

    // 影音同步
    uint64_t VideoStartTime;
    int64_t getTotalTime();

    // 事件回條函數
    VideoEventCallBack *videoCallBack;


};


#endif // DECODER_H
