package threads;

import aquarium.*;
import utils.*;
import java.io.*;
import java.security.InvalidParameterException;

public class PromptThread implements Runnable {
    private View view;
    private Aquarium fishesList;
    private PrintWriter logFile;

    public void run() {
        // TODO
        logFile.println("Starting prompt thread");
        logFile.flush();

        while (!view.isConnected()) {
            logFile.println("Waiting for connection");
            logFile.flush();
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                System.out.println("PromptThread interrupted");
                e.printStackTrace();
            }
        }

        ServerResponseParserResult answer;
        PromptParserResult promptCommand;
        String command;
        while (true) {
            command = System.console().readLine(); // get user prompt
            String serverResponse = transferCommand(command);
            logFile.println("Server answered: " + serverResponse);
            logFile.flush();
            try {
                System.out.println(serverResponse);
                answer = Parse.parserServerResponse(serverResponse);
                if (answer.getFunction() == Parse.PossibleServerResponses.OK) {
                    promptCommand = Parse.parserCommand(command);
                    if (promptCommand.getFunction() == Parse.PromptCommandType.ADDFISH) {
                        logFile.println("Adding fish " + promptCommand.getArgs().get(0) + " at "
                                + promptCommand.getArgs().get(1) + "x" + promptCommand.getArgs().get(2) + " of size "
                                + promptCommand.getArgs().get(3) + "x" + promptCommand.getArgs().get(4));
                        logFile.flush();
                        Fish fishToAdd = new Fish(promptCommand.getArgs().get(0),
                                Integer.parseInt(promptCommand.getArgs().get(1)),
                                Integer.parseInt(promptCommand.getArgs().get(2)),

                                Integer.parseInt(promptCommand.getArgs().get(3)),
                                Integer.parseInt(promptCommand.getArgs().get(4)));
                        fishesList.addFish(fishToAdd);
                    } else if (promptCommand.getFunction() == Parse.PromptCommandType.DELFISH) {
                        logFile.println("Deleting fish " + promptCommand.getArgs().get(0));
                        logFile.flush();
                        fishesList.removeFish(promptCommand.getArgs().get(0));
                    } else if (promptCommand.getFunction() == Parse.PromptCommandType.STARTFISH) {
                        logFile.println("Starting fish " + promptCommand.getArgs().get(0));
                        logFile.flush();
                        fishesList.startFish(promptCommand.getArgs().get(0));
                    }
                } else if (answer.getFunction() == Parse.PossibleServerResponses.NOK) {
                    logFile.println("Error: " + answer.getArgs().get(0));
                    logFile.flush();
                }

            } catch (ParserException | InvalidParameterException e) {
                logFile.println(e);
                logFile.flush();
            } catch (NullPointerException e) {
                System.out.println("parsed response is null (an error occured)");
            }
        }
    }

    public PromptThread(View view, Aquarium aquarium) {
        this.view = view;
        this.fishesList = aquarium;
        try {
            logFile = new PrintWriter("log_prompt_thread.log");
        } catch (IOException e) {
            System.out.println("Error creating log file");
        }
    }

    public PromptThread() {
        this.view = null;
    }

    public String transferCommand(String command) {
        logFile.println("Sending command: " + command);
        logFile.flush();
        view.talkToServer(command);
        try {
            return view.listenToServer();
        } catch (Exception e) {
            System.out.println(e);
            return null;
        }
    }
}
