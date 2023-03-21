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

    public View(int port, String address) throws IOException {
        portNumber = port;
        controllerAddress = address;
        socket = new Socket(address, portNumber);
        input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        output = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);
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