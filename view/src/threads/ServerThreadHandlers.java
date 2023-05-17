package threads;

import java.io.PrintWriter;
import java.util.concurrent.ConcurrentLinkedQueue;

import aquarium.Aquarium;
import aquarium.Fish;
import aquarium.View;
import utils.ParserResult;
import java.io.IOException;

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
                logFile.println("Updating fish " + parsedResponse.getArgs().get(i));
                logFile.flush();
                Fish fish_to_update = fishesList.getFish(parsedResponse.getArgs().get(i));
                fishesList.setFishDestination(fish_to_update, Integer.parseInt(parsedResponse.getArgs().get(i + 1)), // destination.x
                        Integer.parseInt(parsedResponse.getArgs().get(i + 2)), // destination.y
                        Integer.parseInt(parsedResponse.getArgs().get(i + 5))); // time to get to destination
                // logFile.println("Fish " + parsedResponse.getArgs().get(i) + " updated : will
                // go to "
                // + parsedResponse.getArgs().get(i + 1) + "x" + parsedResponse.getArgs().get(i
                // + 2) + " in "
                // + parsedResponse.getArgs().get(i + 5) + " seconds");
                logFile.println(parsedResponse.getArgs().get(i) + " updated. Has "
                        + fish_to_update.getSizeDestinations() + " destinations");
                logFile.flush();
            } catch (IllegalArgumentException e) {
                logFile.println("Fish " + parsedResponse.getArgs().get(i) + " does not exist");
                logFile.flush();
                Fish fish_to_create = new Fish(parsedResponse.getArgs().get(i),
                        Integer.parseInt(parsedResponse.getArgs().get(i + 1)), // position.x
                        Integer.parseInt(parsedResponse.getArgs().get(i + 2)), // position.y
                        Integer.parseInt(parsedResponse.getArgs().get(i + 3)), // width
                        Integer.parseInt(parsedResponse.getArgs().get(i + 4))); // height
                fishesList.addFish(fish_to_create);
                logFile.println("Fish " + parsedResponse.getArgs().get(i) + " created");
                logFile.flush();
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

    public static void logOutHandler(PrintWriter logFile, View view) {
        try {
            view.close();
        } catch (IOException e) {
            logFile.println("Can't close the connection from the view" + view.getId());
        }
    }
}
