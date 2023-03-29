package aquarium;
import java.io.*;
import java.net.*;
import java.nio.file.Path;

public class View {
    // for socket
    private int portNumber;
    private String controllerAddress;
    private Socket socket;
    private BufferedReader input;
    private PrintWriter output;

    // useful for communication with controller
    private String id = "";
    private int displayTimeoutValue;
    private String resources;

    // private Aquarium aquariumView;:

    public View(File config) throws IOException {
        displayTimeoutValue = utils.Parse.parserTimeout(config);
        resources = utils.Parse.parserResources(config);
        id = utils.Parse.parserID(config);
        controllerAddress = utils.Parse.parserIP(config);
        portNumber = utils.Parse.parserPort(config);
        socket = new Socket(controllerAddress, portNumber);
        input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        output = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);
        
    }


    private void init() {
        if (this.id != "") {
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

            View view = new View(8888, "192.168.57.203");

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