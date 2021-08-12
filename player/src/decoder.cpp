#include "decoder.h"

Decoder::Decoder()
{
    decode_loop = false;
    videoFinish = false;

    sdl_audio_mutex = SDL_CreateMutex();
    sdl_audio_cond = SDL_CreateCond();

    sdl_video_mutex = SDL_CreateMutex();
    sdl_video_cond = SDL_CreateCond();

    videoCallBack = nullptr;
}

Decoder::~Decoder()
{

}

void Decoder::InputVideoPacket(AVPacket &packet)
{
    SDL_LockMutex(sdl_video_mutex);
    videoPackList.push_back(packet);
    SDL_CondSignal(sdl_video_cond);         //發送條件訊號，方便等待數據的地方唤醒
    SDL_UnlockMutex(sdl_video_mutex);       //解锁
}

void Decoder::InputAudioPacket(AVPacket &packet)
{
    SDL_LockMutex(sdl_audio_mutex);
    audioPackList.push_back(packet);
    SDL_CondSignal(sdl_audio_cond);         //發送條件訊號，方便等待數據的地方唤醒
    SDL_UnlockMutex(sdl_audio_mutex);       //解锁
}

AVPacket Decoder::GetVideoPacket()
{
    AVPacket p;
    SDL_LockMutex(sdl_video_mutex);
    p = videoPackList.front();
    videoPackList.pop_front();
    SDL_UnlockMutex(sdl_video_mutex);       //解锁
    return p;
}

AVPacket Decoder::GetAudioPacket()
{
    AVPacket p;
    SDL_LockMutex(sdl_audio_mutex);
    p = audioPackList.front();
    audioPackList.pop_front();
    SDL_UnlockMutex(sdl_audio_mutex);       //解锁
    return p;
}


void Decoder::wait_mSec(int msec)
{
    QEventLoop eventloop;
    QTimer::singleShot(msec, &eventloop, SLOT(quit()));
    eventloop.exec();
}

void Decoder::InitDecoder(QString path)
{
    const char *video_path = path.toStdString().c_str();

    qDebug() << video_path;

    pFormatContext = avformat_alloc_context();

    decode_loop = true;
    video_index = -1;
    audio_index = -1;

    videoStream = nullptr;
    audioStream = nullptr;

    aCodecContext = nullptr;
    vCodecContext = nullptr;
    swrCtx = nullptr;


    AVDictionary* opts = NULL;
    av_dict_set(&opts, "rtsp_transport", "tcp", 0);           //tcp or udp，默認優先 tcp 再嘗試 udp
    av_dict_set(&opts, "stimeout", "3000000", 0);             // 超過 3 秒

    if (avformat_open_input(&pFormatContext, video_path, NULL, &opts) != 0)
    {
        qDebug() << "Read time out";
        goto end;
    }

    if(avformat_find_stream_info(pFormatContext, NULL) < 0)
    {
        qDebug() << "ERROR could not get the stream info";
        goto end;
    }

    for(int i = 0; i < pFormatContext->nb_streams; i++)
    {
        AVCodecParameters *pLocalCodecParameters = nullptr;
        pLocalCodecParameters = pFormatContext->streams[i]->codecpar;
        if(pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_index = i;
        }
        if(pLocalCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            audio_index = i;
        }
    }


    qDebug() << "InitDecoder thread: " << QThread::currentThread();
    qDebug() << video_index << audio_index;

    if(video_index >= 0)
    {
        videoStream = pFormatContext->streams[video_index];
        OpenVideoDecode();
    }

    if(audio_index >= 0)
    {
        audioStream = pFormatContext->streams[audio_index];
        OpenAudioDecode();
    }

    VideoStartTime = av_gettime();

    if(videoCallBack != nullptr)
    {
        videoCallBack->totalTimeChange(pFormatContext->duration);
        qDebug() << "get total time";
    }


    while(decode_loop)
    {
        qDebug() << audioPackList.size() << videoPackList.size();
        if(audioPackList.size() > MAX_AUDIO_SIZE || videoPackList.size() > MAX_VIDEO_SIZE)
        {
            wait_mSec(10);
            continue;
        }

        AVPacket packetCodec;
        if(av_read_frame(pFormatContext, &packetCodec) < 0)
        {
            videoFinish = true;
            continue;
        }

        if(packetCodec.stream_index == video_index)
        {
            InputVideoPacket(packetCodec);
        }
        else if(packetCodec.stream_index == audio_index)
        {
            InputAudioPacket(packetCodec);
        }
        else
        {
            av_packet_unref(&packetCodec);
        }
    }

end:
    CloseSDL();
    avformat_close_input(&pFormatContext);
    avcodec_free_context(&vCodecContext);
    avcodec_free_context(&aCodecContext);
    swr_free(&swrCtx);
    return;
}




