#include <android/log.h>
#include <stdlib.h>
#include "lame.h"
#include "audio_encoder.h"

#define info(fmt, ...) do{__android_log_print(ANDROID_LOG_INFO, "jug", fmt, ## __VA_ARGS__);}while(0)

struct lame_ctx
{
    lame_global_flags *gfp;
    int bytes_per_sample;
    int channels;
};

/*
 * Class:     jug_ndk_audio_encoder_AudioEncoder
 * Method:    allocate
 * Signature: (III)Z
 */
JNIEXPORT jboolean JNICALL Java_jug_ndk_audio_encoder_AudioEncoder_allocate
(JNIEnv *env, jobject thiz, jint channels, jint bytesInSample, jint sampleRate)
{
    info("invoke allocate");
    return JNI_FALSE;
}

/*
 * Class:     jug_ndk_audio_encoder_AudioEncoder
 * Method:    encode
 * Signature: (I[B[B)I
 */
JNIEXPORT jint JNICALL Java_jug_ndk_audio_encoder_AudioEncoder_encode
  (JNIEnv *env, jobject thiz, jint inputSize, jbyteArray inBuffer, jbyteArray outBuffer)
{
    info("invoke encode");
    return -1;
}

/*
 * Class:     jug_ndk_audio_encoder_AudioEncoder
 * Method:    destroy
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jug_ndk_audio_encoder_AudioEncoder_destroy
  (JNIEnv *env, jobject thiz)
{
    info("invoke destroy");
}

