package threads;

import java.io.IOException;
import java.io.PrintWriter;
import utils.Log.LogLevel;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.ConcurrentLinkedQueue;

import aquarium.*;
import utils.*;

public class ReadFromServerThread implements Runnable {
    private final ConcurrentLinkedQueue<ParserResult> receivedQueue;
    private final ConcurrentLinkedQueue<String> sendQueue;
    private PrintWriter logFile;
    private View view;
    private Timer timeoutTimer;
    private long id;

    public ReadFromServerThread(View view, ConcurrentLinkedQueue<ParserResult> receivedQueue,
            ConcurrentLinkedQueue<String> sendQueue, long id) {
        this.receivedQueue = receivedQueue;
        this.sendQueue = sendQueue;
        this.view = view;
        this.id = id;
        try {
            logFile = new PrintWriter("log_io_thread" + id + ".log");
        } catch (IOException e) {
            System.out.println("Error creating log file");
        }
        timeoutTimer = new Timer();
        startTimeoutTimer(view.getDisplayTimeoutValue(), id);
    }

    private void startTimeoutTimer(long delay, long id) {
        timeoutTimer.schedule(new TimeoutTask(view, logFile, id), delay * 1000);
    }

    private class TimeoutTask extends TimerTask {
        private View view;
        private PrintWriter logFile;
        private long id;

        public TimeoutTask(View view, PrintWriter logFile, long id) {
            this.view = view;
            this.logFile = logFile;
            this.id = id;
        }

        public void run() {
            Log.logMessage(logFile, LogLevel.INFO, "Timeout reached");
            view.talkToServer("ping " + id);
        }
    }

    public void run() {
        String response;
        ParserResult parsedResponse;
        while (true) {
            if (Thread.currentThread().isInterrupted()) {
                Log.logMessage(logFile, LogLevel.WARNING, "IO thread interrupted");
                timeoutTimer.cancel();
                Thread.currentThread().interrupt();
                return;
            }
            try {
                // while (sendQueue.isEmpty()) { // Just for right now, we have a logic problem
                // (don't know how many lines
                // // to read from server and when to send data to server)
                // logFile.println("Waiting for command");
                // logFile.flush();
                // Thread.sleep(1000);
                // }
                if (!sendQueue.isEmpty()) {
                    Log.logMessage(logFile, LogLevel.INFO, "Sending: " + sendQueue.peek());
                    view.talkToServer(sendQueue.remove());
                }

                if (view.serverIsTalking()) {
                    response = view.listenToServer();
                    Log.logMessage(logFile, LogLevel.INFO, "Server answered: " + response);
                    parsedResponse = Parser.parse(response);
                    if (parsedResponse.getFunction() == utils.Parser.PossibleResponses.PONG) {
                        startTimeoutTimer(view.getDisplayTimeoutValue(), id);
                        continue; // we don't want to add pong to the queue
                    }
                    receivedQueue.offer(parsedResponse);
                    // if (receivedQueue.peek().getFunction() != parsedResponse.getFunction()) {
                    // logFile.println("ERROR: parsed response wasn't added to queue. Last response
                    // was: "
                    // + receivedQueue.peek().getFunction() + " and this response is: "
                    // + parsedResponse.getFunction() + " and the response is: " + response);
                    // for (ParserResult result : receivedQueue) {
                    // logFile.println("Queue contains: " + result.getFunction());
                    // }
                    // logFile.flush();
                    // }
                }

            } catch (IOException e) {
                System.out.println(e.getMessage());
            } catch (ParserException e) {
                Log.logMessage(logFile, LogLevel.ERROR, e.getMessage());
            }
        }
    }
}
