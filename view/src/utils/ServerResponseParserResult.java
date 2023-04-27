package utils;

import java.util.ArrayList;

public class ServerResponseParserResult {
    private Parse.PossibleServerResponses function;
    private ArrayList<String> args;

    public ServerResponseParserResult(Parse.PossibleServerResponses function, ArrayList<String> args) {
        this.function = function;
        this.args = args;
    }

    public Parse.PossibleServerResponses getFunction() {
        return function;
    }

    public ArrayList<String> getArgs() {
        return args;
    }
}
