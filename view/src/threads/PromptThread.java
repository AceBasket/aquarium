package threads;

import aquarium.*;
import utils.*;
public class PromptThread implements Runnable {
    private View view;
    public void run() {
        // TODO
        System.out.println("PromptHandler");

        PromptParserResult answer;
        String command;
        while (true) {
            command = System.console().readLine();
            System.out.println(transferCommand(command));
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
