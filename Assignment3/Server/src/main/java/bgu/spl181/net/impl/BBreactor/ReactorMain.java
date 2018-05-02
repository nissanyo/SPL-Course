package bgu.spl181.net.impl.BBreactor;

import bgu.spl181.net.srv.ClientsInfo;
import bgu.spl181.net.srv.LineMessageEncoderDecoder;
import bgu.spl181.net.srv.Server;
import bgu.spl181.net.srv.VideoServiceTxtProtocol;

import java.util.function.Supplier;

public class ReactorMain {
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

        Server.reactor( 7, Integer.parseInt(args[0]), protocolSup, encdecSup).serve();
    }
}
