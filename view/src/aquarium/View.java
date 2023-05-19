package aquarium;

import java.io.*;
import java.net.*;
import utils.*;

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
    public volatile boolean connected = false;

    // private Aquarium aquariumView;:

    public View(File config) throws IOException, ParserException {
        displayTimeoutValue = Parser.parserTimeout(config);
        // resources = utils.Parse.parserResources(config);
        id = Parser.parserID(config);
        controllerAddress = Parser.parserIP(config);
        portNumber = Parser.parserPort(config);
        socket = new Socket(controllerAddress, portNumber);
        input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        output = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);
    }

    public View(String controllerAddress, int portNumber) throws IOException {
        this.controllerAddress = controllerAddress;
        this.portNumber = portNumber;
        socket = new Socket(controllerAddress, portNumber);
        input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        output = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);
    }

    public void setId(String id) {
        this.id = id;
    }

    public synchronized boolean isConnected() {
        return this.connected;
    }

    public synchronized void connect() {
        this.connected = true;
    }

    public String getId() {
        return this.id;
    }

    public synchronized void talkToServer(String speech) {
        this.output.println(speech);
    }

    public synchronized String listenToServer() throws IOException {
        String answer = this.input.readLine();
        if (answer == null) {
            return null;
        }
        return answer.replace("@@", "\n");
    }

    public synchronized boolean serverIsTalking() throws IOException {
        return this.input.ready();
    }

    public int getDisplayTimeoutValue() {
        return this.displayTimeoutValue;
    }

    public void close() throws IOException {
        this.input.close();
        this.output.close();
        this.socket.close();
    }

}