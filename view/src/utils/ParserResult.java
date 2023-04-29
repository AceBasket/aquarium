package utils;

import java.util.ArrayList;

public class ParserResult {
    private Parser.PossibleResponses function;
    private ArrayList<String> args;

    public ParserResult(Parser.PossibleResponses function, ArrayList<String> args) {
        this.function = function;
        this.args = args;
    }

    public Parser.PossibleResponses getFunction() {
        return function;
    }

    public ArrayList<String> getArgs() {
        return args;
    }
}
