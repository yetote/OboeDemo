//
// Created by ether on 2019/3/27.
//


#include "OboePlayer.h"

void OboePlayer::readData(AudioStream *pStream) {
    size_t wFrameCount = 0;
    Result result;
    char *buffer = new char[44100 * 2 * 2];
    do {
        wFrameCount = fread(buffer, 44100 * 2 * 2, 1, file);
        result = pStream->write(buffer, 44100, 200);
        if (result != Result::OK) {
            LOGE("写入失败,error%s", convertToText(result));
        }
    } while (wFrameCount >= 0);
}

static const char *audioFormatStr[] = {
        "Invalid   非法格式", // = -1,
        "Unspecified  自动格式", // = 0,
        "I16",
        "Float",
};
static const AudioFormat audioFormatEnum[] = {
        AudioFormat::Invalid,
        AudioFormat::Unspecified,
        AudioFormat::I16,
        AudioFormat::Float,
};
static const int32_t audioFormatCount = sizeof(audioFormatEnum) /
                                        sizeof(audioFormatEnum[0]);

const char *FormatToString(AudioFormat format) {
    for (int32_t i = 0; i < audioFormatCount; ++i) {
        if (audioFormatEnum[i] == format)
            return audioFormatStr[i];
    }
    return "UNKNOW_AUDIO_FORMAT";
}

const char *audioApiToString(AudioApi api) {
    switch (api) {
        case AudioApi::AAudio:
            return "AAUDIO";
        case AudioApi::OpenSLES:
            return "OpenSL";
        case AudioApi::Unspecified:
            return "Unspecified";
    }
}

void printAudioStreamInfo(AudioStream *stream) {
    LOGE("StreamID: %p", stream);

    LOGE("缓冲区容量: %d", stream->getBufferCapacityInFrames());
    LOGE("缓冲区大小: %d", stream->getBufferSizeInFrames());
    LOGE("一次读写的帧数: %d", stream->getFramesPerBurst());
    //欠载和过载在官方文档的描述里，大致是欠载-消费者消费的速度大于生产的速度，过载就是生产的速度大于消费的速度
    LOGE("欠载或过载的数量: %d", stream->getXRunCount());
    LOGE("采样率: %d", stream->getSampleRate());
    LOGE("声道布局: %d", stream->getChannelCount());
    LOGE("音频设备id: %d", stream->getDeviceId());
    LOGE("音频格式: %s", FormatToString(stream->getFormat()));
    LOGE("流的共享模式: %s", stream->getSharingMode() == SharingMode::Exclusive ?
                       "独占" : "共享");
    LOGE("使用的音频的API：%s", audioApiToString(stream->getAudioApi()));
    PerformanceMode perfMode = stream->getPerformanceMode();
    std::string perfModeDescription;
    switch (perfMode) {
        case PerformanceMode::None:
            perfModeDescription = "默认模式";
            break;
        case PerformanceMode::LowLatency:
            perfModeDescription = "低延迟";
            break;
        case PerformanceMode::PowerSaving:
            perfModeDescription = "节能";
            break;
    }
    LOGE("性能模式: %s", perfModeDescription.c_str());


    Direction dir = stream->getDirection();
    LOGE("流方向: %s", (dir == Direction::Output ? "OUTPUT" : "INPUT"));
    if (dir == Direction::Output) {
        LOGE("输出流读取的帧数: %d", (int32_t) stream->getFramesRead());
        LOGE("输出流写入的帧数: %d", (int32_t) stream->getFramesWritten());
    } else {
        LOGE("输入流读取的帧数: %d", (int32_t) stream->getFramesRead());
        LOGE("输入流写入的帧数: %d", (int32_t) stream->getFramesWritten());
    }
}

OboePlayer::OboePlayer(const char *path) {
    file = fopen(path, "rb+");
    createPlayBuilder();

}

void OboePlayer::setBuilderParams(AudioStreamBuilder *builder) {
    builder->setDirection(Direction::Output);
    builder->setChannelCount(ChannelCount::Stereo);
    builder->setFormat(AudioFormat::I16);
    builder->setPerformanceMode(PerformanceMode::LowLatency);
    builder->setSharingMode(SharingMode::Exclusive);
    builder->setSampleRate(44100);
//    builder->setAudioApi(AudioApi::OpenSLES);
}

void OboePlayer::createPlayBuilder() {
    AudioStreamBuilder playBuilder;
    setBuilderParams(&playBuilder);
    result = playBuilder.openStream(&playStream);
    if (result != Result::OK) {
        LOGE("打开流失败，error：%s", convertToText(result));
        return;
    }
    if (playStream == nullptr) {
        LOGE("创建流失败");
        return;
    }
    printAudioStreamInfo(playStream);
    result = playStream->requestStart();
    if (result != Result::OK) {
        LOGE("请求打开流失败%s", convertToText(result));
        return;
    }
    std::thread readDataThread(&OboePlayer::readData, this, playStream);
//    readData(playStream);
    readDataThread.detach();
}

