//
// Created by ether on 2019/3/27.
//

#ifndef OBOEDEMO_OBOEPLAYER_H
#define OBOEDEMO_OBOEPLAYER_H

#include <oboe/Oboe.h>
#include "../util/LogUtil.h"
#include <thread>

#define LOG_TAG "OboePlayer"
using namespace oboe;

class OboePlayer {
public:
    OboePlayer(const char *path);

    ~OboePlayer();

    void readData(AudioStream *pStream);

private:
    AudioStreamBuilder *builder;
    Result result;
    AudioStream *playStream;
    FILE *file;

    void setBuilderParams(AudioStreamBuilder *builder);

    void createPlayBuilder();

};


#endif //OBOEDEMO_OBOEPLAYER_H
