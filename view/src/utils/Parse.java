package utils;

import java.io.*;
import java.util.ArrayList;
import java.security.InvalidParameterException;

public class Parse {
    public enum PromptCommandType {
        STATUS, ADDFISH, DELFISH, STARTFISH
    }

    public enum PossibleServerResponses {
        GREETING, NOGREETING, OK, NOK, LISTFISHES, BYE, PONG
    }

    public static void main(String[] argv) throws IOException {
        // BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        // String cmd = reader.readLine();
        File file = new File("./affichage.cfg");
        try {
            parserIP(file);
            parserID(file);
            parserPort(file);
            parserTimeout(file);
            parserResources(file);
            parserServerResponse("list [PoissonRouge at 90x4,10x4,5] [PoissonClown at 90x4,10x4,5]");
            // parserCommand("addFish PoissonNain at 61x52, 4x3, RandomWAyPoint");
            // System.out.println(parserCommand("addFish PoissonNain at 61x52, 4x3,
            // RandomWAyPoint"));
        } catch (IOException e) {
            System.out.println("Le fichier de configuration est introuvable");
        } catch (ParserException e) {
            System.out.println("Erreur de parsing");
        }

    }

    public static PromptParserResult parserCommand(String command) {
        ArrayList<String> args = new ArrayList<String>();
        String[] commandSplit = command.split(" : |, |,| |x| at ");
        if (commandSplit[0].equals("status")) {
            return new PromptParserResult(PromptCommandType.STATUS, args);
        } else if (commandSplit[0].equals("addFish")) {
            if (commandSplit.length % 6 == 1) {
                String poisson = commandSplit[1].substring(0, 7);
                if (poisson.equals("Poisson")) {
                    for (int i = 2; i < commandSplit.length - 1; i++) {
                        try {
                            Integer.parseInt(commandSplit[i]);
                        } catch (NumberFormatException e) {
                            System.out.println("Invalid Argument Format");
                        }
                        args.add(commandSplit[i]);
                    }
                    args.add(commandSplit[commandSplit.length - 1]);
                    return new PromptParserResult(PromptCommandType.ADDFISH, args);
                } else {
                    throw new InvalidParameterException("Le poisson indiqué n'est pas de la forme Poisson*");
                }
            } else {
                throw new InvalidParameterException("Pas le bon nombre d'arguments après addFish");
            }

        } else if (commandSplit[0].equals("delFish")) {
            if (commandSplit.length >= 2) {
                for (int i = 1; i < commandSplit.length; i++) {
                    String poisson = commandSplit[i].substring(0, 7);
                    if (poisson.equals("Poisson")) {
                        args.add(commandSplit[i]);
                    } else {
                        throw new InvalidParameterException("Le poisson indiqué n'est pas de la forme Poisson*");
                    }
                }
                return new PromptParserResult(PromptCommandType.DELFISH, args);
            } else {
                throw new InvalidParameterException("Unknown command");
            }
        } else if (commandSplit[0].equals("startFish")) {
            if (commandSplit.length >= 2) {
                for (int i = 1; i < commandSplit.length; i++) {
                    String poisson = commandSplit[i].substring(0, 7);
                    if (poisson.equals("Poisson")) {
                        args.add(commandSplit[i]);
                    } else {
                        throw new InvalidParameterException("Le poisson indiqué n'est pas de la forme Poisson*");
                    }
                }
                return new PromptParserResult(PromptCommandType.STARTFISH, args);
            } else {
                throw new InvalidParameterException("Unknown command");
            }
        } else {
            throw new InvalidParameterException("Unknown command");
        }

    }

    public static ServerResponseParserResult parserServerResponse(String response) throws ParserException {
        ArrayList<String> args = new ArrayList<String>();
        String[] responseSplit = response.split(" : |, |,| \\[|\\] \\[|\\]|x| at | ");
        if (responseSplit[0].equals("NOK")) {
            if (responseSplit.length >= 2) {
                for (int i = 1; i < responseSplit.length; i++) {
                    args.add(responseSplit[i]);
                }
                return new ServerResponseParserResult(PossibleServerResponses.NOK, args);
            } else {
                throw new InvalidParameterException("Unknown response");
            }
        } else if (responseSplit[0].equals("OK")) {
            if (responseSplit.length == 1) {
                return new ServerResponseParserResult(PossibleServerResponses.OK, args);
            } else {
                if (responseSplit.length == 7) {
                    for (int i = 1; i < responseSplit.length; i++) {
                        if (i == 4) {
                            try {
                                Integer.parseInt(responseSplit[i]);
                            } catch (NumberFormatException e) {
                                System.out.println("Invalid Argument Format");
                            }
                        }
                        args.add(responseSplit[i]);
                    }
                    return new ServerResponseParserResult(PossibleServerResponses.OK, args);
                } else if (responseSplit.length % 7 == 7) {
                    for (int i = 1; i < responseSplit.length; i++) {
                        if (i % 7 == 1) {
                            String poisson = responseSplit[i].substring(0, 7);
                            if (!poisson.equals("Poisson")) {
                                throw new InvalidParameterException("Invalid Fish Format");
                            }
                        } else if (i == 4 || i % 7 == 2 || i % 7 == 3 || i % 7 == 4 || i % 7 == 5) {
                            try {
                                Integer.parseInt(responseSplit[i]);
                            } catch (NumberFormatException e) {
                                System.out.println("Invalid Argument Format");
                            }
                        }
                        args.add(responseSplit[i]);
                    }
                    return new ServerResponseParserResult(PossibleServerResponses.OK, args);
                } else {
                    throw new InvalidParameterException("Pas le bon nombre d'arguments");
                }
            }
        } else if (responseSplit[0].equals("greeting")) {

            if (responseSplit.length != 2) {
                throw new InvalidParameterException("No argument after greeting or to much argument");
            } else {
                String IDN = responseSplit[1].substring(0, 1);
                String IDNb = responseSplit[1].substring(1);
                if (!IDN.equals("N")) {
                    throw new InvalidParameterException("ID do not begin with N");
                }
                try {
                    Integer.parseInt(IDNb);
                } catch (NumberFormatException e) {
                    System.out.println("Invalid ID Format");
                }

                args.add(responseSplit[1]);
                return new ServerResponseParserResult(PossibleServerResponses.GREETING, args);
            }
        } else if (responseSplit[0].equals("no greeting")) {
            return new ServerResponseParserResult(PossibleServerResponses.NOGREETING, args);
        } else if (responseSplit[0].equals("list")) {
            if (responseSplit.length % 6 == 1) {
                for (int i = 1; i < responseSplit.length; i++) {
                    if (i % 6 == 1) {
                        String poisson = responseSplit[i].substring(0, 7);
                        if (!poisson.equals("Poisson")) {
                            throw new InvalidParameterException("Invalid Argument Format");
                        }
                    } else {
                        try {
                            Integer.parseInt(responseSplit[i]);
                        } catch (NumberFormatException e) {
                            System.out.println("Invalid Argument Format");
                        }
                    }
                    args.add(responseSplit[i]);
                }
                return new ServerResponseParserResult(PossibleServerResponses.LISTFISHES, args);
            } else {
                throw new InvalidParameterException("Invalid number of arguments");
            }
        } else if (responseSplit[0].equals("bye")) {
            return new ServerResponseParserResult(PossibleServerResponses.BYE, args);
        } else if (responseSplit[0].equals("pong")) {
            if (responseSplit.length != 2) {
                throw new InvalidParameterException("No argument after pong or to much argument");
            } else {
                try {
                    Integer.parseInt(responseSplit[1]);
                    args.add(responseSplit[1]);
                } catch (NumberFormatException e) {
                    System.out.println("Invalid Response to ping");
                }
                return new ServerResponseParserResult(PossibleServerResponses.PONG, args);
            }
        } else {
            throw new InvalidParameterException("Unknown response");
        }
    }

    public static String parserManager(File file, String wanted) throws IOException {
        FileReader fr = new FileReader(file);
        BufferedReader br = new BufferedReader(fr);
        StringBuffer sb = new StringBuffer();
        String line;
        String line2 = "";
        while ((line = br.readLine()) != null) {
            if (line.length() > wanted.length()) {
                line2 = line.substring(0, wanted.length());
                if (line2.equals(wanted)) {
                    sb.append(line.substring(wanted.length() + 3));
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
            throw new InvalidParameterException("Wrong IP format");
        }
        for (int i = 0; i < IPSplit.length; i++) {
            try {
                Integer.parseInt(IPSplit[i]);
            } catch (NumberFormatException e) {
                System.out.println("Wrong IP Format");
            }
        }
        return IP;
    }

    public static String parserID(File file) throws IOException, ParserException {
        String ID = parserManager(file, "id");
        if (ID.length() < 2) {
            throw new InvalidParameterException("Invalid size of ID");
        } else {
            String IDN = ID.substring(0, 1);
            String IDNb = ID.substring(1);
            if (!IDN.equals("N")) {
                throw new InvalidParameterException("ID do not begin with N");
            }
            try {
                Integer.parseInt(IDNb);
            } catch (NumberFormatException e) {
                System.out.println("Invalid ID Format");
            }
            return ID;
        }
    }

    public static int parserPort(File file) throws IOException, ParserException {
        try {
            int port = Integer.parseInt(parserManager(file, "controller-port"));
            if (port < 0 | port > 65535) {
                throw new InvalidParameterException("Unknown Port Number");
            } else if (port < 1024) {
                throw new InvalidParameterException("Well Known Port");
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
                throw new InvalidParameterException("Negative Timeout");
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