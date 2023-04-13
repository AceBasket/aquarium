// package utils;

import java.io.*;
import java.util.ArrayList;
import java.security.InvalidParameterException;



class PromptParserResult {
    private PromptCommandType command;
    private ArrayList<String> args;

    public PromptParserResult(PromptCommandType command, ArrayList<String> args) {
        this.command = command;
        this.args = args;
    }
    
    public PromptCommandType getCommand() {
        return command;
    }

    public ArrayList<String> getArgs() {
        return args;
    }
}

class ServerResponseParserResult {
    private PossibleServerResponses response;
    private ArrayList<String> args;

    public ServerResponseParserResult(PossibleServerResponses response, ArrayList<String> args) {
        this.response = response;
        this.args = args;
    }
    
    public PossibleServerResponses getResponse() {
        return response;
    }

    public ArrayList<String> getArgs() {
        return args;
    }
}

enum PromptCommandType {
    STATUS, ADDFISH, DELFISH, STARTFISH
}

enum PossibleServerResponses {
    GREETING, NOGREETING, OK, NOK, LISTFISHES, BYE, PONG
}


public class Parse {
    public static void main(String[] argv) throws IOException {
            // BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
            // String cmd = reader.readLine();
            File file = new File("affichage.cfg");
            try{
                parserIP(file);
                parserID(file);
                parserPort(file);
                parserTimeout(file);
                parserResources(file);
                parserServerResponse("list [PoissonRouge at 90x4,10x4,5] [PoissonClown at 90x4,10x4,5]");
                // parserCommand("addFish PoissonNain at 61x52, 4x3, RandomWAyPoint");
                // System.out.println(parserCommand("addFish PoissonNain at 61x52, 4x3, RandomWAyPoint"));
            } catch(FileNotFoundException e) {
                System.out.println("Le fichier de configuration est introuvable");
            }
            
    }
    
    public static PromptParserResult parserCommand(String command) {
        ArrayList<String> args = new ArrayList<String>();
        String[] commandSplit = command.split(" : |, |,| |x| at ");
        if (commandSplit[0].equals("status")) {
            return new PromptParserResult(PromptCommandType.STATUS, args );
        }
        else if (commandSplit[0].equals("addFish")) {
            for (int i = 1 ; i < commandSplit.length ; i++) {
                args.add(commandSplit[i]);
            }
            return new PromptParserResult(PromptCommandType.ADDFISH, args);
        }
        else if (commandSplit[0].equals("delFish")) {
            if (commandSplit.length >= 2) {
                args.add(commandSplit[1]);
                return new PromptParserResult(PromptCommandType.DELFISH, args);
            }
            else {
                throw new InvalidParameterException("Unknown response");
            }
        }
        else if (commandSplit[0].equals("startFish")) {
            if (commandSplit.length >= 2) {
                args.add(commandSplit[1]);
                return new PromptParserResult(PromptCommandType.STARTFISH, args);
            }
            else {
                throw new InvalidParameterException("Unknown response");
            }
        }
        else {
            throw new InvalidParameterException("Unknown command");
        }
        
    }

    public static ServerResponseParserResult parserServerResponse(String response) {
        ArrayList<String> args = new ArrayList<String>();
        String[] responseSplit = response.split(" : |, |,| \\[|\\] \\[|\\]|x| at | ");
        // for (int i = 0 ; i < responseSplit.length ; i++) {
        //     System.out.println(responseSplit[i]);
        // }
        if (responseSplit[0].equals("NOK")) {
            if (responseSplit.length >= 2) {
                args.add(responseSplit[1]);
                return new ServerResponseParserResult(PossibleServerResponses.NOK, args);
            }
            else {
                throw new InvalidParameterException("Unknown response");
            }
        }
        else if (responseSplit[0].equals("OK")) {
            for (int i = 1 ; i < responseSplit.length ; i++) {
                args.add(responseSplit[i]);
            }
            return new ServerResponseParserResult(PossibleServerResponses.OK, args);
        }
        else if (responseSplit[0].equals("greeting")) {
            args.add(responseSplit[1]);
            return new ServerResponseParserResult(PossibleServerResponses.GREETING, args);
        }
        else if (responseSplit[0].equals("no greeting")) {
            return new ServerResponseParserResult(PossibleServerResponses.NOGREETING, args);
        }
        else if (responseSplit[0].equals("list")) {
            for (int i = 1 ; i < responseSplit.length ; i++) {
                args.add(responseSplit[i]);
            }
            return new ServerResponseParserResult(PossibleServerResponses.LISTFISHES, args);
        }
        else if (responseSplit[0].equals("bye")) {
            return new ServerResponseParserResult(PossibleServerResponses.BYE, args);
        }
        else if (responseSplit[0].equals("pong")) {
            args.add(responseSplit[1]);
            return new ServerResponseParserResult(PossibleServerResponses.PONG, args);
        }
        else {
            throw new InvalidParameterException("Unknown response");
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