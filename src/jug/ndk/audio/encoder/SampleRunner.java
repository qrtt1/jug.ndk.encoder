package jug.ndk.audio.encoder;

public class SampleRunner implements Runnable {

    @Override
    public void run() {
        AudioEncoder audioEncoder = new AudioEncoder();
        audioEncoder.allocate(2, 2, 44100);
        audioEncoder.encode(0, new byte[0], new byte[0]);
        audioEncoder.destroy();
    }

}
