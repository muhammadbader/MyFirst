package bgu.spl.net.impl.stomp;


import bgu.spl.net.srv.*;

public class StompServer {

    public static void main(String[] args) {

        Server.threadPerClient(
                Integer.parseInt(args[0]), //port
                () -> new StompMessagingProtocolImpl(), //protocol factory
                () -> new STOMPEncoderDecoder() //message encoder decoder factory
        ).serve();

//        Server.reactor(
//                Runtime.getRuntime().availableProcessors(),
//                Integer.parseInt(args[0]), //port
//                () -> new StompMessagingProtocolImpl(), //protocol factory,when we call the get method
//                () -> new STOMPEncoderDecoder() //message encoder decoder factory
//        ).serve();
    }

}
