
// import utils.*;
import aquarium.*;
import java.io.*;
import java.time.Instant;
import java.util.concurrent.ConcurrentLinkedQueue;

import threads.*;
import utils.ParserException;
import utils.ParserResult;

public class Main {
    PrintWriter logFile;

    public Main(long id) {
        try {
            logFile = new PrintWriter("log_main" + id + ".log");
        } catch (IOException e) {
            System.out.println("Error creating log file for main thread");
        }
    }

    public static void main(String[] argv) {
        long id = ProcessHandle.current().pid();
        System.out.println("Process id: " + id);
        Main main = new Main(id);
        try {
            // main.logFile.println("Main");
            // main.logFile.flush();
            // View view = new View("192.168.191.78", 8888);
            // View view = new View("0.0.0.0", 8000);
            View view = new View(new File("src/affichage.cfg"));
            Aquarium aquarium = new Aquarium();
            ConcurrentLinkedQueue<ParserResult> receivedQueue = new ConcurrentLinkedQueue<ParserResult>();
            ConcurrentLinkedQueue<String> sendQueue = new ConcurrentLinkedQueue<String>();
            Runnable readFromServerThread = new ReadFromServerThread(view, receivedQueue, sendQueue, id);
            Runnable serverThread = new ServerThread(view, aquarium, receivedQueue, sendQueue, id);
            Runnable promptThread = new PromptThread(view, aquarium, receivedQueue, sendQueue, id);
            Thread prompt = new Thread(promptThread);
            Thread server = new Thread(serverThread);
            Thread io = new Thread(readFromServerThread);
            io.start();
            server.start();
            prompt.start();

            main.logFile.println("All threads running");
            main.logFile.flush();

            while (true) {
                // System.out.println("Main thread running");
                if (!aquarium.getFishes().isEmpty()) {
                    for (Fish fish : aquarium.getFishes()) {
                        if (fish.getSizeDestinations() > 0) {
                            main.logFile.println("It is " + Instant.now().getEpochSecond() + " and Fish "
                                    + fish.getName() + " is at " + fish.getPosition().toString()
                                    + " and needs to go to " + fish.getFirstDestination().toString() + " in "
                                    + (fish.getTimeToGetToFirstDestination() - Instant.now().getEpochSecond())
                                    + " seconds");
                            main.logFile.flush();
                        }
                    }
                }

                if (server.isInterrupted() || prompt.isInterrupted()) {
                    server.interrupt();
                    prompt.interrupt();
                    io.interrupt();
                    server.join();
                    main.logFile.println("Server thread finished");
                    main.logFile.flush();
                    prompt.join();
                    main.logFile.println("Prompt thread finished");
                    main.logFile.flush();
                    io.join();
                    main.logFile.println("IO thread finished");
                    main.logFile.flush();
                    view.close();
                    main.logFile.println("Main thread interrupted");
                    main.logFile.flush();
                    return;
                }
                Thread.sleep(500);
            }

            // io.join();
            // server.join();
            // prompt.join();
        } catch (IOException | InterruptedException e) {
            // TODO: handle exception
            main.logFile.println(e);
            main.logFile.flush();
        } catch (ParserException e) {
            System.out.println("ERROR while trying to parse config file: " + e.getMessage());
        }
    }
}
