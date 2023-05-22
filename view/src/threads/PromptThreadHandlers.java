package threads;

import java.io.PrintWriter;

import aquarium.Aquarium;
import aquarium.Fish;
import utils.Parser;
import utils.ParserException;
import utils.ParserResult;

public class PromptThreadHandlers {
    public static void handleOK(PrintWriter logFile, ParserResult command, Aquarium fishesList) {

        switch (command.getFunction()) {
            case ADD_FISH:
                logFile.println("Adding fish " + command.getArgs().get(0) + " at "
                        + command.getArgs().get(1) + "x" + command.getArgs().get(2) + " of size "
                        + command.getArgs().get(3) + "x" + command.getArgs().get(4));
                logFile.flush();
                Fish fishToAdd = new Fish(command.getArgs().get(0), // name
                        Integer.parseInt(command.getArgs().get(1)), // position.x
                        Integer.parseInt(command.getArgs().get(2)), // position.y

                        Integer.parseInt(command.getArgs().get(3)), // length
                        Integer.parseInt(command.getArgs().get(4))); // height
                fishesList.addFish(fishToAdd);
                logFile.println("Fish added");
                logFile.flush();
                break;
            case DEL_FISH:
                logFile.println("Deleting fish " + command.getArgs().get(0));
                logFile.flush();
                fishesList.removeFish(command.getArgs().get(0));
                logFile.println("Fish deleted");
                logFile.flush();
                break;
            case START_FISH:
                logFile.println("Starting fish " + command.getArgs().get(0));
                logFile.flush();
                fishesList.startFish(command.getArgs().get(0));
                logFile.println("Fish started");
                logFile.flush();
                break;
            default:
                logFile.println("Not a command from user prompt");
                logFile.flush();
                break;
        }
    }

    public static void handleNOK(PrintWriter logFile, ParserResult command) {
        switch (command.getFunction()) {
            case ADD_FISH:
                logFile.println("Error: " + command.getArgs().get(0));
                logFile.flush();
                break;
            case DEL_FISH:
                logFile.println("Error: " + command.getArgs().get(0));
                logFile.flush();
                break;
            case START_FISH:
                logFile.println("Error: " + command.getArgs().get(0));
                logFile.flush();
                break;
            default:
                logFile.println("Not a command from user prompt");
                logFile.flush();
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
        logFile.println("Logging out");
        logFile.flush();
        return new String("log out");
    }
}
