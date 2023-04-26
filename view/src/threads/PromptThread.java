package threads;

import aquarium.*;
import utils.*;
public class PromptThread implements Runnable {
    private View view;
    public void run() {
        // TODO
        System.out.println("PromptHandler");

        while(view.connected == false) {
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                System.out.println("PromptThread interrupted");
                e.printStackTrace();
            }
        }

        ServerResponseParserResult answer;
        String command;
        while (true) {
            command = System.console().readLine();
            String serverResponse = transferCommand(command);
            try {
                answer = Parse.parserServerResponse(serverResponse);
                if (answer.getFunction() == OK)
                
            } catch (ParserException e) {
                // TODO: handle exception
                System.out.println(e.getMessage());
            }
        }
    }

    public PromptThread(View view) {
        this.view = view;
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
