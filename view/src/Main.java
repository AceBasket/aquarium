import utils.*;
import aquarium.*;
import java.io.*;

import threads.*;
public class Main {
    public static void main(String[] argv) {
        System.out.println("Main");
        try {
            Runnable promptThread = new PromptThread();
            // Runnable promptThread = new PromptThread(new View(new File("src/affichage.cfg")));
            Thread prompt = new Thread(promptThread);
            prompt.start();
            prompt.join();
        } catch (Exception e) {
            // TODO: handle exception
            System.out.println(e);
        }
    }
}
