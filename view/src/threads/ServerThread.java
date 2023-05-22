package threads;

import aquarium.*;
import utils.*;
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
        logFile.println("Starting server thread");
        logFile.flush();

        ParserResult response;
        // boolean responseReceived = false;

        // First thing first is greeting the server
        try {
            // view.talkToServer("Testing connection");
            sendQueue.offer(ServerThreadHandlers.doHello(logFile, client));
            logFile.println("Sent hello");
            logFile.flush();
            while (true) {
                if (Thread.currentThread().isInterrupted()) {
                    logFile.println("Server thread interrupted");
                    logFile.flush();
                    Thread.currentThread().interrupt();
                    return;
                }

                response = receivedQueue.peek();
                if (response == null) {
                    Thread.sleep(1000); // sleep 1 second and try again
                    logFile.println("Nothing to handle");
                    logFile.flush();
                    continue;
                }

                switch (response.getFunction()) {
                    case GREETING:
                        ServerThreadHandlers.greetingHandler(logFile, client, receivedQueue.remove());
                        sendQueue.offer("getFishesContinuously");
                        logFile.println("Sent getFishesContinuously");
                        logFile.flush();
                        break;
                    case NO_GREETING:
                        logFile.println("Server is full");
                        logFile.flush();
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
                        logFile.println(response.getFunction() + ": Not a command handled by server thread");
                        logFile.flush();
                        Thread.sleep(300); // sleep 0.3 second and try again
                        break;
                }

            }

        } catch (InterruptedException e) {
            logFile.println("Server thread interrupted while sleeping");
            logFile.flush();
            Thread.currentThread().interrupt();
        } catch (Exception e) {
            logFile.println("ERROR: " + e.getMessage());
            logFile.flush();
        }
    }
}
