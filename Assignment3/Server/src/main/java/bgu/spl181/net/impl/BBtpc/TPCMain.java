package bgu.spl181.net.impl.BBtpc;

import bgu.spl181.net.srv.*;

import java.util.function.Supplier;

public class TPCMain{
    public static void main(String[] args) {

        ClientsInfo clients = new ClientsInfo();

        Supplier protocolSup = new Supplier() {
            @Override
            public Object get() {
                return new VideoServiceTxtProtocol(clients);
            }
        };

        Supplier encdecSup = new Supplier() {
            @Override
            public Object get() {
                return new LineMessageEncoderDecoder();
            }
        };

        Server.threadPerClient(Integer.parseInt(args[0]), protocolSup, encdecSup).serve();
    }
}
