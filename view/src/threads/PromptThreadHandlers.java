package threads;

import java.io.PrintWriter;

import aquarium.Aquarium;
import aquarium.Fish;
import utils.Log.LogLevel;
import utils.Log;
import utils.Parser;
import utils.ParserException;
import utils.ParserResult;

public class PromptThreadHandlers {
    public static void handleOK(PrintWriter logFile, String originalCommand, Aquarium fishesList) {
        try {
            ParserResult parsedCommand = Parser.parse(originalCommand);
            switch (parsedCommand.getFunction()) {
                case ADD_FISH:
                    Log.logMessage(logFile, LogLevel.INFO, "Adding fish " + parsedCommand.getArgs().get(0) + " at "
                            + parsedCommand.getArgs().get(1) + "x" + parsedCommand.getArgs().get(2) + " of size "
                            + parsedCommand.getArgs().get(3) + "x" + parsedCommand.getArgs().get(4));
                    Fish fishToAdd = new Fish(parsedCommand.getArgs().get(0), // name
                            Integer.parseInt(parsedCommand.getArgs().get(1)), // position.x
                            Integer.parseInt(parsedCommand.getArgs().get(2)), // position.y

                            Integer.parseInt(parsedCommand.getArgs().get(3)), // length
                            Integer.parseInt(parsedCommand.getArgs().get(4))); // height
                    fishesList.addFish(fishToAdd);
                    Log.logMessage(logFile, LogLevel.INFO, "Fish added");
                    break;
                case DEL_FISH:
                    Log.logMessage(logFile, LogLevel.INFO, "Deleting fish " + parsedCommand.getArgs().get(0));
                    fishesList.removeFish(parsedCommand.getArgs().get(0));
                    Log.logMessage(logFile, LogLevel.INFO, "Fish deleted");
                    break;
                case START_FISH:
                    Log.logMessage(logFile, LogLevel.INFO, "Starting fish " + parsedCommand.getArgs().get(0));
                    fishesList.startFish(parsedCommand.getArgs().get(0));
                    Log.logMessage(logFile, LogLevel.INFO, "Fish started");
                    break;
                case STATUS:
                    Log.logMessage(logFile, LogLevel.INFO, "Status received");
                    System.out.println("Status ...");
                    break;
                default:
                    Log.logMessage(logFile, LogLevel.INFO, "Not a command from user prompt");
                    break;
            }
        } catch (ParserException e) {
            Log.logMessage(logFile, LogLevel.ERROR, e.getMessage());
        }
    }

    public static void handleNOK(PrintWriter logFile, String originalCommand) {
        try {
            ParserResult parsedCommand = Parser.parse(originalCommand);
            switch (parsedCommand.getFunction()) {
                case ADD_FISH:
                    Log.logMessage(logFile, LogLevel.ERROR, "Starting " + parsedCommand.getArgs().get(0));
                    break;
                case DEL_FISH:
                    Log.logMessage(logFile, LogLevel.ERROR, "Starting " + parsedCommand.getArgs().get(0));
                    break;
                case START_FISH:
                    Log.logMessage(logFile, LogLevel.ERROR, "Starting " + parsedCommand.getArgs().get(0));
                    break;
                case STATUS:
                    Log.logMessage(logFile, LogLevel.ERROR, "Starting " + parsedCommand.getArgs().get(0));
                    break;
                default:
                    Log.logMessage(logFile, LogLevel.WARNING, "Not a command from user prompt");
                    break;
            }
        } catch (ParserException e) {
            Log.logMessage(logFile, LogLevel.ERROR, e.getMessage());
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
