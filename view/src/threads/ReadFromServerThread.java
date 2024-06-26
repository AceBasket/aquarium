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
    private Client client;
    private Timer timeoutTimer;
    private long id;

    public ReadFromServerThread(Client client, ConcurrentLinkedQueue<ParserResult> receivedQueue,
            ConcurrentLinkedQueue<String> sendQueue, long id) {
        this.receivedQueue = receivedQueue;
        this.sendQueue = sendQueue;
        this.client = client;
        this.id = id;
        try {
            logFile = new PrintWriter("log_io_thread" + id + ".log");
        } catch (IOException e) {
            System.out.println("Error creating log file");
        }
        timeoutTimer = new Timer();
        startTimeoutTimer(client.getDisplayTimeoutValue(), id);
    }

    private void startTimeoutTimer(long delay, long id) {
        timeoutTimer.schedule(new TimeoutTask(client, logFile, id), delay * 1000);
    }

    private class TimeoutTask extends TimerTask {
        private Client client;
        private PrintWriter logFile;
        private long id;

        public TimeoutTask(Client client, PrintWriter logFile, long id) {
            this.client = client;
            this.logFile = logFile;
            this.id = id;
        }

        public void run() {
            Log.logMessage(logFile, LogLevel.INFO, "Timeout reached");
            client.talkToServer("ping " + id);
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
                if (!sendQueue.isEmpty()) {
                    Log.logMessage(logFile, LogLevel.INFO, "Sending: " + sendQueue.peek());
                    client.talkToServer(sendQueue.remove());
                }

                if (client.serverIsTalking()) {
                    response = client.listenToServer();
                    Log.logMessage(logFile, LogLevel.INFO, "Server answered: " + response);
                    parsedResponse = Parser.parse(response);
                    if (parsedResponse.getFunction() == utils.Parser.PossibleResponses.PONG) {
                        startTimeoutTimer(client.getDisplayTimeoutValue(), id);
                        continue; // we don't want to add pong to the queue
                    }
                    receivedQueue.offer(parsedResponse);
                }

            } catch (IOException e) {
                Log.logMessage(logFile, LogLevel.ERROR, e.getMessage() + " --> " + e.getCause());
            } catch (ParserException e) {
                Log.logMessage(logFile, LogLevel.ERROR, e.getMessage());
            }
        }
    }
}
