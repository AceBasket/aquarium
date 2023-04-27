package utils;

import java.util.ArrayList;

public class PromptParserResult {
    private Parse.PromptCommandType function;
    private ArrayList<String> args;

    public PromptParserResult(Parse.PromptCommandType function, ArrayList<String> args) {
        this.function = function;
        this.args = args;
    }

    public Parse.PromptCommandType getFunction() {
        return function;
    }

    public ArrayList<String> getArgs() {
        return args;
    }
}
