package bgu.spl.net.impl.echo;

import java.io.*;
import java.net.*;
import java.util.HashMap;

public class LPClient {

    public static void main(String[] args) throws IOException {
        // Command line: java LPClient localhost 4500 (Assuming default UTF-8)

        Socket lpSocket = null;
        PrintWriter writerOut = null;  //Prints formatted representations of objects to a text-output stream
        BufferedReader bufferIn = null;

//        System.out.println("here");
        String host = "localhost";     // Get the host from the command line
        int port = 4444;               // Get the port from the command line
        System.out.println("Connecting to " + host + ":" + port);

        try {
            lpSocket = new Socket(host, port);            // Connect to a socket with the host and port
            writerOut = new PrintWriter(lpSocket.getOutputStream(), true); // Initialize an output stream
            System.out.println("Connected to server!");   // Write messages for the server
//            bufferIn = new BufferedReader(new InputStreamReader(System.in)); // Create a console reader
//            String msg;

            String e = new String("CONNECT" + '\n' + "accept-version:1.2" + '\n' + "host:123.232.111.0" +'\n' + "login:bob"+'\n' +"passcode:ayamah"+'\n' + '\n' + '\u0000');
//            bufferIn = new BufferedReader(new InputStreamReader(System.in));
//            System.out.println("first");
            String ee = new String("SUBSCRIBE" + '\n' + "destination:HI" + '\n' + "id:12" + '\n' + '\n' + "HI wish to borrow Dune" + '\n' + '\u0000');
//            bufferIn = new BufferedReader(new InputStreamReader(System.in));
//            System.out.println("second");
            String eee = new String("DISCONNECT" + '\n' + "receipt:78" + '\n' + '\n' + "FRAME" + '\n' + '\u0000');

//            System.out.println("third");
            String eeee = new String("CONNECT" + '\n' + "login:HI" + '\n' + "passcode:Heehe" + '\n' + '\n' +  "FRAME" + '\n' + '\u0000');


                writerOut.print(e);
                writerOut.flush();
//                writerOut.print(ee);
//                writerOut.flush();
//                writerOut.print(eee);
//                writerOut.flush();
//                writerOut.print(eeee);
//                writerOut.flush();


        } catch (Exception e) {                          // Expecting UnknownHostException or IOException
            System.out.println("Connection problems with " + host + "\n" + e);
            System.exit(1);
        } finally {
            //if (lpSocket != null) lpSocket.close();
            //if (writerOut != null) writerOut.close();
            //if (bufferIn != null) bufferIn.close();
            System.out.println("Exiting...");
        }
    }

    class Frame {
        private String comand;
        private HashMap<String, String> headers = new HashMap<>();
        private String frame;

        public Frame(String message) {
            if (message.charAt(0) == '\n') {
                message = message.substring(1, message.length() - 1);
            }

            System.out.println(message);
            String[] str = message.split("\n");
            comand = str[0];
            int i = 1;
            for (; i < str.length; i++) {
                if (!str[i].equals("")) {
                    String a1 = str[i];
                    String a2 = str[i];
                    String b1 = str[i];
                    String b2 = str[i];
                    a1 = a2.substring(0, a1.indexOf(':'));
                    b1 = b2.substring(b1.indexOf(':') + 1);
                    headers.put(a1, b1);
                } else if (str[i].equals("") & str[i + 1].equals("")) {
                    if (!str[i + 2].equals('\u0000')) ;
                } else i = str.length;

            }


        }

        public String getComand() {
            return comand;
        }

        public void setFrame(String f) {
            this.frame = f;
        }

        public String getHeaderValue(String key) {
            return headers.get(key);
        }

        public void setHeaderValue(String key, String value) {
            headers.put(key, value);
        }

        public String getFrame() {
            return frame;
        }
    }

}

