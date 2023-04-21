package aquarium;
import java.io.*;
import java.net.*;
import java.nio.file.Path;

public class View {
    private int portNumber;
    private int id = 0;
    private int displayTimeoutValue;
    private Path resources;
    private String controllerAddress;
    private Socket socket;
    private BufferedReader input;
    private PrintWriter output;

    public View(int port, String address, Path resources) throws IOException {
        this.resources = resources;
        portNumber = port;
        controllerAddress = address;
        socket = new Socket(address, portNumber);
        input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        output = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);
    }


    private void init() {
        if (this.id != 0) {
            this.output.println("hello in as " + this.id);
        } else {
            this.output.println("hello");
        }
    }

    private void getFishes() {
        this.output.println("getFishes");
    }

    private void getFishesContinuously() {
        this.output.println("getFishesContinuously");
    }

    private void getListFishes() {
        this.output.println("ls");
    }

    private void logOut() {
        this.output.println("log out");
    }

    private void ping() {
        this.output.println("ping " + this.id);
    }

    // prototype not fixed
    private void addFish(String name, String destination, String size, String path) {
        this.output.println("addFish  " + name + " at " + destination + ", " + size + ", " + path);
    }

    // prototype not fixed
    private void delFish(String name) {
        this.output.println("delFish " + name);
    }

    // prototype not fixed
    private void startFish(String name) {
        this.output.println("delFish " + name);
    }

    public static void main(String[] argv) {
        try {

            View view = new View(8888, "192.168.57.203", null);

            view.output.println("Testing connection");

            System.out.println("END");
            view.output.println("END");
            view.input.close();
            view.output.close();
            view.socket.close();
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }
}