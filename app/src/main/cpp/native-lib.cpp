#include <jni.h>
#include <string>
#include "audio/OboePlayer.h"

OboePlayer *oboePlayer;
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_oboedemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_oboedemo_Player_create(JNIEnv *env, jobject instance, jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);

    if (oboePlayer == nullptr) {
        oboePlayer = new OboePlayer(path);
    }

    env->ReleaseStringUTFChars(path_, path);
}