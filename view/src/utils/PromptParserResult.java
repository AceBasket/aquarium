package utils;
import java.util.ArrayList;

public class PromptParserResult {
    private PromptParsedFunctionTypes function;
    private ArrayList<String> args;

    public enum PromptParsedFunctionTypes {
        STATUS, ADDFISH, DELFISH, STARTFISH
    }

    public PromptParserResult(PromptParsedFunctionTypes function, ArrayList<String> args) {
        this.function = function;
        this.args = args;
    }
    
    public PromptParsedFunctionTypes getFunction() {
        return function;
    }

    public ArrayList<String> getArgs() {
        return args;
    }
}
