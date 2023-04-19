package utils;
import java.util.ArrayList;


public class ServerResponseParserResult {
    private PossibleServerResponses response;
    private ArrayList<String> args;

    public ServerResponseParserResult(PossibleServerResponses response, ArrayList<String> args) {
        this.response = response;
        this.args = args;
    }
    
    public PossibleServerResponses getResponse() {
        return response;
    }

    public ArrayList<String> getArgs() {
        return args;
    }
}
