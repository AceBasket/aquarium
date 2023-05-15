package threads;

import aquarium.*;
import utils.*;
import java.io.*;
import java.util.concurrent.ConcurrentLinkedQueue;

public class ServerThread implements Runnable {
    private View view;
    private Aquarium fishesList;
    private PrintWriter logFile;
    private final ConcurrentLinkedQueue<ParserResult> receivedQueue;
    private final ConcurrentLinkedQueue<String> sendQueue;

    public ServerThread(View view, Aquarium aquarium, ConcurrentLinkedQueue<ParserResult> receivedQueue,
            ConcurrentLinkedQueue<String> sendQueue) {
        this.view = view;
        this.fishesList = aquarium;
        this.receivedQueue = receivedQueue;
        this.sendQueue = sendQueue;
        try {
            logFile = new PrintWriter("log_server_thread.log");
        } catch (IOException e) {
            System.out.println("Error creating log file");
        }
    }

    public void run() {
        // TODO
        logFile.println("Starting server thread");
        logFile.flush();

        ParserResult response;
        boolean listFishesDestinations = false;
        // boolean responseReceived = false;

        // First thing first is greeting the server
        try {
            // view.talkToServer("Testing connection");
            sendQueue.offer(ServerThreadHandlers.doHello(logFile, view));
            logFile.println("Sent hello");
            logFile.flush();
            while (true) {
                if (Thread.currentThread().isInterrupted()) {
                    logFile.println("Server thread interrupted");
                    logFile.flush();
                    Thread.currentThread().interrupt();
                    return;
                }
                // listFishesDestinations = false;
                for (Fish fish : fishesList.getFishes()) {
                    // if fish started but less than two destinations
                    fish.removeExpiredDestinations();
                    if (fish.getSizeDestinations() != -1 && fish.getSizeDestinations() < 1) {
                        logFile.println("Fish " + fish.getName() + " needs an update on his destinations");
                        logFile.flush();
                        if (!listFishesDestinations) {
                            // ask for list of fishes
                            // sendQueue.offer(ServerThreadHandlers.doLs(logFile));
                            // logFile.println("Sent ls");
                            // logFile.flush();
                            listFishesDestinations = true;
                        }
                    }
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
                        ServerThreadHandlers.greetingHandler(logFile, view, receivedQueue.remove());
                        sendQueue.offer("getFishesContinuously");
                        logFile.println("Sent getFishesContinuously");
                        logFile.flush();
                        break;
                    case NOGREETING:
                        logFile.println("Server is full");
                        logFile.flush();
                        // BREAK CONNECTION SOMEHOW
                        break;
                    case LISTFISHES:
                        ServerThreadHandlers.listHandler(logFile, fishesList, receivedQueue.remove());
                        listFishesDestinations = false;
                        break;
                    case BYE:
                        logFile.println("Logging out");
                        logFile.flush();
                        // LOG OUT SOMEHOW
                        break;
                    case PONG:
                        logFile.println("Pong received");
                        logFile.flush();
                        // reset timeout timer somehow
                        break;

                    default:
                        logFile.println("Not a command handled by server thread");
                        logFile.flush();
                        Thread.sleep(500); // sleep 1 second and try again
                        break;
                }

                // for (Fish fish : fishesList.getFishes()) {
                // // if fish started but less than two destinations
                // if (fish.getSizeDestinations() != -1 && fish.getSizeDestinations() < 2) {
                // logFile.println(
                // "[second for loop] Fish " + fish.getName() + " needs an update on his
                // destinations");
                // logFile.flush();
                // if (!listFishesDestinations) {
                // sendQueue.offer(ServerThreadHandlers.doLs(logFile)); // ask for list of
                // fishes
                // logFile.println("Sent ls");
                // logFile.flush();
                // listFishesDestinations = true;
                // }
                // }
                // }
                Thread.sleep(200); // 200ms = 0.2s

            }

        } catch (Exception e) {
            logFile.println("ERROR: " + e.getMessage());
            logFile.flush();
        }
    }
}
