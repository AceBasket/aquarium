package threads;

import aquarium.*;
import utils.*;
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
            ConcurrentLinkedQueue<String> sendQueue) {
        this.view = view;
        this.fishesList = aquarium;
        this.receivedQueue = receivedQueue;
        this.sendQueue = sendQueue;
        this.commandQueue = new LinkedList<String>();
        try {
            logFile = new PrintWriter("log_prompt_thread.log");
        } catch (IOException e) {
            System.out.println("Error creating log file");
        }
    }

    public void run() {
        // TODO
        logFile.println("Starting prompt thread");
        logFile.flush();

        // while (!view.isConnected()) {
        // logFile.println("Waiting for connection");
        // logFile.flush();
        // try {
        // Thread.sleep(100);
        // } catch (InterruptedException e) {
        // // TODO Auto-generated catch block
        // System.out.println("PromptThread interrupted");
        // e.printStackTrace();
        // }
        // }

        ParserResult response;
        String command;
        while (true) {
            command = System.console().readLine(); // get user prompt
            // String serverResponse = transferCommand(command);
            // logFile.println("Server answered: " + serverResponse);
            // logFile.flush();
            sendQueue.offer(command);
            logFile.println("User asked for: " + command);
            try {
                response = receivedQueue.peek();
                if (response == null) {
                    logFile.println("Nothing to handle");
                    logFile.flush();
                    continue;
                }
                switch (response.getFunction()) {
                    case OK:
                        PromptThreadHandlers.handleOK(logFile, commandQueue.removeFirst(), fishesList);
                        receivedQueue.remove();
                        break;
                    case NOK:
                        PromptThreadHandlers.handleNOK(logFile, commandQueue.removeFirst());
                        receivedQueue.remove();
                        break;

                    default:
                        logFile.println("Not a command handle by prompt thread");
                        break;
                }

            } catch (NoSuchElementException | InvalidParameterException e) {
                logFile.println(e);
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
