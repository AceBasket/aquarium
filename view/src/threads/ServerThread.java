package threads;

import aquarium.*;
import utils.*;
import utils.Log.LogLevel;
import java.io.*;
import java.util.concurrent.ConcurrentLinkedQueue;

public class ServerThread implements Runnable {
    private Client client;
    private Aquarium fishesList = Aquarium.getInstance();
    private PrintWriter logFile;
    private final ConcurrentLinkedQueue<ParserResult> receivedQueue;
    private final ConcurrentLinkedQueue<String> sendQueue;

    public ServerThread(Client client, Aquarium aquarium, ConcurrentLinkedQueue<ParserResult> receivedQueue,
            ConcurrentLinkedQueue<String> sendQueue, long id) {
        this.client = client;
        this.receivedQueue = receivedQueue;
        this.sendQueue = sendQueue;
        try {
            logFile = new PrintWriter("log_server_thread" + id + ".log");
        } catch (IOException e) {
            System.out.println("Error creating log file");
        }
    }

    public void run() {
        // TODO
        Log.logMessage(logFile, LogLevel.INFO, "Starting server thread");

        ParserResult response;
        // boolean responseReceived = false;

        // First thing first is greeting the server
        try {
            // view.talkToServer("Testing connection");
            sendQueue.offer(ServerThreadHandlers.doHello(logFile, client));
            Log.logMessage(logFile, LogLevel.INFO, "Sent hello");
            while (true) {
                if (Thread.currentThread().isInterrupted()) {
                    Log.logMessage(logFile, LogLevel.WARNING, "Server thread interrupted");
                    Thread.currentThread().interrupt();
                    return;
                }
                response = receivedQueue.peek();
                if (response == null) {
                    Thread.sleep(1000); // sleep 1 second and try again
                    Log.logMessage(logFile, LogLevel.INFO, "Nothing to handle");
                    continue;
                }

                switch (response.getFunction()) {
                    case GREETING:
                        ServerThreadHandlers.greetingHandler(logFile, client, receivedQueue.remove());
                        sendQueue.offer("getFishesContinuously");
                        Log.logMessage(logFile, LogLevel.INFO, "Sent getFishesContinuously");
                        break;
                    case NO_GREETING:
                        Log.logMessage(logFile, LogLevel.WARNING, "Server is full");
                        // BREAK CONNECTION SOMEHOW
                        break;
                    case LIST_FISHES:
                        ServerThreadHandlers.listHandler(logFile, fishesList, receivedQueue.remove());
                        // listFishesDestinations = false;
                        break;
                    case BYE:
                        receivedQueue.remove();
                        ServerThreadHandlers.byeHandler(logFile);
                        return; // end thread

                    default:
                        Log.logMessage(logFile, LogLevel.WARNING, response.getFunction() + ": Not a command handled by server thread");
                        Thread.sleep(300); // sleep 0.3 second and try again
                        break;
                }

            }

        } catch (InterruptedException e) {
            Log.logMessage(logFile, LogLevel.FATAL_ERROR, "Server thread interrupted while sleeping");
            Thread.currentThread().interrupt();
        } catch (Exception e) {
            Log.logMessage(logFile, LogLevel.ERROR, e.getMessage());
        }
    }
}
