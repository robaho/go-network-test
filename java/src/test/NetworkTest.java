package test;

import org.openjdk.jmh.annotations.*;

import java.io.IOException;
import java.net.*;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;
import java.util.concurrent.TimeUnit;

@State(Scope.Benchmark)
@Fork(0)
@Warmup(iterations = 1)
@Measurement(iterations = 10, time = 1)
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.NANOSECONDS)

public class NetworkTest {

    static InetSocketAddress server = new InetSocketAddress("localhost",8500);
    static InetSocketAddress client = new InetSocketAddress("localhost",8501);

    static Thread serverThread;
    static ByteBuffer clientBuffer = ByteBuffer.allocateDirect(1024);
    static ByteBuffer serverBuffer = ByteBuffer.allocateDirect(1024);

    static DatagramChannel serverSocket,clientSocket;

    @Setup
    public void setup() throws IOException {
        serverSocket= DatagramChannel.open();
        clientSocket=DatagramChannel.open();

        serverSocket.bind(server);
        clientSocket.bind(client);

        serverThread = new Thread(() -> {
            try {
                while (true) {
                    serverSocket.receive(serverBuffer);
                    serverSocket.send(serverBuffer, client);
                }
            } catch(Exception e){
            }
        });
        serverThread.start();
    }

    @TearDown
    public void tearDown() throws InterruptedException {
        try { clientSocket.close(); } catch(Exception e){}
        try {serverSocket.close(); } catch(Exception e){}
        serverThread.join();
    }

    @Benchmark
    public void TestPingPong() throws IOException {
        clientSocket.send(clientBuffer,server);
        clientSocket.receive(clientBuffer);
    }
}
