/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlMessage.h"
#include "ObjectBox.h"

AlMessage *AlMessage::obtain() {
    AlMessage *msg = AlMessageManager::getInstance()->popOne();
    if (msg) {
        msg->what = 0;
        msg->obj = nullptr;
        msg->arg1 = 0;
        msg->arg2 = 0;
        msg->desc = "Undef";
        msg->queueMode = QUEUE_MODE_NORMAL;
        return msg;
    }
    return new AlMessage();
}

AlMessage *AlMessage::obtain(int32_t what) {
    AlMessage *msg = AlMessageManager::getInstance()->popOne();
    if (msg) {
        msg->what = what;
        msg->obj = nullptr;
        msg->arg1 = 0;
        msg->arg2 = 0;
        msg->desc = "Undef";
        msg->queueMode = QUEUE_MODE_NORMAL;
        return msg;
    }
    return new AlMessage(what);
}

AlMessage *AlMessage::obtain(int32_t what, Object *obj) {
    AlMessage *msg = AlMessageManager::getInstance()->popOne();
    if (msg) {
        msg->what = what;
        msg->obj = obj;
        msg->arg1 = 0;
        msg->arg2 = 0;
        msg->desc = "Undef";
        msg->queueMode = QUEUE_MODE_NORMAL;
        return msg;
    }
    return new AlMessage(what, obj);
}

AlMessage *AlMessage::obtain(int32_t what, Object *obj, int16_t queueMode) {
    AlMessage *msg = AlMessageManager::getInstance()->popOne();
    if (msg) {
        msg->what = what;
        msg->obj = obj;
        msg->arg1 = 0;
        msg->arg2 = 0;
        msg->desc = "Undef";
        msg->queueMode = queueMode;
        return msg;
    }
    return new AlMessage(what, obj, queueMode);
}

AlMessage::AlMessage()
        : AlMessage(0) {

}

AlMessage::AlMessage(int32_t what)
        : AlMessage(what, nullptr) {

}

AlMessage::AlMessage(int32_t what,
                     Object *obj)
        : AlMessage(what, obj, QUEUE_MODE_NORMAL) {
}

AlMessage::AlMessage(int32_t what,
                     Object *obj,
                     int16_t queueMode)
        : Object() {
    this->what = what;
    this->obj = obj;
    this->arg1 = 0;
    this->arg2 = 0;
    this->desc = "Undef";
    this->queueMode = queueMode;
}

AlMessage::~AlMessage() {
    if (obj) {
        delete obj;
        obj = nullptr;
    }
}

void *AlMessage::tyrUnBox() {
    ObjectBox *ob = dynamic_cast<ObjectBox *>(obj);
    return ob->ptr;
}

AlMessageManager *AlMessageManager::instance = new AlMessageManager();

AlMessageManager *AlMessageManager::getInstance() {
    return instance;
}

AlMessage *AlMessageManager::popOne() {
    std::lock_guard<std::mutex> guard(poolMtx);
    if (!pool.empty()) {
        AlMessage *msg = pool.front();
        pool.pop();
        return msg;
    }
    return nullptr;
}

void AlMessageManager::recycle(AlMessage *msg) {
    if (nullptr == msg) {
        return;
    }
    std::lock_guard<std::mutex> guard(poolMtx);
    if (msg->obj) {
        delete msg->obj;
        msg->obj = nullptr;
    }
    pool.push(msg);
}