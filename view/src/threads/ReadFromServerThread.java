package threads;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.concurrent.BlockingQueue;
import aquarium.*;
import utils.*;

public class ReadFromServerThread {
    private final BlockingQueue<Parse> messageQueue;
    private PrintWriter logFile;
    private View view;

    public ReadFromServerThread(BlockingQueue<Parse> messageQueue) {
        this.messageQueue = messageQueue;
        try {
            logFile = new PrintWriter("log_io_thread.log");
        } catch (IOException e) {
            System.out.println("Error creating log file");
        }
    }

    public void run() {
        String response;
        while (true) {
            try {
                response = view.listenToServer();
                logFile.println("Server answered: " + response);
                logFile.flush();

            } catch (IOException e) {
                // TODO: handle exception
                System.out.println(e.getMessage());
            }
        }
    }
}
