//
// Created by ether on 2019/3/28.
//

#ifndef OBOEDEMO_LOGUTIL_H
#define OBOEDEMO_LOGUTIL_H

#endif //OBOEDEMO_LOGUTIL_H

#include <android/log.h>

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)