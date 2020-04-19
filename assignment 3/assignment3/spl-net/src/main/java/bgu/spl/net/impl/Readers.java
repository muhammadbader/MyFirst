package bgu.spl.net.impl;

import java.util.HashMap;
import java.util.LinkedList;

public class Readers {

    private final String username;
    private final String password;
    private boolean connected;
    private HashMap<String,String> topics=new HashMap<>();

    public Readers(String name,String pass) {
        this.username = name;
        this.password = pass;
        this.connected = false;

    }
    public boolean isConnect(){
        return connected;
    }
    public void disconnect(){
        topics=new HashMap<>();
        connected=false;
    }
    public LinkedList<String> gettopics() {
        LinkedList<String> alltopics=new LinkedList<>();
        for(String s:topics.keySet())
            alltopics.add(s);
        return alltopics;
    }
    public String getTopic(String subscription){
        for(String s:topics.keySet()){
            if(topics.get(s).equals(subscription)){
                return s;
            }
        }
        return null;
    }
    public String getSubscriptionID(String topic){
        return topics.get(topic);
    }

    public void connect() {
        connected=true;
    }
    public String getName(){
        return username;
    }
    public String getPass(){
        return password;
    }
    public void joinGenre(String topic,String subsription){
        topics.put(topic,subsription);
    }
    public void removeShelf(String topic){
        topics.remove(topic);
    }
}