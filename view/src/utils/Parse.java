// package utils;

import java.io.*;

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
    public static String[] parserCommand(String command) {
        String commandAdd = command.split("addFish");
        String wantedDel = "delFish";
        String commandDel = command.split("delFish");
        String wantedStart = "startFish";
        String commandStart = command.split("startFish");

        String[] arrayRes;
        arrayRes = new String[10];
        if (command.equals("status")) {
            System.out.println("OK : status");
            arrayRes[0] = "0";
        }
        else if (commandAdd.equals("addFish")) {
            arrayRes[0] = "1";
            // Comment analyser les léléments entre les virgules avec split() ? Parce que là je vois pas

            // System.out.println("OK : addFish");
        }
        else if (commandDel.equals("delFish")) {
            // System.out.println("OK : delFish");
            arrayRes[0] = "2";
            arrayRes[1] = commandDel.substring(wantedDel.length()+1);
        }
        else if (commandStart.equals("startFish")) {
            // System.out.println("OK : startFish");
            arrayRes[0] = "3";
            arrayRes[1] = commandStart.substring(wantedStart.length()+1);
        }
        else {
            // System.out.println("NOK : commande introuvable");
            arrayRes[0] = "-1";
        }
        
        return arrayRes;
    }

    public static String parserServerResponse(String response) {
        String responseNOK = response.split("NOK");
        String responseOK = response.split("OK");
        if (responseNOK.equals("NOK")) {
            return response.substring(responseNOK.length() + 3); //On retourne l'erreur correspondante
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
