package threads;

import java.io.PrintWriter;
import java.util.concurrent.ConcurrentLinkedQueue;
// import java.util.concurrent.Executors;
// import java.util.concurrent.ScheduledExecutorService;
// import java.util.concurrent.TimeUnit;

import aquarium.Aquarium;
import aquarium.Fish;
import aquarium.View;
import utils.ParserResult;
import utils.Log;
import utils.Log.LogLevel;;

public class ServerThreadHandlers {
    public static String doHello(PrintWriter logFile, View view) {
        if (view.getId().equals("")) {
            return new String("hello");
        }
        return new String("hello in as " + view.getId());
    }

    public static String doLs(PrintWriter logFile) {
        return new String("ls");
    }

    public static String doGetFishesContinuously(PrintWriter logFile) {
        return new String("getFishesContinuously");
    }

    public static void listHandler(PrintWriter logFile, Aquarium fishesList, ParserResult parsedResponse) {
        for (int i = 0; i < parsedResponse.getArgs().size(); i += 6) {
            try {
                Log.logMessage(logFile, LogLevel.INFO, "Updating fish " + parsedResponse.getArgs().get(i));
                Fish fish_to_update = fishesList.getFish(parsedResponse.getArgs().get(i));
                fishesList.setFishDestination(fish_to_update, Integer.parseInt(parsedResponse.getArgs().get(i + 1)), // destination.x
                        Integer.parseInt(parsedResponse.getArgs().get(i + 2)), // destination.y
                        Integer.parseInt(parsedResponse.getArgs().get(i + 5))); // time to get to destination

                Log.logMessage(logFile, LogLevel.INFO, "Fish " + parsedResponse.getArgs().get(i) + " updated : will go to "
                        + parsedResponse.getArgs().get(i + 1) + "x" + parsedResponse.getArgs().get(i + 2) + " in "
                        + parsedResponse.getArgs().get(i + 5) + " seconds");
                Log.logMessage(logFile, LogLevel.INFO, parsedResponse.getArgs().get(i) + " updated. Has "
                        + fish_to_update.getSizeDestinations() + " destinations");
            } catch (IllegalArgumentException e) {
                Log.logMessage(logFile, LogLevel.WARNING, "Fish " + parsedResponse.getArgs().get(i) + " does not exist");

                Fish fish_to_create = new Fish(parsedResponse.getArgs().get(i), // name
                        -1, -1, // position.x, position.y (until it reaches its destination)
                        // Integer.parseInt(parsedResponse.getArgs().get(i + 1)), // position.x
                        // Integer.parseInt(parsedResponse.getArgs().get(i + 2)), // position.y
                        Integer.parseInt(parsedResponse.getArgs().get(i + 3)), // width
                        Integer.parseInt(parsedResponse.getArgs().get(i + 4))); // height

                fishesList.addFish(fish_to_create);
                Log.logMessage(logFile, LogLevel.INFO, "Fish " + parsedResponse.getArgs().get(i) + " created");

                // ScheduledExecutorService scheduler = Executors.newScheduledThreadPool(1);

                // int delay = Integer.parseInt(parsedResponse.getArgs().get(i + 5));

                // Runnable action = new Runnable() {
                // @Override
                // public void run() {
                // fishesList.addFish(fish_to_create);
                // fish_to_create.start();
                // }
                // };

                // scheduler.schedule(action, delay, TimeUnit.SECONDS);
                fish_to_create.start();
                Log.logMessage(logFile, LogLevel.INFO, "Fish " + parsedResponse.getArgs().get(i) + " started");

                fishesList.setFishDestination(fish_to_create, Integer.parseInt(parsedResponse.getArgs().get(i + 1)), // destination.x
                        Integer.parseInt(parsedResponse.getArgs().get(i + 2)), // destination.y
                        Integer.parseInt(parsedResponse.getArgs().get(i + 5))); // time to get to destination

                Log.logMessage(logFile, LogLevel.INFO, "Fish " + parsedResponse.getArgs().get(i) + " updated : will go to "
                        + parsedResponse.getArgs().get(i + 1) + "x" + parsedResponse.getArgs().get(i + 2) + " in "
                        + parsedResponse.getArgs().get(i + 5) + " seconds");
                // scheduler.shutdown();
            }
        }
    }

    public static void getFishesHandler(PrintWriter logFile, View view, ParserResult parsedAnswer) {

    }

    public static void greetingHandler(PrintWriter logFile, View view, ParserResult parsedAnswer) {
        view.setId(parsedAnswer.getArgs().get(0));
        view.connect();
    }

    public static void noGreetingHandler(PrintWriter logFile, View view, ConcurrentLinkedQueue<String> sendQueue)
            throws InterruptedException {
        Thread.sleep(10000); // wait 10 seconds
        sendQueue.offer(ServerThreadHandlers.doHello(logFile, view));
    }

    public static void byeHandler(PrintWriter logFile) {
        Log.logMessage(logFile, LogLevel.FATAL_ERROR, "Server thread interrupted");
        Thread.currentThread().interrupt();
    }
}
