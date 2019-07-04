/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWVIDEOOUTPUT_H
#define HWVC_ANDROID_HWVIDEOOUTPUT_H

#include "Unit.h"
#include "../include/HwFFmpegEncoder.h"
#include "../include/HwVideoFrame.h"
#include <atomic>

class HwVideoOutput : public Unit {
public:
    HwVideoOutput();

    virtual ~HwVideoOutput();

    bool eventPrepare(Message *msg);

    bool eventRelease(Message *msg) override;

    /**
     * Response read pixels message.
     */
    bool eventResponsePixels(Message *msg);

    bool eventWrite(Message *msg);

    bool eventStart(Message *msg);

    bool eventPause(Message *msg);

private:
    void write(HwBuffer *buf);

private:
    HwFFmpegEncoder *encoder = nullptr;
    HwVideoFrame *videoFrame = nullptr;
    std::atomic_bool recording;
    int count = 0;
};


#endif //HWVC_ANDROID_HWVIDEOOUTPUT_H