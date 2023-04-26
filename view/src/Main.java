
// import utils.*;
import aquarium.*;
import java.io.*;

import threads.*;

public class Main {
    PrintWriter logFile;

    public Main() {
        try {
            logFile = new PrintWriter("log_main");
        } catch (IOException e) {
            System.out.println("Error creating log file for main thread");
        }
    }

    public static void main(String[] argv) {
        Main main = new Main();
        try {
            main.logFile.println("Main");
            main.logFile.flush();
            View view = new View("0.0.0.0", 8000);
            // View view = new View(new File("src/affichage.cfg"));
            Aquarium aquarium = new Aquarium();
            Runnable serverThread = new ServerThread(view, aquarium);
            Runnable promptThread = new PromptThread(view, aquarium);
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
