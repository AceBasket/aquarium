package utils;
import java.util.ArrayList;

public class PromptParserResult {
    private PromptCommandType command;
    private ArrayList<String> args;

    public PromptParserResult(PromptCommandType command, ArrayList<String> args) {
        this.command = command;
        this.args = args;
    }
    
    public PromptCommandType getCommand() {
        return command;
    }

    public ArrayList<String> getArgs() {
        return args;
    }
}
