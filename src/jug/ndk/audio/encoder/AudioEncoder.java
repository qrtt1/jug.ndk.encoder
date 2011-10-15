package jug.ndk.audio.encoder;
public class AudioEncoder {

    static {
        System.loadLibrary("mp3lame");
        System.loadLibrary("audio_encoder");
    }
    
    /* don't rename it nativePointerAddress*/
    protected int nativePointerAddress = 0;
    
    /* configuration used when lame allocated*/
    protected int outSampleRate = 44100;
    protected int outBitRate = 64; /* n kbps*/
    protected int outMono = 0; /* 1 is mono, not 1 is stereo */

    public native boolean allocate(int channels, int bytesInSample, int sampleRate);

    public native int encode(int inputSize, byte[] pcm, byte[] mp3buffer);

    public native void destroy();
}
