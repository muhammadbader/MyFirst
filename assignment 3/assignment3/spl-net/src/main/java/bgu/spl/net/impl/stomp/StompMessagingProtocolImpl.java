package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.*;
import bgu.spl.net.srv.Connections;

public class StompMessagingProtocolImpl implements StompMessagingProtocol<STOMPMessage> {
    private boolean terminate=false;
    private int ownerID;
    private ConnectionsImpl connections;

    @Override
    public void start(int connectionId, Connections<STOMPMessage> connections) {
        this.ownerID=connectionId;
        this.connections=(ConnectionsImpl)connections;
    }

    @Override
    public void process(STOMPMessage message) {
        STOMPMessage error=message.checkMessage(message);
        if(error!=null){//in case we got a non-complete message
            connections.send(ownerID, error);
            try {
                Thread.currentThread().sleep(50);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            connections.disconnect(ownerID);
            return;
        }
        String frame = message.getFrame();
        String[] headers = message.parseHeaders();
        if (frame.equals("CONNECT")) {
            terminate = !login(headers[0], headers[2], headers[3]);// check successful login in
        } else if (frame.contains("DISCONNECT")) {
            terminate = true;
            STOMPMessage m = new STOMPMessage("RECEIPT");
            m.setHeader("receipt-id:" + headers[0]);
            connections.send(ownerID, m);
            try {
                Thread.currentThread().sleep(50);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            connections.disconnect(ownerID);
        } else if (frame.equals("SUBSCRIBE")) {
            connections.joinTopic(headers[0], ownerID, headers[1]);
            STOMPMessage m = new STOMPMessage("RECEIPT");
            m.setHeader("receipt-id:" + headers[2]);
            connections.send(ownerID, m);
        } else if (frame.equals("UNSUBSCRIBE")) {
            String subscriptionID = headers[0];//it is the subscription id
            connections.exitTopic(subscriptionID, ownerID);
            STOMPMessage m = new STOMPMessage("RECEIPT");
            m.setHeader("receipt-id:" + headers[0]);
            connections.send(ownerID, m);
        } else if (frame.equals("SEND")) {
            String body = message.getBody();
            STOMPMessage m = new STOMPMessage("MESSAGE");
            String subscription = connections.getSubscriptionID(ownerID, headers[0]);
            if (subscription != null) {//todo deal with this user who didn't subscribe for this topic
                if (body.contains("has added the book")) {
                    //todo client side: check for double book
                    m.setbody(body);
                    m.setHeader("subscription:" + subscription);
                    m.setHeader("Message-id:" + m.getidMessage());
                    m.setHeader("destination:" + headers[0]);
                    connections.send(headers[0], m);
                } else if (body.contains("Returning")) {
                    m.setHeader("subscription:" + subscription);
                    m.setHeader("Message-id:" + m.getidMessage());
                    m.setHeader("destination:" + headers[0]);
                    m.setbody(body);
                    connections.send(headers[0], m);
                } else if (body.contains("ook status")) {

                    m.setHeader("subscription:" + subscription);
                    m.setHeader("Message-id:" + m.getidMessage());
                    m.setHeader("destination:" + headers[0]);
                    m.setbody("Book status");
                    connections.send(headers[0], m);
                } else if (body.contains(":")) {
                    m.setHeader("subscription:" + subscription);
                    m.setHeader("Message-id:" + m.getidMessage());
                    m.setHeader("destination:" + headers[0]);
                    m.setbody(body);
                    connections.send(headers[0], m);
                } else if (body.contains("wish to borrow")) {
                    m.setHeader("subscription:" + subscription);
                    m.setHeader("Message-id:" + m.getidMessage());
                    m.setHeader("destination:" + headers[0]);
                    m.setbody(body);
                    connections.send(headers[0], m);
                } else if (body.contains("has")) {
                    m.setHeader("subscription:" + subscription);
                    m.setHeader("Message-id:" + m.getidMessage());
                    m.setHeader("destination:" + headers[0]);
                    m.setbody(body);
                    connections.send(headers[0], m);
                } else if (body.contains("Taking")) {
                    m.setHeader("subscription:" + subscription);
                    m.setHeader("Message-id:" + m.getidMessage());
                    m.setHeader("destination:" + headers[0]);
                    m.setbody(body);
                    connections.send(headers[0], m);
                }
            }
        }
    }

    private boolean login(String version,String user,String pass){
        String ans=connections.login(user,pass);
        if(ans.equals("new user")){
            Readers newreader=new Readers(user,pass);
            //todo: add the id to the message
            if(connections.addreader(newreader)){
                connections.connectReader(ownerID,newreader);
                //send  Login successful
                STOMPMessage m=new STOMPMessage("CONNECTED");
                m.setHeader("version:"+version);
                connections.send(ownerID, m);
                return true;
            }
            return login(version,user,pass);
        }else if(ans.equals("tamam")){
            if(connections.connectReader(ownerID,connections.getReader(user))) {
                //send  Login successful
                STOMPMessage m = new STOMPMessage("CONNECTED");
                m.setHeader("version:"+version);
                connections.send(ownerID, m);
                return true;
            }
            return login(version,user,pass);
        }else if(ans.equals("wrong password")){
            STOMPMessage m=new STOMPMessage("ERROR");
            m.setHeader("version:"+version);
            m.setbody("Wrong password");
            connections.send(ownerID, m);
            try {
                Thread.currentThread().sleep(50);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            connections.disconnect(ownerID);
            return false;
        }else{
            STOMPMessage m=new STOMPMessage("ERROR");
            m.setHeader("version:"+version);
            m.setbody("User already logged in");
            connections.send(ownerID, m);
            try {
                Thread.currentThread().sleep(50);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            connections.disconnect(ownerID);
            return false;
        }
    }
    @Override
    public boolean shouldTerminate() {
        return terminate;
    }
}