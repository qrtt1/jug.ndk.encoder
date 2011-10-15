package jug.ndk.audio.encoder;

import java.io.FilterOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.logging.Logger;

public class Mp3OutputStream extends FilterOutputStream {

    private Logger logger = Logger.getLogger(Mp3OutputStream.class.getSimpleName());
    private AudioEncoder encoder;
    private byte[] outBuffer;

    public Mp3OutputStream(OutputStream out, AudioEncoder encoder, int outBufferSize) {
        super(out);
        this.encoder = encoder;
        outBuffer = new byte[outBufferSize];
    }
    
    @Override
    public void write(byte[] buffer, int offset, int length) throws IOException {
        int outSize = encoder.encode(length, buffer, outBuffer);
        logger.info("convert pcm data " + length + " bytes to mp3 in " + outSize + " bytes");
        out.write(outBuffer, 0, outSize);
    }
    
    @Override
    public void write(byte[] buffer) throws IOException {
        write(buffer, 0, buffer.length);
    }
    
    @Override
    public void write(int oneByte) throws IOException {
        write(new byte[] { (byte) oneByte });
    }

}
