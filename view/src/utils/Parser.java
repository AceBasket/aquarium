package utils;

import java.io.*;
import java.util.ArrayList;
import java.security.InvalidParameterException;

public class Parser {

    public enum PossibleResponses {
        GREETING, NOGREETING, OK, NOK, LISTFISHES, BYE, PONG, STATUS, ADDFISH, DELFISH, STARTFISH
    }

    public static void main(String[] argv) throws ParserException, IOException {
        // BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        // String cmd = reader.readLine();
        File file = new File("./affichage.cfg");
        try {
            parserIP(file);
            parserID(file);
            parserPort(file);
            parserTimeout(file);
            parserResources(file);
            // parserServerResponse("list [PoissonRouge at 90x4,10x4,5] [PoissonClown at
            // 90x4,10x4,5]");
            // parserCommand("addFish PoissonNain at 61x52, 4x3, RandomWAyPoint");
            // System.out.println(parserCommand("addFish PoissonNain at 61x52, 4x3,
            // RandomWAyPoint"));
        } catch (IOException e) {
            System.out.println("The config file does not exist");
        } catch (InvalidParameterException e) {
            System.out.println("Parsing error");
        }

    }

    public static ParserResult parse(String command) throws ParserException {
        ArrayList<String> args = new ArrayList<String>();
        String[] responseSplit = command.split(" at | : |, |,| \\[|\\] \\[|\\]|x| ");
        if (responseSplit[0].equals("status")) {
            return new ParserResult(PossibleResponses.STATUS, args);
        } else if (responseSplit[0].equals("addFish")) {
            if (responseSplit.length == 7) {
                String poisson = responseSplit[1].substring(0, 7);
                if (poisson.equals("Poisson")) {
                    args.add(responseSplit[1]);
                    for (int i = 2; i < responseSplit.length - 1; i++) {
                        try {
                            Integer.parseInt(responseSplit[i]);
                        } catch (NumberFormatException e) {
                            System.out.println("Invalid Argument Format");
                        }
                        args.add(responseSplit[i]);
                    }
                    args.add(responseSplit[responseSplit.length - 1]);
                    return new ParserResult(PossibleResponses.ADDFISH, args);
                } else {
                    throw new ParserException("The fish name is not Poisson*");
                }
            } else {
                throw new ParserException("There is not the right number of argument after addFish");
            }

        } else if (responseSplit[0].equals("delFish")) {
            if (responseSplit.length >= 2) {
                for (int i = 1; i < responseSplit.length; i++) {
                    String poisson = responseSplit[i].substring(0, 7);
                    if (poisson.equals("Poisson")) {
                        args.add(responseSplit[i]);
                    } else {
                        throw new ParserException("The fish name is not Poisson*");
                    }
                }
                return new ParserResult(PossibleResponses.DELFISH, args);
            } else {
                throw new ParserException("There is no argument after delFish");
            }
        } else if (responseSplit[0].equals("startFish")) {
            if (responseSplit.length >= 2) {
                for (int i = 1; i < responseSplit.length; i++) {
                    String poisson = responseSplit[i].substring(0, 7);
                    if (poisson.equals("Poisson")) {
                        args.add(responseSplit[i]);
                    } else {
                        throw new ParserException("The fish name is not Poisson*");
                    }
                }
                return new ParserResult(PossibleResponses.STARTFISH, args);
            } else {
                throw new ParserException("There is no argument after startFish");
            }
        } else if (responseSplit[0].equals("NOK")) {
            if (responseSplit.length >= 2) {
                for (int i = 1; i < responseSplit.length; i++) {
                    args.add(responseSplit[i]);
                }
                return new ParserResult(PossibleResponses.NOK, args);
            } else {
                return new ParserResult(PossibleResponses.NOK, args);
            }
        } else if (responseSplit[0].equals("OK")) {
            if (responseSplit.length == 1) {
                return new ParserResult(PossibleResponses.OK, args);
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
                    return new ParserResult(PossibleResponses.OK, args);
                } else if (responseSplit.length % 7 == 0) {
                    for (int i = 1; i < responseSplit.length; i++) {
                        if (i % 7 == 1 && i != 1) {
                            String poisson = responseSplit[i].substring(0, 7);
                            if (!poisson.equals("Poisson")) {
                                throw new ParserException("The fish name is not Poisson*");
                            }
                        } else if (i % 7 == 2 || i % 7 == 3 || i % 7 == 4 || i % 7 == 5) {
                            try {
                                Integer.parseInt(responseSplit[i]);
                            } catch (NumberFormatException e) {
                                System.out.println("Invalid Argument Format");
                            }
                        }
                        args.add(responseSplit[i]);
                    }
                    return new ParserResult(PossibleResponses.OK, args);
                } else {
                    throw new ParserException("There is not the right number of argument after OK");
                }
            }
        } else if (responseSplit[0].equals("greeting")) {

            if (responseSplit.length != 2) {
                throw new ParserException("There is not the right number of argument after greeting");
            } else {
                String IDN = responseSplit[1].substring(0, 1);
                String IDNb = responseSplit[1].substring(1);
                if (!IDN.equals("N")) {
                    throw new ParserException("ID do not begin with N");
                }
                try {
                    Integer.parseInt(IDNb);
                } catch (NumberFormatException e) {
                    System.out.println("Invalid ID Format");
                }

                args.add(responseSplit[1]);
                return new ParserResult(PossibleResponses.GREETING, args);
            }
        } else if (responseSplit[0].equals("no greeting")) {
            return new ParserResult(PossibleResponses.NOGREETING, args);
        } else if (responseSplit[0].equals("list")) {
            if (responseSplit.length % 6 == 1) {
                for (int i = 1; i < responseSplit.length; i++) {
                    if (i % 6 == 1) {
                        String poisson = responseSplit[i].substring(0, 7);
                        if (!poisson.equals("Poisson")) {
                            throw new ParserException("The fish name is not Poisson*");
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
                return new ParserResult(PossibleResponses.LISTFISHES, args);
            } else {
                throw new ParserException("There is not the right number of argument after list");
            }
        } else if (responseSplit[0].equals("bye")) {
            return new ParserResult(PossibleResponses.BYE, args);
        } else if (responseSplit[0].equals("pong")) {
            if (responseSplit.length != 2) {
                throw new ParserException("There is not the right number of argument after pong");
            } else {
                try {
                    Integer.parseInt(responseSplit[1]);
                    args.add(responseSplit[1]);
                } catch (NumberFormatException e) {
                    System.out.println("Invalid Response Format to ping");
                }
                return new ParserResult(PossibleResponses.PONG, args);
            }
        } else {
            throw new ParserException("Unknown response/command");
        }
    }

    public static String parserManager(File file, String wanted) throws ParserException, IOException {
        String result = "";
        try {
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
            result = sb.toString();
        } catch (IOException e) {
            System.out.println("The config file does not exist");
        }
        return result;
    }

    public static String parserIP(File file) throws ParserException, IOException {
        String IP = parserManager(file, "controller-address");
        String[] IPSplit = IP.split(".");
        if (IPSplit.length != 4) {
            throw new ParserException("Wrong IP format");
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
            throw new ParserException("Invalid size of ID");
        } else {
            String IDN = ID.substring(0, 1);
            String IDNb = ID.substring(1);
            if (!IDN.equals("N")) {
                throw new ParserException("ID do not begin with N");
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
                throw new ParserException("Unknown Port Number");
            } else if (port < 1024) {
                throw new ParserException("Well Known Port");
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
                throw new ParserException("Negative Timeout");
            }
            return timeout;
        } catch (NumberFormatException e) {
            throw new ParserException("Timeout given is not a number");
        }
    }

    public static String parserResources(File file) throws IOException, ParserException {
        return parserManager(file, "resources");
    }

}