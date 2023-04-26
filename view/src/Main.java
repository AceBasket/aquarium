import utils.*;
import aquarium.*;
import java.io.*;

import threads.*;
public class Main {
    public static void main(String[] argv) {
        System.out.println("Main");
        String hello = "Hello\rworld";
        System.out.println(hello.replace('\r', '\n'));
        try {
            View view = new View(new File("src/affichage.cfg"));
            Runnable serverThread = new ServerThread(view);
            Runnable promptThread = new PromptThread(view);
            Thread prompt = new Thread(promptThread);
            Thread server = new Thread(serverThread);
            server.start();
            prompt.start();
            server.join();
            prompt.join();
        } catch (Exception e) {
            // TODO: handle exception
            System.out.println(e);
        }
    }
}
