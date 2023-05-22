
// import utils.*;
import aquarium.*;
import javafx.application.Platform;

import java.io.*;
import java.time.Instant;
import java.util.concurrent.ConcurrentLinkedQueue;

import threads.*;
import utils.Log;
import utils.ParserException;
import utils.ParserResult;
import utils.Log.LogLevel;
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
            LogLevel verbosityLevel = LogLevel.valueOf(System.getProperty("VERBOSITY_LEVEL", "INFO"));
            Log.setVerbosityLevel(verbosityLevel);
            Client client = new Client(new File("src/affichage.cfg"));
            Aquarium aquarium = Aquarium.getInstance();
            ConcurrentLinkedQueue<ParserResult> receivedQueue = new ConcurrentLinkedQueue<ParserResult>();
            ConcurrentLinkedQueue<String> sendQueue = new ConcurrentLinkedQueue<String>();
            Runnable readFromServerThread = new ReadFromServerThread(client, receivedQueue, sendQueue, id);
            Runnable serverThread = new ServerThread(client, aquarium, receivedQueue, sendQueue, id);
            Runnable promptThread = new PromptThread(client, aquarium, receivedQueue, sendQueue, id);
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
                Log.logMessage(main.logFile, LogLevel.INFO, "Starting JavaFX");
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

            Log.logMessage(main.logFile, LogLevel.INFO, "All threads running");
            while (true) {
                // System.out.println("Main thread running");
                if (!aquarium.getFishes().isEmpty()) {
                    for (Fish fish : aquarium.getFishes()) {
                        if (fish.getSizeDestinations() > 0) {
                            Log.logMessage(main.logFile, LogLevel.INFO, "It is " + Instant.now().getEpochSecond() + " and Fish "
                                    + fish.getName() + " is at " + fish.getPosition().toString()
                                    + " and needs to go to " + fish.getFirstDestination().toString() + " in "
                                    + (fish.getTimeToGetToFirstDestination() - Instant.now().getEpochSecond())
                                    + " seconds");
                        }
                    }
                }

                if (server.isInterrupted() || prompt.isInterrupted()) {
                    Log.logMessage(main.logFile, LogLevel.WARNING, "Interrupting all threads");
                    aquariumFX.stopAquariumFX();
                    server.interrupt();
                    prompt.interrupt();
                    io.interrupt();
                    fxThread.join();
                    Log.logMessage(main.logFile, LogLevel.INFO, "FX thread finished");
                    server.join();
                    Log.logMessage(main.logFile, LogLevel.INFO, "Server thread finished");
                    prompt.join();
                    Log.logMessage(main.logFile, LogLevel.INFO, "Prompt thread finished");
                    io.join();
                    Log.logMessage(main.logFile, LogLevel.INFO, "IO thread finished");
                    client.close();
                    Log.logMessage(main.logFile, LogLevel.INFO, "Main thread interrupted");
                    return;
                }
                Thread.sleep(500);
            }

        } catch (IOException | InterruptedException e) {
            Log.logMessage(main.logFile, LogLevel.ERROR, e.getMessage());
        } catch (ParserException e) {
            System.out.println("ERROR while trying to parse config file: " + e.getMessage());
        } catch (Exception e) { // for aquariumFX.stop()
            Log.logMessage(main.logFile, LogLevel.ERROR, e.getMessage());
        }
    }
}
