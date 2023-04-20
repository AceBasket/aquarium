package utils;
import java.util.ArrayList;

public class ParserResult {
    private ParsedFunctionTypes function;
    private ArrayList<String> args;

    public ParserResult(ParsedFunctionTypes function, ArrayList<String> args) {
        this.function = function;
        this.args = args;
    }
    
    public ParsedFunctionTypes getFunction() {
        return function;
    }

    public ArrayList<String> getArgs() {
        return args;
    }
}
