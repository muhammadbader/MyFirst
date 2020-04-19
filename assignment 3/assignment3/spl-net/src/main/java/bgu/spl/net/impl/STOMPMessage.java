package bgu.spl.net.impl;

import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

public class STOMPMessage {
    protected final String frame;
    protected List<String> headers = new LinkedList<>();
    protected String body = "";
    private static AtomicInteger id = new AtomicInteger(0);
    private static AtomicInteger errorid = new AtomicInteger(0);

    public STOMPMessage(String frame) {
        this.frame = frame;
    }

    public String getFrame() {
        return frame;
    }

    public String[] parseHeaders() {
        String[] parse = new String[headers.size()];
        int i = 0;

        for (String s : headers){
            parse[i] = s.split(":")[1];
            i++;
        }

        return parse;
    }

    public String[] parse() {
        String[] parse = new String[headers.size()];
        int i = 0;
        for (String s : headers){

            parse[i++] = s.split(":")[0];
        }

        return parse;
    }

    public String getHeaders() {
        String headers = "";
        for (String h : this.headers) {
            headers = headers + h + "\n";
        }
        return headers + "\n";
    }

    public String getBody() {
        if (!body.isEmpty()) {
            return body+'\n' ;
        }
        return "";
    }

    public String toString() {
        return getFrame()+'\n' + getHeaders() + getBody()+ "\u0000";
    }

    public void setbody(String body) {
        this.body = this.body + body;
    }

    public void setHeader(String header) {
        if (!header.isEmpty())
            headers.add(header);
    }

    public static String getidMessage() {
        Integer x = id.incrementAndGet();
        String i = (x).toString();
        x = i.length();
        for (int j = 0; j < 5 - x; j++)
            i = "0" + i;
        return i;
    }

    public static String getErroridMessage() {
        Integer x = errorid.incrementAndGet();
        String i = (x).toString();
        x = i.length();
        for (int j = 0; j < 5 - x; j++)
            i = "0" + i;
        return i;
    }

    public STOMPMessage checkMessage(STOMPMessage message) {

        STOMPMessage error = new STOMPMessage("ERROR");

        String[] headers = message.parse();
        if (message.getFrame().equals("CONNECT")) {
            if (headers.length > 4) {
                error.setbody("too much information");
            } else if (headers.length < 4) {
                boolean host = false, login = false, pass = false;
                for (String s : headers) {
                    if (s.contains("passcode")) pass = true;
                    if (s.contains("login")) login = true;
                    if (s.contains("host")) host = true;
                }
                if (!host)
                    error.setbody("Did not contain a host which is REQUIRED for message propagation to logging in \n");
                if (!login)
                    error.setbody("Did not contain a login name which is REQUIRED for message propagation to logging in \n");
                if (!pass)
                    error.setbody("Did not contain a passcode which is REQUIRED for message propagation to logging in");
            } else {
                return null;
            }
            error.setHeader("receipt-id: "+getErroridMessage());
            error.setHeader("message: malformed frame received");
            return error;
        } else if (message.getFrame().equals("SUBSCRIBE")) {
            if (headers.length > 3) {
                error.setbody("too much information");
            } else if (headers.length < 3) {
                boolean dest = false, id = false, receipt = false;
                for (String s : headers) {
                    if (s.contains("destination")) dest = true;
                    if (s.contains("id")) id = true;
                    if (s.contains("receipt")) receipt = true;
                }
                if (!dest) error.setbody("Did not contain a destination which is REQUIRED joining in\n");
                if (!id)
                    error.setbody("Did not contain a subscription id which is REQUIRED for message propagation joining in\n");
                if (!receipt)
                    error.setbody("Did not contain a receipt id which is REQUIRED for message propagation joining in");
            } else {
                return null;
            }
            error.setHeader("receipt-id: "+getErroridMessage());
            error.setHeader("message: malformed frame received");
            return error;
        } else if (message.getFrame().contains("SEND")) {
            if (headers.length > 1) {
                error.setbody("too much information");
            } else if (headers.length == 0) {
                error.setbody("Did not contain destination which is REQUIRED for sending the message\n");
            }
            if (message.getBody().isEmpty()) {
                error.setbody("Did not contain a body which is REQUIRED for sending the message");
            } else if(message.getBody().contains("ook status")) {
                boolean s=false;
                for(String tmp:this.headers){
                    if(tmp.split(":").length<2){
                        error.setbody("Did not contain a destination which is REQUIRED for sending the message");
                        s=true;
                    }
                    if(!s)
                        return null;
                }
            }else{
                return null;
            }
            error.setHeader("receipt-id: "+getErroridMessage());
            error.setHeader("message: malformed frame received");
            return error;
        } else if (message.getFrame().contains("DISCONNECT")) {
            if (headers.length > 1) {
                error.setbody("too much information");
            } else if (headers.length == 0) {
                error.setbody("Did not contain a destination which is REQUIRED for message propagation");
            } else {
                return null;
            }
            error.setHeader("receipt-id: "+getErroridMessage());
            error.setHeader("message: malformed frame received");
            return error;
        } else if (message.getFrame().equals("UNSUBSCRIBE")) {
            if (headers.length > 1) {
                error.setbody("too much information");
            } else if (headers.length == 0) {
                error.setbody("Did not contain an id which is REQUIRED for message propagation");
            } else {
                return null;
            }
            error.setHeader("receipt-id: "+getErroridMessage());
            error.setHeader("message: malformed frame received");
            return error;
        } else {
            error.setbody("UNKNOWN message frame");
            error.setHeader("receipt-id: "+getErroridMessage());
            error.setHeader("message: malformed frame received");
            return error;
        }
    }
}