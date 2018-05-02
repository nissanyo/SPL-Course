package bgu.spl181.net.srv;

import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class LineMessageEncoderDecoder implements bgu.spl181.net.api.MessageEncoderDecoder {

    private byte[] bytes = new byte[1 << 10];
    private int len = 0;


    /**
     * add the next byte to the decoding process
     *
     * @param nextByte the next byte to consider for the currently decoded
     *                 message
     * @return a message if this byte completes one or null if it doesnt.
     */
    @Override
    public Object decodeNextByte(byte nextByte) {
        if (nextByte == '\n') {
            return popString();
        }

        pushByte(nextByte);
        return null;
    }

    /**
     * encodes the given message to bytes array
     *
     * @param message the message to encode
     * @return the encoded bytes
     */
    @Override
    public byte[] encode(Object message) {
        return (message + "\n").getBytes();
    }

    private void pushByte(byte nextByte) {
        if (len >= bytes.length) {
            bytes = Arrays.copyOf(bytes, len * 2);
        }

        bytes[len++] = nextByte;
    }

    private String popString() {
        String result = new String(bytes, 0, len, StandardCharsets.UTF_8);
        len = 0;
        return result;
    }
}

