package threads;

import aquarium.*;
import utils.*;
import utils.Log.LogLevel;
import utils.Parser.PossibleResponses;

import java.io.*;
import java.security.InvalidParameterException;
import java.util.LinkedList;
import java.util.NoSuchElementException;
import java.util.concurrent.ConcurrentLinkedQueue;

public class PromptThread implements Runnable {
    private final ConcurrentLinkedQueue<ParserResult> receivedQueue;
    private final ConcurrentLinkedQueue<String> sendQueue;
    private final LinkedList<ParserResult> commandQueue;
    private Client client;
    private Aquarium fishesList = Aquarium.getInstance();
    private PrintWriter logFile;

    public PromptThread(Client client, Aquarium aquarium, ConcurrentLinkedQueue<ParserResult> receivedQueue,
            ConcurrentLinkedQueue<String> sendQueue, long id) {
        this.client = client;
        this.receivedQueue = receivedQueue;
        this.sendQueue = sendQueue;
        this.commandQueue = new LinkedList<ParserResult>();
        try {
            logFile = new PrintWriter("log_prompt_thread" + id + ".log");
        } catch (IOException e) {
            System.out.println("Error creating log file");
        }
    }

    public void run() {
        // TODO
        Log.logMessage(logFile, LogLevel.INFO, "Starting prompt thread");

        ParserResult response;
        String command;
        boolean responseReceived = true;

        ParserResult parsedCommand;
        while (true) {
            if (Thread.currentThread().isInterrupted()) {
                Log.logMessage(logFile, LogLevel.WARNING, "Prompt thread interrupted");
                Thread.currentThread().interrupt();
                return;
            }

            if (responseReceived) {
                Log.logMessage(logFile, LogLevel.INFO, "Waiting for user prompt");
                command = System.console().readLine(); // get user prompt
                if (command == null) {
                    sendQueue.offer(PromptThreadHandlers.doLogOut(logFile));
                    Thread.currentThread().interrupt();
                    Log.logMessage(logFile, LogLevel.WARNING, "Interrupting prompt thread after Ctrl+D");
                    responseReceived = false;
                    continue;
                }
                try {
                    parsedCommand = Parser.parse(command);
                    if (parsedCommand.getFunction() == PossibleResponses.STATUS) {
                        PromptThreadHandlers.handleStatus(logFile, client.isConnected(), fishesList);
                        continue;
                    }
                } catch (ParserException | InvalidParameterException e) {
                    System.out.println(e.getMessage());
                    continue;
                }
                commandQueue.add(parsedCommand);
                sendQueue.offer(command);
                Log.logMessage(logFile, LogLevel.INFO, "User asked for: " + command);
            }
            responseReceived = false;
            try {
                response = receivedQueue.peek();
                while (response == null) {
                    Log.logMessage(logFile, LogLevel.INFO, "Nothing to handle");
                    Thread.sleep(300); // sleep 0.3 second and try again
                    response = receivedQueue.peek();
                }
                switch (response.getFunction()) {
                    case OK:
                        PromptThreadHandlers.handleOK(logFile, commandQueue.removeFirst(), fishesList);
                        receivedQueue.remove();
                        System.out.println("OK");
                        responseReceived = true;
                        break;
                    case NOK:
                        PromptThreadHandlers.handleNOK(logFile, commandQueue.removeFirst());
                        receivedQueue.remove();
                        System.out.println("NOK");
                        responseReceived = true;
                        break;

                    default:
                        Log.logMessage(logFile, LogLevel.WARNING, response.getFunction() + ": Not a command handled by prompt thread");
                        Thread.sleep(300);
                        break;
                }

            } catch (InterruptedException e) {
                Log.logMessage(logFile, LogLevel.FATAL_ERROR, "Prompt thread interrupted");
                Thread.currentThread().interrupt();
                return;
            } catch (NoSuchElementException | InvalidParameterException e) {
                // if (e.getMessage().equals("null")) {
                // System.out.println(e);
                // }
                // Log.logMessage(logFile, LogLevel.ERROR, e);
                Log.logMessage(logFile, LogLevel.ERROR, e.getMessage() + " -> " + e.getCause());
            }
        }
    }
}
