package threads;

import java.io.PrintWriter;

import aquarium.Aquarium;
import aquarium.Fish;
import utils.Log.LogLevel;
import utils.Log;
import utils.ParserResult;

public class PromptThreadHandlers {
    public static void handleOK(PrintWriter logFile, ParserResult command, Aquarium fishesList) {

        switch (command.getFunction()) {
            case ADD_FISH:
                Log.logMessage(logFile, LogLevel.INFO, "Adding fish " + command.getArgs().get(0) + " at "
                        + command.getArgs().get(1) + "x" + command.getArgs().get(2) + " of size "
                        + command.getArgs().get(3) + "x" + command.getArgs().get(4));
                Fish fishToAdd = new Fish(command.getArgs().get(0), // name
                        Integer.parseInt(command.getArgs().get(1)), // position.x
                        Integer.parseInt(command.getArgs().get(2)), // position.y

                        Integer.parseInt(command.getArgs().get(3)), // length
                        Integer.parseInt(command.getArgs().get(4))); // height
                fishesList.addFish(fishToAdd);
                Log.logMessage(logFile, LogLevel.INFO, "Fish added");
                break;
            case DEL_FISH:
                Log.logMessage(logFile, LogLevel.INFO, "Deleting fish " + command.getArgs().get(0));
                fishesList.removeFish(command.getArgs().get(0));
                Log.logMessage(logFile, LogLevel.INFO, "Fish deleted");
                break;
            case START_FISH:
                Log.logMessage(logFile, LogLevel.INFO, "Starting fish " + command.getArgs().get(0));
                fishesList.startFish(command.getArgs().get(0));
                Log.logMessage(logFile, LogLevel.INFO, "Fish started");
                break;
            default:
                Log.logMessage(logFile, LogLevel.WARNING, "Not a command from user prompt");
                break;
        }
    }

    public static void handleNOK(PrintWriter logFile, ParserResult command) {
        switch (command.getFunction()) {
            case ADD_FISH:
                Log.logMessage(logFile, LogLevel.ERROR, command.getArgs().get(0));
                break;
            case DEL_FISH:
            Log.logMessage(logFile, LogLevel.ERROR, command.getArgs().get(0));
                break;
            case START_FISH:
            Log.logMessage(logFile, LogLevel.ERROR, command.getArgs().get(0));
                break;
            default:
            Log.logMessage(logFile, LogLevel.WARNING, "Not a command from user prompt");
                break;
        }
    }

    public static void handleStatus(PrintWriter logFile, boolean isRunning, Aquarium fishesList) {
        if (isRunning) {
            System.out.println("OK : Connected to controller, " + fishesList.getFishes().size() + " fishes found");
            for (Fish fish : fishesList.getFishes()) {
                System.out.println(fish);
            }
        }
    }

    public static String doLogOut(PrintWriter logFile) {
        Log.logMessage(logFile, LogLevel.INFO, "Logging out");
        return new String("log out");
    }
}
