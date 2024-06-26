import aquarium.*;

import java.io.*;
import java.util.concurrent.ConcurrentLinkedQueue;

import threads.*;
import utils.Log;
import utils.ParserException;
import utils.ParserResult;
import utils.Log.LogLevel;
import visuals.AquariumFX;

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
                });
            }
            fxThread.start();

            Log.logMessage(main.logFile, LogLevel.INFO, "All threads running");
            while (true) {
                if (!aquarium.getFishes().isEmpty()) {
                    for (Fish fish : aquarium.getFishes()) {
                        if (fish.getSizeDestinations() > 0) {
                            Log.logMessage(main.logFile, LogLevel.INFO,
                                    "It is " + System.currentTimeMillis() + " and Fish "
                                            + fish.getName() + " is at " + fish.getPosition().toString()
                                            + " and needs to go to " + fish.getFirstDestination().toString() + " in "
                                            + ((fish.getTimeToGetToFirstDestination() - System.currentTimeMillis())
                                                    / 1000)
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
            Log.logMessage(main.logFile, LogLevel.FATAL_ERROR, "while trying to parse config file: " + e.getMessage());
        } catch (Exception e) { // for aquariumFX.stop()
            Log.logMessage(main.logFile, LogLevel.ERROR, e.getMessage());
        }
    }
}
