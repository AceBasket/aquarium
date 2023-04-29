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

        // First thing first is greeting the server
        try {
            // view.talkToServer("Testing connection");
            sendQueue.offer(ServerThreadHandlers.doHello(logFile, view));
            logFile.println("Sent hello");
            logFile.flush();
            while (true) {
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
                        break;
                    case NOGREETING:
                        logFile.println("Server is full");
                        logFile.flush();
                        // BREAK CONNECTION SOMEHOW
                        break;
                    case LISTFISHES:
                        ServerThreadHandlers.listHandler(logFile, fishesList, receivedQueue.remove());
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
                        break;
                }

                boolean listFishesDestinations = false;

                for (Fish fish : fishesList.getFishes()) {
                    // if fish started but less than two destinations
                    if (fish.getSizeDestinations() != -1 && fish.getSizeDestinations() < 2) {
                        logFile.println("Fish " + fish.getName() + " needs an update on his destinations");
                        logFile.flush();
                        if (!listFishesDestinations) {
                            sendQueue.offer(ServerThreadHandlers.doLs(logFile)); // ask for list of fishes
                            listFishesDestinations = true;
                        }
                    }
                }

            }

        } catch (Exception e) {
            logFile.println(e);
            logFile.flush();
        }
    }
}
