import java.io.*;
import java.net.*;

public class View {
    private int portNumber;
    private String host;
    private Socket socket;
    private BufferedReader input;
    private PrintWriter output;

    public View(int port, String hostAdress) throws UnknownHostException, IOException {
        portNumber = port;
        host = hostAdress;
        socket = new Socket(host, portNumber);
        input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        output = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);
    }

    public static void main(String[] argv) {
        try {

            View view = new View(8080, "0.0.0.0");

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