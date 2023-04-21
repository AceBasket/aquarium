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
            try {
                answer = Parse.parserCommand(command);
            } catch (ParserException e) {
                // TODO: handle exception
            }
            switch (answer.getFunction()) {
                case STATUS:
                    System.out.println("STATUS");
                    break;
                case ADDFISH:
                    System.out.println("ADDFISH");
                    break;
                case DELFISH:
                    System.out.println("DELFISH");
                    break;
                case STARTFISH: 
                    System.out.println("STARTFISH");
                    break;
                default:
                    break;
            }
        }
    }

    public PromptThread(View view) {
        this.view = view;
    }

    public PromptThread() {
        this.view = null;
    }
}
