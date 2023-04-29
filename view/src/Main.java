
// import utils.*;
import aquarium.*;
import java.io.*;
import java.util.concurrent.ConcurrentLinkedQueue;

import threads.*;
import utils.ParserResult;

public class Main {
    PrintWriter logFile;

    public Main() {
        try {
            logFile = new PrintWriter("log_main.log");
        } catch (IOException e) {
            System.out.println("Error creating log file for main thread");
        }
    }

    public static void main(String[] argv) {
        // Main main = new Main();
        try {
            // main.logFile.println("Main");
            // main.logFile.flush();
            // View view = new View("192.168.191.78", 8888);
            View view = new View("0.0.0.0", 8000);
            // View view = new View(new File("src/affichage.cfg"));
            Aquarium aquarium = new Aquarium();
            ConcurrentLinkedQueue<ParserResult> receivedQueue = new ConcurrentLinkedQueue<ParserResult>();
            ConcurrentLinkedQueue<String> sendQueue = new ConcurrentLinkedQueue<String>();
            Runnable readFromServerThread = new ReadFromServerThread(view, receivedQueue, sendQueue);
            Runnable serverThread = new ServerThread(view, aquarium, receivedQueue, sendQueue);
            Runnable promptThread = new PromptThread(view, aquarium, receivedQueue, sendQueue);
            Thread prompt = new Thread(promptThread);
            Thread server = new Thread(serverThread);
            Thread io = new Thread(readFromServerThread);
            io.start();
            server.start();
            prompt.start();

            // while (true) {
            // // System.out.println("Main thread running");
            // main.logFile.println("Main thread running");
            // main.logFile.flush();
            // Thread.sleep(1000);
            // }
            io.join();
            server.join();
            prompt.join();
        } catch (Exception e) {
            // TODO: handle exception
            System.out.println(e);
        }
    }
}
