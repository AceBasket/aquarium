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
    public boolean connected = false;

    // private Aquarium aquariumView;:

    public View(File config) throws IOException, ParserException {
        displayTimeoutValue = Parse.parserTimeout(config);
        // resources = utils.Parse.parserResources(config);
        id = Parse.parserID(config);
        controllerAddress = Parse.parserIP(config);
        portNumber = Parse.parserPort(config);
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

    public synchronized void talkToServer(String speech) {
        this.output.println(speech);
    }

    public synchronized String listenToServer() throws IOException {
        String answer = this.input.readLine();
        if (answer == null) {
            throw new IOException("Server is down");
        }
        return answer.replace("@@", "\n");
    }

    public void close() throws IOException {
        this.input.close();
        this.output.close();
        this.socket.close();
    }


    
}