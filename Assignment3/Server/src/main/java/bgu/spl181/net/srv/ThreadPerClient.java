package bgu.spl181.net.srv;

import java.util.function.Supplier;

public class ThreadPerClient extends BaseServer {


    public ThreadPerClient(int port, Supplier protocolFactory, Supplier encdecFactory) {
        super(port, protocolFactory, encdecFactory);
    }

    @Override
    protected void execute(BlockingConnectionHandler handler) {
        Thread thread = new Thread(handler);
        thread.start();
    }

}
