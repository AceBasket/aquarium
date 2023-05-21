
// import utils.*;
import aquarium.*;
import javafx.application.Platform;

import java.io.*;
import java.time.Instant;
import java.util.concurrent.ConcurrentLinkedQueue;

import threads.*;
import utils.ParserException;
import utils.ParserResult;
import visuals.AquariumFX;

public class Main {
    PrintWriter logFile;
    // private static AquariumFX aquariumFX;

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
            Aquarium aquarium = Aquarium.getInstance();
            ConcurrentLinkedQueue<ParserResult> receivedQueue = new ConcurrentLinkedQueue<ParserResult>();
            ConcurrentLinkedQueue<String> sendQueue = new ConcurrentLinkedQueue<String>();
            Runnable readFromServerThread = new ReadFromServerThread(view, receivedQueue, sendQueue, id);
            Runnable serverThread = new ServerThread(view, aquarium, receivedQueue, sendQueue, id);
            Runnable promptThread = new PromptThread(view, aquarium, receivedQueue, sendQueue, id);
            Thread prompt = new Thread(promptThread);
            Thread server = new Thread(serverThread);
            Thread io = new Thread(readFromServerThread);
            Thread fxThread = null;
            io.start();
            server.start();
            prompt.start();

            AquariumFX aquariumFX = new AquariumFX();
            boolean runningFX = false;

            if (!runningFX) {
                runningFX = true;
                main.logFile.println("Starting JavaFX");
                main.logFile.flush();
                fxThread = new Thread(() -> {
                    AquariumFX.setAquarium(aquarium);
                    AquariumFX.setId(id);
                    AquariumFX.launch(AquariumFX.class, argv);
                    // try {
                    // } catch (Exception e) {
                    // main.logFile.println(e);
                    // main.logFile.flush();
                    // }
                });
            }
            fxThread.start();

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
                    main.logFile.println("Interrupting all threads");
                    main.logFile.flush();
                    aquariumFX.stopAquariumFX();
                    server.interrupt();
                    prompt.interrupt();
                    io.interrupt();
                    fxThread.join();
                    main.logFile.println("FX thread finished");
                    main.logFile.flush();
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

        } catch (IOException | InterruptedException e) {
            main.logFile.println(e);
            main.logFile.flush();
        } catch (ParserException e) {
            System.out.println("ERROR while trying to parse config file: " + e.getMessage());
        } catch (Exception e) { // for aquariumFX.stop()
            main.logFile.println(e);
            main.logFile.flush();
        }
    }
}
