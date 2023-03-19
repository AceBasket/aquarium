import java.io.*;

public class Parse {
    public static void main(String[] argv) throws IOException {
            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
            String cmd = reader.readLine();
            parserCommand(cmd);
            File file = new File("affichage.cfg");
            parserIP(file);
            parserID(file);
            parserPort(file);
            parserTimeout(file);
            parserResources(file);
    }
    public static void parserCommand(String command) {
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

    public static String parserManager(File file, String wanted) throws IOException {
        FileReader fr = new FileReader(file);
        BufferedReader br = new BufferedReader(fr);
        StringBuffer sb = new StringBuffer();
        String line;
        String line2 = "";
        while((line = br.readLine()) != null)
        {
            if(line.length() > wanted.length()) {
                line2 = line.substring(0, wanted.length());
                if(line2.equals(wanted)) {
                    sb.append(line.substring(wanted.length()+3));
                    break;
                }
            }
        }
        fr.close();
        return sb.toString();
    }

    public static String parserIP(File file) throws IOException {
        return parserManager(file, "controller-address");
    }

    public static String parserID(File file) throws IOException {
        return parserManager(file, "id");
    }

    public static int parserPort(File file) throws IOException {
        return Integer.parseInt(parserManager(file, "controller-port"));
    }

    public static int parserTimeout(File file) throws IOException {
        return Integer.parseInt(parserManager(file, "display-timeout-value"));
    }

    public static String parserResources(File file) throws IOException {
        return parserManager(file, "resources");
    }
}
