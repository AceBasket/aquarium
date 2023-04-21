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
            String response = view.listenToServer();
            answer = Parse.parserServerResponse(response);
            if (answer.getFunction() == ServerResponseParserResult.ServerResponseParsedFunctionTypes.GREETING) {
                view.setId(answer.getArgs().get(0));
                System.out.println("Connected as " + view.getId());
            }
            else if (answer.getFunction() == ServerResponseParserResult.ServerResponseParsedFunctionTypes.NOGREETING) {
                System.out.println("Server is full");
            }
            else {
                System.out.println("Unknown error");
                System.out.println(response);
            }
            
        } catch (IOException|ParserException e) {
            // TODO: handle exception
        } catch(NullPointerException e) {
            System.out.println("answer is null");
        }
    }

    public ServerThread(View view) {
        this.view = view;
    }
}
