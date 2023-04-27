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
        logFile.println("Starting server thread");
        logFile.flush();

        ServerResponseParserResult parsedAnswer;
        String response;

        // First thing first is greeting the server
        try {
            // view.talkToServer("Testing connection");
            ServerThreadHandlers.doHello(view);
            response = view.listenToServer();
            System.out.println(response);
            parsedAnswer = Parse.parserServerResponse(response);
            if (parsedAnswer.getFunction() == Parse.PossibleServerResponses.GREETING) {
                ServerThreadHandlers.greetingHandler(view, parsedAnswer);
                logFile.println("Connected as " + view.getId());
                logFile.flush();
            } else if (parsedAnswer
                    .getFunction() == Parse.PossibleServerResponses.NOGREETING) {
                logFile.println("Server is full");
                logFile.flush();
            } else {
                logFile.println("Response was neither greeting nor nogreeting");
                logFile.flush();
            }

        } catch (IOException | ParserException e) {
            // TODO: handle exception
            System.out.println(e.getMessage());
        } catch (NullPointerException e) {
            logFile.println("Parsed Response failed and was null");
            logFile.flush();
        }

        int nbListFishes = 0;

        while (true) {
            while (fishesList.getFishes().size() == 0) {
                logFile.println("No fish in aquarium, need to add some");
                logFile.flush();
                try {
                    Thread.sleep(1000); // sleep for 1 second if no fish exist --> until there is at least 1 fish
                    continue;
                } catch (InterruptedException e) {
                    logFile.println("ERROR: " + e.getMessage());
                    logFile.flush();
                }
            }
            for (Fish fish : fishesList.getFishes()) {
                if (fish.getSizeDestinations() < 2) {
                    ServerThreadHandlers.doLs(view);
                    nbListFishes++;
                }
            }
            if (nbListFishes == 0) {
                try {
                    Thread.sleep(1000); // sleep for 1 second if no fish need an update in their destinations
                    continue;
                } catch (InterruptedException e) {
                    logFile.println("ERROR: " + e.getMessage());
                    logFile.flush();
                }
            }
            try {
                /* read all lines and handle fishes until no more communication */
                response = view.listenToServer();
                while (response != null) {
                    parsedAnswer = Parse.parserServerResponse(response);
                    if (parsedAnswer.getFunction() == Parse.PossibleServerResponses.LISTFISHES) {
                        ServerThreadHandlers.listHandler(view, fishesList, parsedAnswer);
                    }
                    response = view.listenToServer();
                }
            } catch (IOException e) {
                logFile.println("ERROR: " + e.getMessage());
                logFile.flush();
            } catch (ParserException e) {
                System.out.println(e);
            }
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
