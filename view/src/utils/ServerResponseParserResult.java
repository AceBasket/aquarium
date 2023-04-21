package utils;
import java.util.ArrayList;

public class ServerResponseParserResult {
    private ServerResponseParsedFunctionTypes function;
    private ArrayList<String> args;

    public enum ServerResponseParsedFunctionTypes {
        GREETING, NOGREETING, OK, NOK, LISTFISHES, BYE, PONG // TODO : add ERROR
    }

    public ServerResponseParserResult(ServerResponseParsedFunctionTypes function, ArrayList<String> args) {
        this.function = function;
        this.args = args;
    }
    
    public ServerResponseParsedFunctionTypes getFunction() {
        return function;
    }

    public ArrayList<String> getArgs() {
        return args;
    }
}
