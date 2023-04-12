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
                parserCommand("addFish PoissonNain at 61x52, 4x3, RandomWAyPoint");
                // System.out.println(parserCommand("addFish PoissonNain at 61x52, 4x3, RandomWAyPoint"));
            } catch(FileNotFoundException e) {
                System.out.println("Le fichier de configuration est introuvable");
            }
            
    }
    public static PromptParserResult parserCommand(String command) {
        ArrayList<String> args = new ArrayList<String>();

        String commandAdd = command.split("addFish");
        String wantedDel = "delFish";
        String commandDel = command.split("delFish");
        String wantedStart = "startFish";
        String commandStart = command.split("startFish");

        // arrayRes = new String[10];
        if (command.equals("status")) {
            System.out.println("OK : status");
            return new PromptParserResult(PromptCommandType.STATUS, args )
        }
        else if (commandAdd.equals("addFish")) {
            return new PromptParserResult(PromptCommandType.ADDFISH, args)
            // Comment analyser les léléments entre les virgules avec split() ? Parce que là je vois pas

            // System.out.println("OK : addFish");
        }
        else if (commandDel.equals("delFish")) {
            // System.out.println("OK : delFish");
            // arrayRes[0] = "2";
            // arrayRes[1] = commandDel.substring(wantedDel.length()+1);
            args.add(commandDel.substring(wantedDel.length()+1));
            return new PromptParserResult(PromptCommandType.DELFISH, args);
        }
        else if (commandStart.equals("startFish")) {
            // System.out.println("OK : startFish");
            // arrayRes[0] = "3";
            // arrayRes[1] = commandStart.substring(wantedStart.length()+1);
            args.add(commandStart.substring(wantedStart.length()+1));
            return new PromptParserResult(PromptCommandType.STARTFISH, args);
        }
        else {
            // System.out.println("NOK : commande introuvable");
            // arrayRes[0] = "-1";
            throw new InvalidParameterException("Unknown command");
        }
        
        // return arrayRes;
    }

    public static ServerResponseParserResult parserServerResponse(String response) {
        ArrayList<String> args = new ArrayList<String>();
        String responseNOK = response.split("NOK");
        String responseOK = response.split("OK");
        if (responseNOK.equals("NOK")) {
            // return response.substring(responseNOK.length() + 3); //On retourne l'erreur correspondante
            return new ServerResponseParserResult(PossibleServerResponses.NOK, args);
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

    // public static void parserComm(String sent, String received) {
    //     String hello = "hello";
    //     String greeting = "greeting";
    //     String sentHello = sent.substring(0, hello.length());
    //     String ping = "ping";
    //     String sentPing = sent.substring(0, ping.length());
    //     if (sentHello.equals("hello")) {
    //         String receivedGreeting = received.substring(0, greeting.length());
    //         if (receivedGreeting.equals("greeting")) {
    //             String ID = received.substring(greeting.length()+1);
    //         }
    //         else {
    //             //Renvoyer une erreur
    //         }
    //     }
    //     //Faire les list en réponse à getFishes, ls et getFishesContinuously
    
    //     else if (sent.equals("log out")) {
    //         if (received.equals("bye")) {
    //             //Renvoyer ok
    //         }
    //         else {
    //             //Renvoyer que la déconnexion n'a pas été faite
    //         }
    //     }
    
    //     else if (sentPing.equals("ping")) {
    //         String pong = "pong";
    //         String receivedPong = received.substring(0, pong.length());
    //         if (receivedPong.equals("pong")) {
    //             //Retourner que la connexion est bonne
    //         }
    //         else {
    //             //Renvoyer que problème de connexion
    //         }
    //     }
    
    //     else {
    //         //Retourner que la réponse n'est pas reconnue
    //     }
    // }
}
