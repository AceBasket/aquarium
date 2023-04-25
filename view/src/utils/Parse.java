package utils;

import java.io.*;
import java.util.ArrayList;
import java.security.InvalidParameterException;
import java.util.Scanner;


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
            File file = new File("./affichage.cfg");
            try{
                parserIP(file);
                parserID(file);
                parserPort(file);
                parserTimeout(file);
                parserResources(file);
                parserServerResponse("list [PoissonRouge at 90x4,10x4,5] [PoissonClown at 90x4,10x4,5]");
                // parserCommand("addFish PoissonNain at 61x52, 4x3, RandomWAyPoint");
                // System.out.println(parserCommand("addFish PoissonNain at 61x52, 4x3, RandomWAyPoint"));
            } catch(IOException e) {
                System.out.println("Le fichier de configuration est introuvable");
            } catch(ParserException e) {
                System.out.println("Erreur de parsing");
            }
            
    }
    
    public static PromptParserResult parserCommand(String command) {
        ArrayList<String> args = new ArrayList<String>();
        String[] commandSplit = command.split(" : |, |,| |x| at ");
        if (commandSplit[0].equals("status")) {
            return new PromptParserResult(PromptCommandType.STATUS, args );
        }
        else if (commandSplit[0].equals("addFish")) {
            // Vérifier qu'il y a des éléménts après addFish, vérifier que c'est multiple de ... et vérifier la forme des éléments
            for (int i = 1 ; i < commandSplit.length ; i++) {
                args.add(commandSplit[i]);
            }
            return new PromptParserResult(PromptCommandType.ADDFISH, args);
        }
        else if (commandSplit[0].equals("delFish")) {
            if (commandSplit.length >= 2) {
                for (int i = 1 ; i < commandSplit.length ; i++) {
                    String poisson = commandSplit[i].substring(0, 7);
                    if (poisson.equals("Poisson")) {
                        args.add(commandSplit[i]);
                    }
                    else {
                        throw new InvalidParameterException("Le poisson indiqué n'est pas de la forme Poisson*")
                    }
                }
                return new PromptParserResult(PromptCommandType.DELFISH, args);
            }
            else {
                throw new InvalidParameterException("Unknown command");
            }
        }
        else if (commandSplit[0].equals("startFish")) {
            if (commandSplit.length >= 2) {
                for (int i = 1 ; i < commandSplit.length ; i++) {
                    String poisson = commandSplit[i].substring(0, 7);
                    if (poisson.equals("Poisson")) {
                        args.add(commandSplit[i]);
                    }
                    else {
                        throw new InvalidParameterException("Le poisson indiqué n'est pas de la forme Poisson*")
                    }
                }
                return new PromptParserResult(PromptCommandType.STARTFISH, args);
            }
            else {
                throw new InvalidParameterException("Unknown command");
            }
        }
        else {
            throw new InvalidParameterException("Unknown command");
        }
        
    }

    public static ServerResponseParserResult parserServerResponse(String response) throws ParserException {
        ArrayList<String> args = new ArrayList<String>();
        String[] responseSplit = response.split(" : |, |,| \\[|\\] \\[|\\]|x| at | ");
        if (responseSplit[0].equals("NOK")) {
            if (responseSplit.length >= 2) {
                for (int i = 1 ; i < responseSplit.length ; i++) {
                    args.add(responseSplit[i]);
                }
                return new ServerResponseParserResult(PossibleServerResponses.NOK, args);
            }
            else {
                throw new InvalidParameterException("Unknown response");
            }
        }
        else if (responseSplit[0].equals("OK")) {
            // Vérifier qu'il y a des éléments derrière OK et vérifier leur nombre (multiple de ...)
            // Vérifier la forme des éléments "connecté au contrôleur n poissons trouvés Fish name_fish n n n n"
            for (int i = 1 ; i < responseSplit.length ; i++) {
                args.add(responseSplit[i]);
            }
            return new ServerResponseParserResult(PossibleServerResponses.OK, args);
        }
        else if (responseSplit[0].equals("greeting")) {

            String IDN = responseSplit[1].substring(0, 1);
            String IDNb = responseSplit[1].substring(1);
            if (!IDN.equals("N")) {
                throw new InvalidIDException("ID do not begin with N");
            }
            try {
                Integer.parseInt(IDNb);
            } catch (NumberFormatException e) {
                System.out.println("Invalid ID Format");
            }

            return new ServerResponseParserResult(PossibleServerResponses.GREETING, args);
        }
        else if (responseSplit[0].equals("no greeting")) {
            return new ServerResponseParserResult(PossibleServerResponses.NOGREETING, args);
        }
        else if (responseSplit[0].equals("list")) {
            // Vérifier qu'il y a bien des éléments après list
            // Vérifier le nombre d'arguments (multiple de ...), que le premier c'est Poisson* et les suivants sont des numéros
            // Trouver une méthode qui vérifie si c'est des numéros
            for (int i = 1 ; i < responseSplit.length ; i++) {
                args.add(responseSplit[i]);
            }
            return new ServerResponseParserResult(PossibleServerResponses.LISTFISHES, args);
        }
        else if (responseSplit[0].equals("bye")) {
            return new ServerResponseParserResult(PossibleServerResponses.BYE, args);
        }
        else if (responseSplit[0].equals("pong")) {
            if (responseSplit.length != 2) {
                throw new InvalidParameterException("No argument after pong or to much argument");
            }
            else {
                try {
                    Integer.parseInt(responseSplit[1]);
                    args.add(responseSplit[1]);
                } catch (NumberFormatException e) {
                    System.out.println("Invalid Response to ping");
                }
                return new ServerResponseParserResult(PossibleServerResponses.PONG, args);
            }
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

    public static String parserIP(File file) throws IOException, ParserException {
        String IP = parserManager(file, "controller-address");
        String[] IPSplit = IP.split(".");
        if (IPSplit.length != 4) {
            throw new InvalidIPException("Wrong IP format");
        }
        for (int i = 0 ; i < IPSplit.length ; i++) {
            try {
                Integer.parseInt(IPSplit[i]);
            } catch (NumberFormatException e) {
                System.out.println("Wrong IP Format");
            }
        }
        return IP;
    }

    public static String parserID(File file) throws IOException, ParserException {
        //Vérifier que ça commence par N suivi d'un nombre
        String ID = parserManager(file, "id");
        String IDN = ID.substring(0, 1);
        String IDNb = ID.substring(1);
        if (!IDN.equals("N")) {
            throw new InvalidIDException("ID do not begin with N");
        }
        try {
            Integer.parseInt(IDNb);
        } catch (NumberFormatException e) {
            System.out.println("Invalid ID Format");
        }
        return ID;
    }

    public static int parserPort(File file) throws IOException, ParserException {
        try {
            int port = Integer.parseInt(parserManager(file, "controller-port"));
            if (port < 0 | port > 65535) {
                throw new InvalidPortNumberException("Unknown Port Number")
            }
            else if (port < 1024) {
                throw new WellKnownPortException("Well Known Port");
            }
            return port;
        } catch (NumberFormatException e) {
            throw new ParserException("Port given is not a number");
        }
    }

    public static int parserTimeout(File file) throws IOException, ParserException {
        try {
            int timeout = Integer.parseInt(parserManager(file, "display-timeout-value"));
            if (timeout < 0) {
                throw new InvalidTimeoutException("Negative Timeout");
            }
            return timeout;
        } catch (NumberFormatException e) {
            throw new ParserException("Timeout given is not a number");
        }
    }

    public static String parserResources(File file) throws IOException {
        return parserManager(file, "resources");
    }

}