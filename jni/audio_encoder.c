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
    jclass cls = (*env)->GetObjectClass(env, thiz);
    jfieldID lame_pointer_field = (*env)->GetFieldID(env, cls, "nativePointerAddress", "I");
    int my_lame_addr = (*env)->GetIntField(env, thiz, lame_pointer_field);
    if(my_lame_addr != 0)
    {
        info("native lame address is not null.");
        return JNI_FALSE;
    }

    lame_global_flags *gfp = NULL;
    gfp = lame_init();
    if(gfp == NULL)
    {
        info("cannot allocate lame");
        return JNI_FALSE;
    }

    jfieldID out_samplerate_field = (*env)->GetFieldID(env, cls, "outSampleRate", "I");
    jfieldID out_bitrate_field = (*env)->GetFieldID(env, cls, "outBitRate", "I");
    jfieldID out_mono_field = (*env)->GetFieldID(env, cls, "outMono", "I");
    int out_samplerate = (*env)->GetIntField(env, thiz, out_samplerate_field);
    int out_bitrate = (*env)->GetIntField(env, thiz, out_bitrate_field);
    int out_mono = (*env)->GetIntField(env, thiz, out_mono_field);

    info("init lame_global_flags: %p", gfp);
    lame_set_num_channels(gfp, channels);
    lame_set_in_samplerate(gfp, sampleRate);
    lame_set_out_samplerate(gfp, out_samplerate);
    lame_set_brate(gfp, out_bitrate);
    lame_set_mode(gfp, out_mono == 1 ? 3 : 0 );
    lame_set_quality(gfp, 7); /* 2=high  5 = medium  7=low */ 

    info("out_sample_rate: %d", out_samplerate);
    info("out_bitrate: %d", out_bitrate);
    info("out_mono: %s", out_mono == 1 ? "yes" : "no");

    if(lame_init_params(gfp) <0)
    {
        info("init lame_global_flags failure.");
        lame_close(gfp);
        return JNI_FALSE;
    }

    info("lame_init_params is ok.");
    struct lame_ctx* ctx = malloc(sizeof(struct lame_ctx));
    memset(ctx, 0, sizeof(struct lame_ctx));
    ctx->gfp = gfp;
    ctx->bytes_per_sample = bytesInSample;
    ctx->channels = channels;

    (*env)->SetIntField(env, thiz, lame_pointer_field, (int) ctx);
    info("create lame_ctx: %p", ctx);
    
    return JNI_TRUE;
}

/*
 * Class:     jug_ndk_audio_encoder_AudioEncoder
 * Method:    encode
 * Signature: (I[B[B)I
 */
JNIEXPORT jint JNICALL Java_jug_ndk_audio_encoder_AudioEncoder_encode
  (JNIEnv *env, jobject thiz, jint inputSize, jbyteArray inBuffer, jbyteArray outBuffer)
{
    jclass cls = (*env)->GetObjectClass(env, thiz);
    jfieldID lame_pointer_field = (*env)->GetFieldID(env, cls, "nativePointerAddress", "I");
    int my_lame_addr = (*env)->GetIntField(env, thiz, lame_pointer_field);
    if(my_lame_addr == 0)
    {
        info("no lame-ctx found. try allocate first");
        return -1;
    }
    struct lame_ctx * ctx = (struct lame_ctx*) my_lame_addr;
    jint pcm_len = (*env)->GetArrayLength(env, inBuffer);
    jint out_mp3_len = (*env)->GetArrayLength(env, outBuffer);

    jbyte* pcm_buffer = (*env)->GetByteArrayElements(env, inBuffer, NULL);
    jbyte* out_mp3_buffer = (*env)->GetByteArrayElements(env, outBuffer, NULL);

    int len = lame_encode_buffer_interleaved(ctx->gfp, (short int *) pcm_buffer, 
            inputSize / (ctx->channels * ctx->bytes_per_sample) , out_mp3_buffer, out_mp3_len);

    (*env)->ReleaseByteArrayElements(env, inBuffer, pcm_buffer, 0);
    (*env)->ReleaseByteArrayElements(env, outBuffer, out_mp3_buffer, 0);

    return len;
}

/*
 * Class:     jug_ndk_audio_encoder_AudioEncoder
 * Method:    destroy
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jug_ndk_audio_encoder_AudioEncoder_destroy
  (JNIEnv *env, jobject thiz)
{
    jclass cls = (*env)->GetObjectClass(env, thiz);
    jfieldID lame_pointer_field = (*env)->GetFieldID(env, cls, "nativePointerAddress", "I");
    int my_lame_addr = (*env)->GetIntField(env, thiz, lame_pointer_field);
    if(my_lame_addr != 0)
    {
        struct lame_ctx* ctx = (struct lame_ctx*) my_lame_addr;
        info("close lame global flag: %p", ctx->gfp);
        lame_close(ctx->gfp);

        info("destroy lame-ctx: %p", ctx);
        free(ctx);
        (*env)->SetIntField(env, thiz, lame_pointer_field, 0);
    }
}

