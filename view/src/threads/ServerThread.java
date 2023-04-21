package threads;
import aquarium.*;
import utils.*;
import java.io.*;
public class ServerThread implements Runnable {
    private View view;
    public void run() {
        // TODO
        System.out.println("ServerThread");

        ServerResponseParserResult answer;
        try {
            // view.talkToServer("Testing connection");
            view.talkToServer("hello in as " + view.getId());
            answer = Parse.parserServerResponse(view.listenToServer());
            if (answer.getFunction() == ServerResponseParserResult.ServerResponseParsedFunctionTypes.GREETING) {
                view.setId(answer.getArgs().get(0));
            }
            else if (answer.getFunction() == ServerResponseParserResult.ServerResponseParsedFunctionTypes.NOGREETING) {
                System.out.println("Server is full");
            }
            else {
                System.out.println("Unknown error");
            }
            
        } catch (IOException|ParserException e) {
            // TODO: handle exception
        }
    }

    public ServerThread(View view) {
        this.view = view;
    }
}
