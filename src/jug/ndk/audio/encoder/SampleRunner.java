package jug.ndk.audio.encoder;

import java.io.FileInputStream;
import java.io.FileOutputStream;

public class SampleRunner implements Runnable {

    @Override
    public void run() {

        AudioEncoder audioEncoder = new AudioEncoder();
        audioEncoder.allocate(2, 2, 44100);
        try {
            byte[] readBuffer = new byte[4096];
            Mp3OutputStream fout = new Mp3OutputStream(new FileOutputStream("/sdcard/jug.mp3"),
                    audioEncoder, readBuffer.length);
            FileInputStream fin = new FileInputStream("/sdcard/sample.wav");
            int read = 0;
            fin.read(readBuffer, 0, 44 /* skip wav header */);
            int count = 120;
            while ((read = fin.read(readBuffer)) != -1 && count-- > 0) {
                fout.write(readBuffer, 0, read);
            }
            fin.close();
            fout.close();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            audioEncoder.destroy();
        }

    }

}
