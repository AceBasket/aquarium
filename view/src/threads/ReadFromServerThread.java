package threads;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.concurrent.ConcurrentLinkedQueue;

import aquarium.*;
import utils.*;

public class ReadFromServerThread implements Runnable {
    private final ConcurrentLinkedQueue<ParserResult> receivedQueue;
    private final ConcurrentLinkedQueue<String> sendQueue;
    private PrintWriter logFile;
    private View view;

    public ReadFromServerThread(View view, ConcurrentLinkedQueue<ParserResult> receivedQueue,
            ConcurrentLinkedQueue<String> sendQueue) {
        this.receivedQueue = receivedQueue;
        this.sendQueue = sendQueue;
        this.view = view;
        try {
            logFile = new PrintWriter("log_io_thread.log");
        } catch (IOException e) {
            System.out.println("Error creating log file");
        }
    }

    public void run() {
        String response;
        ParserResult parsedResponse;
        while (true) {
            try {
                // while (sendQueue.isEmpty()) { // Just for right now, we have a logic problem
                // (don't know how many lines
                // // to read from server and when to send data to server)
                // logFile.println("Waiting for command");
                // logFile.flush();
                // Thread.sleep(1000);
                // }
                if (!sendQueue.isEmpty()) {
                    logFile.println("Sending: " + sendQueue.peek());
                    logFile.flush();
                    view.talkToServer(sendQueue.remove());
                }

                if (view.serverIsTalking()) {
                    response = view.listenToServer();
                    logFile.println("Server answered: " + response);
                    logFile.flush();
                    parsedResponse = Parser.parse(response);
                    receivedQueue.offer(parsedResponse);
                    if (receivedQueue.peek().getFunction() != parsedResponse.getFunction()) {
                        logFile.println("ERROR: parsed response wasn't added to queue");
                        logFile.flush();
                    }
                }

            } catch (IOException e) {
                // TODO: handle exception
                System.out.println(e.getMessage());
            } catch (ParserException e) {
                logFile.println("ERROR: " + e.getMessage());
                logFile.flush();
            }
        }
    }
}
