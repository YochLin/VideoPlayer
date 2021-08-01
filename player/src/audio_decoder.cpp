#include "decoder.h"

bool Decoder::OpenSDL()
{
    SDL_AudioSpec wanted_spec, spec;

    wanted_spec.freq = aCodecContext->sample_rate;      // 採樣率
    wanted_spec.channels  = aCodecContext->channels;    // 聲道數
    wanted_spec.samples = FFMAX(512, 2 << av_log2(wanted_spec.freq / 30));
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.silence = 0;
    wanted_spec.callback = SDLAudioCallBackFunc;
    wanted_spec.userdata = this;

    int num = SDL_GetNumAudioDevices(0);
    qDebug() << "Audio Device" << num;
    for(int i = 0; i < num; i++)
    {
        qDebug("Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
        audioID = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(i,0), false, &wanted_spec, &spec, 0);
        if(audioID > 0)
        {
            break;
        }
    }

    qDebug() << "Audio Device ID" << audioID;
    qDebug() << spec.freq;

    if(audioID < 0)
    {
        return false;
    }

    return true;
}

void Decoder::CloseSDL()
{
    if(audioID >= 0)
    {
        SDL_LockAudioDevice(audioID);
        SDL_PauseAudioDevice(audioID, 1);
        SDL_UnlockAudioDevice(audioID);
    }
    audioID = -1;
}

void Decoder::OpenAudioDecode()
{
    if (SDL_Init(SDL_INIT_AUDIO))
    {
        qDebug("Could not initialize SDL - %s. \n", SDL_GetError());
        return;
    }

    AVCodecParameters* pCodecParameters = audioStream->codecpar;
    AVCodec *aCodec = nullptr;
    aCodecContext = nullptr;



    qDebug() << "OpenAudioDecode thread: " << QThread::currentThread();

    aCodec = avcodec_find_decoder(pCodecParameters->codec_id);
    if(aCodec == NULL)
    {
        qDebug() << "ERROR unsupported codec!";
        goto end;
    }

    aCodecContext = avcodec_alloc_context3(aCodec);
    if(!aCodecContext)
    {
        qDebug() << "failed to allocated memory for AVCodecContext";
        goto end;
    }

    if(avcodec_parameters_to_context(aCodecContext, pCodecParameters) < 0)
    {
        qDebug() << "failed to copy codec params to codec context";
        goto end;
    }

    if(avcodec_open2(aCodecContext, aCodec, NULL) < 0)
    {
        qDebug() << "failed to open codec through avcodec_open2";
        goto end;
    }

    aCodecContext->pkt_timebase = audioStream->time_base;  // 需先設置否則會有(Could not update timestamps for skipped samples. 問題)

    swrCtx = swr_alloc_set_opts(NULL,
                                av_get_default_channel_layout(aCodecContext->channels),
                                AV_SAMPLE_FMT_S16,
                                aCodecContext->sample_rate,
                                av_get_default_channel_layout(aCodecContext->channels),
                                aCodecContext->sample_fmt,
                                aCodecContext->sample_rate,
                                0,
                                NULL);

    if(swr_init(swrCtx) < 0)
    {
        qDebug() << "Could not open resample";
        goto end;
    }


    if(OpenSDL())
    {
        qDebug() << "Open SDL";
        SDL_LockAudioDevice(audioID);
        SDL_PauseAudioDevice(audioID,0);
        SDL_UnlockAudioDevice(audioID);
    }

    return;

end:
    avcodec_free_context(&aCodecContext);
    swr_free(&swrCtx);
    return;
}

void Decoder::SDLAudioCallBackFunc(void *userdata, Uint8 *stream, int len)
{
    qDebug() << "SDLAudioCallBackFunc thread: " << QThread::currentThread();
    Decoder *decoder = (Decoder *)userdata;
    decoder->SDLAudioCallBack(stream, len);
}

void Decoder::SDLAudioCallBack(Uint8 *stream, int len)
{

    int audio_buf_size = 0;          // 新取得的聲音資料大小
    int audio_buf_index = 0;         // 發送給設備的數據量
    int get_audio_size = 0;          // 解碼後的聲音資料大小

    int tmp_len = 0;

    while(len > 0)
    {
        if(audio_buf_index >= audio_buf_size)
        {
            get_audio_size = AudioDecode();
            qDebug() << "Get size" << get_audio_size;
            if(get_audio_size <= 0)
            {
                audio_buf_size = 1024;
                memset(audio_buff, 0, audio_buf_size);
            }
            else
            {
                audio_buf_size = get_audio_size;
            }

            audio_buf_index = 0;
        }

        if(audio_buff == NULL) return;;

        tmp_len = audio_buf_size - audio_buf_index;
        if(tmp_len > len)
        {
            tmp_len = len;
        }
        // 將解碼後的資訊(audio_buff)寫入到設備緩衝區(stream)內
        memcpy(stream, (uint8_t *)audio_buff + audio_buf_index, tmp_len);
        len -= tmp_len;
        stream += tmp_len;
        audio_buf_index += tmp_len;
    }
}

int Decoder::AudioDecode()
{
    int audioBufferSize = 0;
    AVFrame *audioFrame = nullptr;
    AVFrame *audioResample = nullptr;

    audioFrame = av_frame_alloc();

    AVPacket packet;
    while(decode_loop)
    {

        if(audioPackList.size() <= 0)
        {
            wait_mSec(10);
            break;
        }

        packet = GetAudioPacket();

        if(packet.pts != AV_NOPTS_VALUE)
        {
            audio_clock = av_q2d(audioStream->time_base) * packet.pts;
        }

        if(avcodec_send_packet(aCodecContext, &packet) != 0)
        {
            av_packet_unref(&packet);
            continue;
        }

        while(avcodec_receive_frame(aCodecContext, audioFrame) >= 0)
        {

//            int nb_samples = av_rescale_rnd(swr_get_delay(swrCtx, aCodecContext->sample_rate) + audioFrame->nb_samples, aCodecContext->sample_rate, aCodecContext->sample_rate, AV_ROUND_UP);

//            qDebug() << "nb_samples" << nb_samples << audioFrame->nb_samples;
            audioResample = av_frame_alloc();
            audioResample->format = AV_SAMPLE_FMT_S16;
            audioResample->channel_layout = av_get_default_channel_layout(aCodecContext->channels);
            audioResample->sample_rate = audioFrame->sample_rate;
            audioResample->nb_samples = audioFrame->nb_samples;


            int ret = av_samples_fill_arrays(audioResample->data,
                                             audioResample->linesize,
                                             audio_buff,
                                             audioFrame->channels,
                                             audioFrame->nb_samples,
                                             AV_SAMPLE_FMT_S16,
                                             0);
            if(ret < 0)
            {
                qDebug() << "Error allocating audio buffer";
                continue;
            }

            int len = swr_convert(swrCtx, audioResample->data, audioResample->nb_samples, (const uint8_t**)audioFrame->data, audioFrame->nb_samples);

//            audioBufferSize = len * audioFrame->channels * av_get_bytes_per_sample(aCodecContext->sample_fmt);
            audioBufferSize = av_samples_get_buffer_size(NULL,
                                                         audioFrame->channels,
                                                         audioFrame->nb_samples,
                                                         AV_SAMPLE_FMT_S16,
                                                         1);

        }

        break;
    }
    av_packet_unref(&packet);
    av_frame_free(&audioResample);
    av_frame_free(&audioFrame);
    return audioBufferSize;
}
