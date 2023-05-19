package threads;

import aquarium.*;
import utils.*;
import utils.Parser.PossibleResponses;

import java.io.*;
import java.security.InvalidParameterException;
import java.util.LinkedList;
import java.util.NoSuchElementException;
import java.util.concurrent.ConcurrentLinkedQueue;

public class PromptThread implements Runnable {
    private final ConcurrentLinkedQueue<ParserResult> receivedQueue;
    private final ConcurrentLinkedQueue<String> sendQueue;
    private final LinkedList<String> commandQueue;
    private View view;
    private Aquarium fishesList;
    private PrintWriter logFile;

    public PromptThread(View view, Aquarium aquarium, ConcurrentLinkedQueue<ParserResult> receivedQueue,
            ConcurrentLinkedQueue<String> sendQueue, long id) {
        this.view = view;
        this.fishesList = aquarium;
        this.receivedQueue = receivedQueue;
        this.sendQueue = sendQueue;
        this.commandQueue = new LinkedList<String>();
        try {
            logFile = new PrintWriter("log_prompt_thread" + id + ".log");
        } catch (IOException e) {
            System.out.println("Error creating log file");
        }
    }

    public void run() {
        // TODO
        logFile.println("Starting prompt thread");
        logFile.flush();

        ParserResult response;
        String command;
        boolean responseReceived = true;

        ParserResult parsedCommand;
        while (true) {
            if (Thread.currentThread().isInterrupted()) {
                logFile.println("Prompt thread interrupted");
                logFile.flush();
                Thread.currentThread().interrupt();
                return;
            }

            if (responseReceived) {
                logFile.println("Waiting for user prompt");
                logFile.flush();
                command = System.console().readLine(); // get user prompt
                if (command == null) {
                    sendQueue.offer(PromptThreadHandlers.doLogOut(logFile));
                    Thread.currentThread().interrupt();
                    logFile.println("Interrupting prompt thread after Ctrl+D");
                    responseReceived = false;
                    continue;
                }
                try {
                    parsedCommand = Parser.parse(command);
                    if (parsedCommand.getFunction() == PossibleResponses.STATUS) {
                        PromptThreadHandlers.handleStatus(logFile, view.isConnected(), fishesList);
                        continue;
                    }
                } catch (ParserException | InvalidParameterException e) {
                    System.out.println(e.getMessage());
                    continue;
                }
                commandQueue.add(command);
                sendQueue.offer(command);
                logFile.println("User asked for: " + command);
            }
            responseReceived = false;
            try {
                response = receivedQueue.peek();
                while (response == null) {
                    logFile.println("Nothing to handle");
                    logFile.flush();
                    Thread.sleep(500); // sleep 0.1 second and try again
                    response = receivedQueue.peek();
                }
                responseReceived = true;
                switch (response.getFunction()) {
                    case OK:
                        PromptThreadHandlers.handleOK(logFile, commandQueue.removeFirst(), fishesList);
                        receivedQueue.remove();
                        System.out.println("OK");
                        break;
                    case NOK:
                        PromptThreadHandlers.handleNOK(logFile, commandQueue.removeFirst());
                        receivedQueue.remove();
                        System.out.println("NOK");
                        break;

                    default:
                        logFile.println("Not a command handled by prompt thread");
                        logFile.flush();
                        break;
                }

            } catch (InterruptedException e) {
                logFile.println("Prompt thread interrupted");
                logFile.flush();
                Thread.currentThread().interrupt();
                return;
            } catch (NoSuchElementException | InvalidParameterException e) {
                // if (e.getMessage().equals("null")) {
                // System.out.println(e);
                // }
                logFile.println(e);
                logFile.println("Error: " + e.getMessage());
                logFile.println("Cause: " + e.getCause());
                logFile.flush();
            }
        }
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
