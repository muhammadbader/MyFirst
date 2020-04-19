package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.impl.STOMPMessage;

import javax.swing.plaf.IconUIResource;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class STOMPEncoderDecoder implements MessageEncoderDecoder<STOMPMessage> {

    private byte[] bytes = new byte[1 << 10]; //start with 8k
    private int len = 0;

    @Override
    public STOMPMessage decodeNextByte(byte nextByte) {
        //notice that the top 128 ascii characters have the same representation as their utf-8 counterparts
        //this allow us to do the following comparison
        if (nextByte == '\u0000') {
            return popMessage();
        }
        pushByte(nextByte);
        return null; //not a full frame yet
    }

    @Override
    public byte[] encode(STOMPMessage message) {
        return message.toString().getBytes();
    }

    private void pushByte(byte nextByte) {
        if (len >= bytes.length) {
            bytes = Arrays.copyOf(bytes, len * 2);
        }
        bytes[len++] = nextByte;
    }

    private STOMPMessage popMessage() {
        String result = new String(bytes, 0, len, StandardCharsets.UTF_8);
        len = 0;

        String[] STOMP = result.split("\n");
        int i = 0;
        while(STOMP[i].isEmpty()){
            i++;
        }
        STOMPMessage message = new STOMPMessage(STOMP[i]);
        i++;
        for (; i < STOMP.length && !STOMP[i].isEmpty(); i++) {
            message.setHeader(STOMP[i]);
        }
        if (i < STOMP.length)
            message.setbody(STOMP[++i]);
        return message;
    }
}