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

    // private Aquarium aquariumView;:

    public View(File config) throws IOException {
        displayTimeoutValue = utils.Parse.parserTimeout(config);
        id = utils.Parse.parserID(config);
        controllerAddress = utils.Parse.parserIP(config);
        portNumber = utils.Parse.parserPort(config);
        socket = new Socket(controllerAddress, portNumber);
        input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        output = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);
        
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getId() {
        return this.id;
    }

    public void talkToServer(String speech) {
        this.output.println(speech);
    }

    public String listenToServer() throws IOException {
        return this.input.readLine();
    }

    public void close() throws IOException {
        this.input.close();
        this.output.close();
        this.socket.close();
    }


    
}