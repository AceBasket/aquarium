package utils;

import java.io.*;
import java.util.ArrayList;
import java.security.InvalidParameterException;

public class Parser {

    public enum PossibleResponses {
        GREETING, NO_GREETING, OK, NOK, LIST_FISHES, BYE, PONG, STATUS, ADD_FISH, DEL_FISH, START_FISH
    }

    public static void main(String[] argv) throws ParserException, IOException {
        File file = new File("./affichage.cfg");
        try {
            parserIP(file);
            parserID(file);
            parserPort(file);
            parserTimeout(file);
            parserResources(file);
        } catch (IOException e) {
            throw new ParserException("The config file does not exist");
        } catch (InvalidParameterException e) {
            throw new ParserException("Parsing error");
        }
    }

    public static ParserResult parse(String command) throws ParserException {
        if (command == null || command.length() == 0) {
            throw new ParserException("Empty command");
        }
        ArrayList<String> args = new ArrayList<String>();
        String[] responseSplit = command.split(" at | : |: |:|, |,| \\[|\\] \\[|\\]|x| ");
        if (responseSplit[0].equals("status")) {
            // STATUS
            return new ParserResult(PossibleResponses.STATUS, args);
        } else if (responseSplit[0].equals("addFish")) {
            // ADDFISH
            if (responseSplit.length == 7) {
                if (responseSplit[1].length() < 7) {
                    throw new ParserException("The fish name is not Poisson*");
                }
                String poisson = responseSplit[1].substring(0, 7);
                if (poisson.equals("Poisson")) {
                    args.add(responseSplit[1]);
                    for (int i = 2; i < responseSplit.length - 1; i++) {
                        try {
                            Integer.parseInt(responseSplit[i]);
                        } catch (NumberFormatException e) {
                            throw new ParserException("Invalid Argument Format");
                        }
                        args.add(responseSplit[i]);
                    }
                    args.add(responseSplit[responseSplit.length - 1]);
                    return new ParserResult(PossibleResponses.ADD_FISH, args);
                } else {
                    throw new ParserException("The fish name is not Poisson*");
                }
            } else {
                throw new ParserException("There is not the right number of argument after addFish");
            }
        } else if (responseSplit[0].equals("delFish")) {
            // DELFISH
            if (responseSplit.length >= 2) {
                for (int i = 1; i < responseSplit.length; i++) {
                    if (responseSplit[i].length() < 7) {
                        throw new ParserException("The fish name is not Poisson*");
                    }
                    String poisson = responseSplit[i].substring(0, 7);
                    if (poisson.equals("Poisson")) {
                        args.add(responseSplit[i]);
                    } else {
                        throw new ParserException("The fish name is not Poisson*");
                    }
                }
                return new ParserResult(PossibleResponses.DEL_FISH, args);
            } else {
                throw new ParserException("There is no argument after delFish");
            }
        } else if (responseSplit[0].equals("startFish")) {
            // STARTFISH
            if (responseSplit.length >= 2) {
                for (int i = 1; i < responseSplit.length; i++) {
                    if (responseSplit[i].length() < 7) {
                        throw new ParserException("The fish name is not Poisson*");
                    }
                    String poisson = responseSplit[i].substring(0, 7);
                    if (poisson.equals("Poisson")) {
                        args.add(responseSplit[i]);
                    } else {
                        throw new ParserException("The fish name is not Poisson*");
                    }
                }
                return new ParserResult(PossibleResponses.START_FISH, args);
            } else {
                throw new ParserException("There is no argument after startFish");
            }
        } else if (responseSplit[0].equals("NOK")) {
            // NOK
            if (responseSplit.length >= 2) {
                for (int i = 1; i < responseSplit.length; i++) {
                    args.add(responseSplit[i]);
                }
                return new ParserResult(PossibleResponses.NOK, args);
            } else {
                return new ParserResult(PossibleResponses.NOK, args);
            }
        } else if (responseSplit[0].equals("OK")) {
            // OK
            if (responseSplit.length == 1) {
                return new ParserResult(PossibleResponses.OK, args);
            } else {
                if (responseSplit.length == 7) {
                    for (int i = 1; i < responseSplit.length; i++) {
                        if (i == 4) {
                            try {
                                Integer.parseInt(responseSplit[i]);
                            } catch (NumberFormatException e) {
                                throw new ParserException("Invalid Argument Format");
                            }
                        }
                        args.add(responseSplit[i]);
                    }
                    return new ParserResult(PossibleResponses.OK, args);
                } else if (responseSplit.length % 7 == 0) {
                    for (int i = 1; i < responseSplit.length; i++) {
                        if (i % 7 == 1 && i != 1) {
                            if (responseSplit[i].length() < 7) {
                                throw new ParserException("The fish name is not Poisson*");
                            }
                            String poisson = responseSplit[i].substring(0, 7);
                            if (!poisson.equals("Poisson")) {
                                throw new ParserException("The fish name is not Poisson*");
                            }
                        } else if (i % 7 == 2 || i % 7 == 3 || i % 7 == 4 || i % 7 == 5) {
                            try {
                                Integer.parseInt(responseSplit[i]);
                            } catch (NumberFormatException e) {
                                throw new ParserException("Invalid Argument Format");
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
            // GREETING
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
                    throw new ParserException("Invalid ID Format");
                }

                args.add(responseSplit[1]);
                return new ParserResult(PossibleResponses.GREETING, args);
            }
        } else if (responseSplit[0].equals("no greeting")) {
            // NO GREETING
            return new ParserResult(PossibleResponses.NO_GREETING, args);
        } else if (responseSplit[0].equals("list")) {
            // LIST
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
                            throw new ParserException("Invalid Argument Format");
                        }
                    }
                    args.add(responseSplit[i]);
                }
                return new ParserResult(PossibleResponses.LIST_FISHES, args);
            } else {
                throw new ParserException("There is not the right number of argument after list");
            }
        } else if (responseSplit[0].equals("bye")) {
            // BYE
            return new ParserResult(PossibleResponses.BYE, args);
        } else if (responseSplit[0].equals("pong")) {
            // PONG
            if (responseSplit.length != 2) {
                throw new ParserException("There is not the right number of argument after pong");
            } else {
                try {
                    Integer.parseInt(responseSplit[1]);
                    args.add(responseSplit[1]);
                } catch (NumberFormatException e) {
                    throw new ParserException("Invalid Argument Format");
                }
                return new ParserResult(PossibleResponses.PONG, args);
            }
        } else {
            // UNKNOWN COMMAND
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
            throw new ParserException("The config file does not exist");
        }
        return result;
    }

    public static String parserIP(File file) throws ParserException, IOException {
        String IP = parserManager(file, "controller-address");
        if (IP.length() == 0) {
            throw new ParserException("No IP");
        }
        String[] IPSplit = IP.split("\\.");
        if (IPSplit.length != 4) {
            throw new ParserException("Wrong IP format");
        }
        for (int i = 0; i < IPSplit.length; i++) {
            try {
                Integer.parseInt(IPSplit[i]);
            } catch (NumberFormatException e) {
                throw new ParserException("IP contains letters");
            }
        }
        return IP;
    }

    public static String parserID(File file) throws IOException, ParserException {
        String ID = parserManager(file, "id");
        if (ID.length() == 0) {
            throw new ParserException("No ID");
        } else if (ID.length() == 1) {
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
                throw new ParserException("Invalid ID Format");
            }
            return ID;
        }
    }

    public static int parserPort(File file) throws IOException, ParserException {
        String portStr = parserManager(file, "controller-port");
        if (portStr.length() == 0) {
            throw new ParserException("No port");
        }
        try {
            int port = Integer.parseInt(portStr);
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
        String timeoutStr = parserManager(file, "display-timeout-value");
        if (timeoutStr.length() == 0) {
            throw new ParserException("No timeout");
        }
        try {
            int timeout = Integer.parseInt(timeoutStr);
            if (timeout < 0) {
                throw new ParserException("Negative Timeout");
            }
            return timeout;
        } catch (NumberFormatException e) {
            throw new ParserException("Timeout given is not a number");
        }
    }

    public static String parserResources(File file) throws IOException, ParserException {
        String resources = parserManager(file, "resources");
        if (resources.length() == 0) {
            throw new ParserException("No resources");
        }
        return resources;
    }

}