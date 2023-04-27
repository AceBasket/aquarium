package threads;

import aquarium.*;
import utils.*;
import java.io.*;

public class ServerThread implements Runnable {
    private View view;
    private Aquarium fishesList;
    private PrintWriter logFile;

    public void run() {
        // TODO
        logFile.println("ServerThread");
        logFile.flush();

        System.out.println("ServerThread running");

        ServerResponseParserResult answer;
        try {
            // view.talkToServer("Testing connection");
            if (view.getId().equals("")) {
                view.talkToServer("hello");
            } else {
                view.talkToServer("hello in as " + view.getId());
            }
            String response = view.listenToServer();
            System.out.println(response);
            answer = Parse.parserServerResponse(response);
            if (answer.getFunction() == Parse.PossibleServerResponses.GREETING) {
                view.setId(answer.getArgs().get(0));
                logFile.println("Connected as " + view.getId());
                logFile.flush();
                view.connected = true;
            } else if (answer
                    .getFunction() == Parse.PossibleServerResponses.NOGREETING) {
                logFile.println("Server is full");
                logFile.flush();
            } else {
                System.out.println("Unknown error");
                System.out.println(response);
            }

        } catch (IOException | ParserException e) {
            // TODO: handle exception
            System.out.println(e.getMessage());
        } catch (NullPointerException e) {
            System.out.println("answer is null");
        }
    }

    public ServerThread(View view, Aquarium aquarium) {
        this.view = view;
        this.fishesList = aquarium;
        try {
            logFile = new PrintWriter("log_server_thread");
        } catch (IOException e) {
            System.out.println("Error creating log file");
        }
    }
}
