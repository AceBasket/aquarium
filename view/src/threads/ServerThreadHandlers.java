package threads;

import java.io.PrintWriter;
import java.util.concurrent.ConcurrentLinkedQueue;
import aquarium.Aquarium;
import aquarium.Fish;
import aquarium.Client;
import utils.ParserResult;
import utils.Log;
import utils.Log.LogLevel;;

public class ServerThreadHandlers {
    public static String doHello(PrintWriter logFile, Client client) {
        if (client.getId().equals("")) {
            return new String("hello");
        }
        return new String("hello in as " + client.getId());
    }

    public static String doLs(PrintWriter logFile) {
        return new String("ls");
    }

    public static String doGetFishesContinuously(PrintWriter logFile) {
        return new String("getFishesContinuously");
    }

    public static void listHandler(PrintWriter logFile, Aquarium fishesList, ParserResult parsedResponse) {
        for (int i = 0; i < parsedResponse.getArgs().size(); i += 6) {
            Fish fish_to_update = new Fish();
            try {
                Log.logMessage(logFile, LogLevel.INFO, "Updating fish " + parsedResponse.getArgs().get(i));
                fish_to_update = fishesList.getFish(parsedResponse.getArgs().get(i));

            } catch (IllegalArgumentException e) {
                Log.logMessage(logFile, LogLevel.WARNING, "Fish " + parsedResponse.getArgs().get(i) + " does not exist");

                Fish fish_to_create = new Fish(parsedResponse.getArgs().get(i), // name
                        -1, -1, // position.x, position.y (until it reaches its destination)
                        Integer.parseInt(parsedResponse.getArgs().get(i + 3)), // width
                        Integer.parseInt(parsedResponse.getArgs().get(i + 4))); // height

                fishesList.addFish(fish_to_create);
                Log.logMessage(logFile, LogLevel.INFO, "Fish " + parsedResponse.getArgs().get(i) + " created");

                fish_to_create.start();
                Log.logMessage(logFile, LogLevel.INFO, "Fish " + parsedResponse.getArgs().get(i) + " started");

                fish_to_update = fish_to_create;

            } finally {
                fishesList.setFishDestination(fish_to_update, Integer.parseInt(parsedResponse.getArgs().get(i + 1)), // destination.x
                        Integer.parseInt(parsedResponse.getArgs().get(i + 2)), // destination.y
                        Integer.parseInt(parsedResponse.getArgs().get(i + 5))); // time to get to destination

                Log.logMessage(logFile, LogLevel.INFO, "Fish " + parsedResponse.getArgs().get(i) + " updated : will go to "
                        + parsedResponse.getArgs().get(i + 1) + "x" + parsedResponse.getArgs().get(i + 2) + " in "
                        + parsedResponse.getArgs().get(i + 5) + " seconds");
                Log.logMessage(logFile, LogLevel.INFO, parsedResponse.getArgs().get(i) + " updated. Has "
                        + fish_to_update.getSizeDestinations() + " destinations");

            }
        }
    }

    public static void greetingHandler(PrintWriter logFile, Client client, ParserResult parsedAnswer) {
        client.setId(parsedAnswer.getArgs().get(0));
        client.setConnected();
    }

    public static void noGreetingHandler(PrintWriter logFile, Client client, ConcurrentLinkedQueue<String> sendQueue)
            throws InterruptedException {
        Thread.sleep(10000); // wait 10 seconds
        sendQueue.offer(ServerThreadHandlers.doHello(logFile, client));
    }

    public static void byeHandler(PrintWriter logFile) {
        Log.logMessage(logFile, LogLevel.FATAL_ERROR, "Server thread interrupted");
        Thread.currentThread().interrupt();
    }
}
