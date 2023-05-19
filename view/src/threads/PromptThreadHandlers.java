package threads;

import java.io.PrintWriter;

import aquarium.Aquarium;
import aquarium.Fish;
import utils.Parser;
import utils.ParserException;
import utils.ParserResult;

public class PromptThreadHandlers {
    public static void handleOK(PrintWriter logFile, String originalCommand, Aquarium fishesList) {
        try {
            ParserResult parsedCommand = Parser.parse(originalCommand);
            switch (parsedCommand.getFunction()) {
                case ADDFISH:
                    logFile.println("Adding fish " + parsedCommand.getArgs().get(0) + " at "
                            + parsedCommand.getArgs().get(1) + "x" + parsedCommand.getArgs().get(2) + " of size "
                            + parsedCommand.getArgs().get(3) + "x" + parsedCommand.getArgs().get(4));
                    logFile.flush();
                    Fish fishToAdd = new Fish(parsedCommand.getArgs().get(0), // name
                            Integer.parseInt(parsedCommand.getArgs().get(1)), // position.x
                            Integer.parseInt(parsedCommand.getArgs().get(2)), // position.y

                            Integer.parseInt(parsedCommand.getArgs().get(3)), // length
                            Integer.parseInt(parsedCommand.getArgs().get(4))); // height
                    fishesList.addFish(fishToAdd);
                    logFile.println("Fish added");
                    logFile.flush();
                    break;
                case DELFISH:
                    logFile.println("Deleting fish " + parsedCommand.getArgs().get(0));
                    logFile.flush();
                    fishesList.removeFish(parsedCommand.getArgs().get(0));
                    logFile.println("Fish deleted");
                    logFile.flush();
                    break;
                case STARTFISH:
                    logFile.println("Starting fish " + parsedCommand.getArgs().get(0));
                    logFile.flush();
                    fishesList.startFish(parsedCommand.getArgs().get(0));
                    logFile.println("Fish started");
                    logFile.flush();
                    break;
                case STATUS:
                    logFile.println("Status received");
                    logFile.flush();
                    System.out.println("Status ...");
                    break;
                default:
                    logFile.println("Not a command from user prompt");
                    logFile.flush();
                    break;
            }
        } catch (ParserException e) {
            logFile.println("ERROR: " + e.getMessage());
        }
    }

    public static void handleNOK(PrintWriter logFile, String originalCommand) {
        try {
            ParserResult parsedCommand = Parser.parse(originalCommand);
            switch (parsedCommand.getFunction()) {
                case ADDFISH:
                    logFile.println("Error: " + parsedCommand.getArgs().get(0));
                    logFile.flush();
                    break;
                case DELFISH:
                    logFile.println("Error: " + parsedCommand.getArgs().get(0));
                    logFile.flush();
                    break;
                case STARTFISH:
                    logFile.println("Error: " + parsedCommand.getArgs().get(0));
                    logFile.flush();
                    break;
                case STATUS:
                    logFile.println("Error: " + parsedCommand.getArgs().get(0));
                    logFile.flush();
                    break;
                default:
                    logFile.println("Not a command from user prompt");
                    logFile.flush();
                    break;
            }
        } catch (ParserException e) {
            logFile.println(e);
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
        logFile.println("Logging out");
        logFile.flush();
        return new String("log out");
    }
}
