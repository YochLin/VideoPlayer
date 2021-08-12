#ifndef VIDEO_EVENT_CALLBACK_H
#define VIDEO_EVENT_CALLBACK_H

#include <Windows.h>
#include <stdint.h>

class VideoEventCallBack
{
public:
    virtual void totalTimeChange(const int64_t &uSec) = 0;
};

#endif // VIDEO_EVENT_CALLBACK_H
