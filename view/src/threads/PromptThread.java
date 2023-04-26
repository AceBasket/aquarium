package threads;

import aquarium.*;
import utils.*;
import java.io.*;

public class PromptThread implements Runnable {
    private View view;
    private Aquarium fishesList;
    private PrintWriter logFile;

    public void run() {
        // TODO
        logFile.println("PromptHandler");
        logFile.flush();

        while (view.connected == false) {
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
            command = System.console().readLine();
            String serverResponse = transferCommand(command);
            logFile.println("Server answered: " + serverResponse);
            logFile.flush();
            try {
                answer = Parse.parserServerResponse(serverResponse);
                if (answer.getFunction() == ServerResponseParserResult.ServerResponseParsedFunctionTypes.OK) {
                    promptCommand = Parse.parserCommand(command);
                    if (promptCommand.getFunction() == PromptParserResult.PromptParsedFunctionTypes.ADDFISH) {
                        Fish fishToAdd = new Fish(promptCommand.getArgs().get(0),
                                Integer.parseInt(promptCommand.getArgs().get(1)),
                                Integer.parseInt(promptCommand.getArgs().get(2)),

                                Integer.parseInt(promptCommand.getArgs().get(3)),
                                Integer.parseInt(promptCommand.getArgs().get(4)),
                                promptCommand.getArgs().get(5));
                        fishesList.addFish(fishToAdd);
                    } else if (promptCommand.getFunction() == PromptParserResult.PromptParsedFunctionTypes.DELFISH) {
                        fishesList.removeFish(promptCommand.getArgs().get(0));
                    } else if (promptCommand.getFunction() == PromptParserResult.PromptParsedFunctionTypes.STARTFISH) {
                        fishesList.startFish(promptCommand.getArgs().get(0));
                    }
                } else if (answer.getFunction() == ServerResponseParserResult.ServerResponseParsedFunctionTypes.NOK) {
                    logFile.println("Error: " + answer.getArgs().get(0));
                    logFile.flush();
                }
                System.out.println(serverResponse);

            } catch (ParserException e) {
                // TODO: handle exception
                System.out.println(e.getMessage());
            } catch (NullPointerException e) {
                System.out.println("answer is null");
            }
        }
    }

    public PromptThread(View view, Aquarium aquarium) {
        this.view = view;
        this.fishesList = aquarium;
        try {
            logFile = new PrintWriter("log_prompt_thread");
        } catch (IOException e) {
            // TODO: handle exception
            System.out.println("Error creating log file");
        }
    }

    public PromptThread() {
        this.view = null;
    }

    public String transferCommand(String command) {
        view.talkToServer(command);
        try {
            return view.listenToServer();
        } catch (Exception e) {
            System.out.println(e);
            return null;
        }
    }
}
