#include "decoder.h"

void Decoder::OpenVideoDecode()
{
    AVCodecParameters* pCodecParameters = videoStream->codecpar;
    AVCodec *vCodec = nullptr;
    vCodecContext = nullptr;

    qDebug() << "OpenVideoDecode thread: " << QThread::currentThread();

    vCodec = avcodec_find_decoder(pCodecParameters->codec_id);
    if(vCodec == NULL)
    {
        qDebug() << "ERROR unsupported codec!";
        goto end;
    }

    vCodecContext = avcodec_alloc_context3(vCodec);
    if(!vCodecContext)
    {
        qDebug() << "failed to allocated memory for AVCodecContext";
        goto end;
    }

    if(avcodec_parameters_to_context(vCodecContext, pCodecParameters) < 0)
    {
        qDebug() << "failed to copy codec params to codec context";
        goto end;
    }

    if(avcodec_open2(vCodecContext, vCodec, NULL) < 0)
    {
        qDebug() << "failed to open codec through avcodec_open2";
        goto end;
    }

    std::thread([&](Decoder *pointer)
    {
        pointer->VideoDecode();
    }, this).detach();

    return;

end:
    avcodec_free_context(&vCodecContext);
    return;
}

void Decoder::VideoDecode()
{

    AVFrame *frame = nullptr;
    AVFrame *yuyFrame = nullptr;

    unsigned char *buffer = nullptr;
    struct SwsContext *imgConvertCtx = nullptr;

    double video_pts = 0; //当前视频的pts
    double audio_pts = 0; //音频pts

    frame = av_frame_alloc();    // set its fields to default values


    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, vCodecContext->width, vCodecContext->height, 1);
//  unsigned int numBytes = static_cast<unsigned int>(yuvSize);
    buffer = static_cast<uint8_t *>(av_malloc(numBytes * sizeof(uint8_t)));
    yuyFrame = av_frame_alloc();

    av_image_fill_arrays(yuyFrame->data,
                         yuyFrame->linesize,
                         buffer,
                         AV_PIX_FMT_YUV420P,
                         vCodecContext->width,
                         vCodecContext->height,
                         1);

    //Reference: https://stackoverflow.com/questions/23067722/swscaler-warning-deprecated-pixel-format-used
    AVPixelFormat pixFormat;
    switch (vCodecContext->pix_fmt) {
        case AV_PIX_FMT_YUVJ420P :
            pixFormat = AV_PIX_FMT_YUV420P;
            break;
        case AV_PIX_FMT_YUVJ422P  :
            pixFormat = AV_PIX_FMT_YUV422P;
            break;
        case AV_PIX_FMT_YUVJ444P   :
            pixFormat = AV_PIX_FMT_YUV444P;
            break;
        case AV_PIX_FMT_YUVJ440P :
            pixFormat = AV_PIX_FMT_YUV440P;
            break;
        default:
            pixFormat = vCodecContext->pix_fmt;
    }

    imgConvertCtx = sws_getContext(vCodecContext->width,                    // 原始影像寬度
                                vCodecContext->height,                      // 原始影像高度
                                pixFormat,                              // 原始影像格式
                                vCodecContext->width,                       // 輸出影像寬度
                                vCodecContext->height,                      // 原始影像高度
                                AV_PIX_FMT_YUV420P,                     // 輸出影像格式
                                SWS_FAST_BILINEAR,                      // 變換尺寸方法
                                NULL,
                                NULL,
                                NULL);

    qDebug() << "VideoDecode thread: " << QThread::currentThread();

    AVPacket packet;
    while(decode_loop)
    {
//        qDebug() << "video decode ";

        if(videoPackList.size() <= 0)
        {

//            if(videoFinish)
//            {
//                decode_loop = false;
//                break;
//            }
            wait_mSec(10);
            continue;
        }

        packet = GetVideoPacket();


        if(avcodec_send_packet(vCodecContext, &packet) != 0)
        {
            av_packet_unref(&packet);
            continue;
        }

        while(avcodec_receive_frame(vCodecContext, frame) >= 0)
        {

            if (packet.dts == AV_NOPTS_VALUE && frame->opaque && *(uint64_t*) frame->opaque != AV_NOPTS_VALUE)
            {
                video_pts = *(uint64_t *) frame->opaque;
            }
            else if (packet.dts != AV_NOPTS_VALUE)
            {
                video_pts = packet.dts;
            }
            else
            {
                video_pts = 0;
            }

            video_pts *= av_q2d(videoStream->time_base);
            video_clock = video_pts;

            while (1)
            {
                if(audioStream != NULL)
                {
                    audio_pts = audio_clock;
                }
                else
                {
                    audio_pts = (av_gettime() - VideoStartTime) / 1000000.0;
                    audio_clock = audio_pts;
                }


                video_pts = video_clock;

                if(video_pts <= audio_pts) break;

                int delaytime = (video_pts - audio_pts) * 1000;
                delaytime = delaytime > 5 ? 5:delaytime;
                wait_mSec(delaytime);
            }


            sws_scale(imgConvertCtx,
                      frame->data,
                      frame->linesize,
                      0,
                      frame->height,
                      yuyFrame->data,
                      yuyFrame->linesize);

            Q_EMIT SendVideoData(buffer, frame->width, frame->height);



        }
        av_packet_unref(&packet);
    }


    avformat_close_input(&pFormatContext);
    avcodec_free_context(&vCodecContext);
    sws_freeContext(imgConvertCtx);
    av_frame_free(&frame);
    av_frame_free(&yuyFrame);
}
