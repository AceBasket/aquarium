import java.io.*;

public class Parse {
    public static void main(String[] argv) throws IOException {
            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
            String cmd = reader.readLine();
            parser(cmd);
    }
    public static void parser(String command) {
        // switch(command) {
        //     case status:

        // }
        if (command.equals("status")) {
            System.out.println("Request : status");
        }
        else if (command.equals("addFish")) {
            System.out.println("Request : addFish");
        }
        else if (command.equals("delFish")) {
            System.out.println("Request : delFish");
        }
        else if (command.equals("startFish")) {
            System.out.println("Request : startFish");
        }
        else {
            System.out.println("This is not a request");
        }
    }
}