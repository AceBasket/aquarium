package utils;

import java.io.File;
import java.io.IOException;

public class ConfigTest {
    public static void main(String[] args) throws IOException, ParserException {
        boolean missingAssertion = false;
        assert missingAssertion = true;

        if (!missingAssertion) {
            System.out.println("Option -ea needed");
            return;
        }
        ConfigTest test = new ConfigTest();
        System.out.print('.');
        test.testParserIpAddress();
        System.out.print('.');
        test.testParserTimeout();
        System.out.print('.');
        test.testParserPort();
        System.out.print('.');
        test.testParserID();
        System.out.print('.');
        test.testParserResources();
        System.out.println(" OK");

    }

    void testParserIpAddress() throws IOException, ParserException {
        try {
            Parser.parserIP(new File("./tst/testFilesToParse/1.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("The config file does not exist") : "testParserIpAddress no file : a échoué";
        }
        // Pas de ligne indiquand l'adresse IP
        try {
            Parser.parserIP(new File("./tst/testFilesToParse/0.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("No IP") : "testParserIpAddress 0 : a échoué";
        }
        // Aucune exception à lever
        assert Parser.parserIP(new File("./tst/testFilesToParse/11.txt")).equals("11.11.11.11")
                : "testParserIpAddress 11 a échoué";
        // Aucune exception à lever
        assert Parser.parserIP(new File("./tst/testFilesToParse/255.txt")).equals("255.255.255.255")
                : "testParserIpAddress 255 a échoué";
        // Adresse IP avec des lettres
        try {
            Parser.parserIP(new File("./tst/testFilesToParse/02.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("IP contains letters") : "testParserIpAddress 02 : a échoué";
        }
        // Adresse IP qui ne comporte pas 4 parties
        try {
            Parser.parserIP(new File("./tst/testFilesToParse/01.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("Wrong IP format") : "testParserIpAddress 01 : a échoué";
        }
    }

    void testParserTimeout() throws IOException, ParserException {
        try {
            Parser.parserTimeout(new File("./tst/testFilesToParse/1.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("The config file does not exist") : "testParserTimeout no file : a échoué";
        }
        // Négatif
        try {
            Parser.parserTimeout(new File("./tst/testFilesToParse/0.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("Negative Timeout") : "testParserTimeout 0 : a échoué";
        }
        // Aucune exception à lever
        assert 2 == Parser.parserTimeout(new File("./tst/testFilesToParse/11.txt")) : "testParserTimeout 11 : a échoué";
        // Pas de ligne indiquant le timeout
        try {
            Parser.parserTimeout(new File("./tst/testFilesToParse/255.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("No timeout") : "testParserTimeout255 : a échoué";
        }
        // Contient une lettre
        try {
            Parser.parserTimeout(new File("./tst/testFilesToParse/01.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("Timeout given is not a number") : "testParserTimeout 01 : a échoué";
        }
        // Acune exception à lever
        assert 25 == Parser.parserTimeout(new File("./tst/testFilesToParse/02.txt"))
                : "testParserTimeout 02 : a échoué";
    }

    void testParserResources() throws IOException, ParserException {
        try {
            Parser.parserResources(new File("./tst/testFilesToParse/1.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("The config file does not exist") : "testParserResources no file : a échoué";
        }
        // Pas de ligne indiquand le dossier de ressources
        try {
            Parser.parserResources(new File("./tst/testFilesToParse/11.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("No resources") : "testParserResources 11 : a échoué";
        }
        // Aucune exception à lever
        assert Parser.parserResources(new File("./tst/testFilesToParse/0.txt")).equals("blabla")
                : "testParserResources 0 a échoué";
        // Aucune exception à lever
        assert Parser.parserResources(new File("./tst/testFilesToParse/255.txt")).equals("0toto")
                : "testParserResources 255 a échoué";
        // Pas de ligne indiquand le dossier de ressources
        try {
            Parser.parserResources(new File("./tst/testFilesToParse/01.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("No resources") : "testParserResources 01 : a échoué";
        }
        // Pas de ligne indiquand le dossier de ressources
        try {
            Parser.parserResources(new File("./tst/testFilesToParse/02.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("No resources") : "testParserResources 02 : a échoué";
        }
    }

    void testParserID() throws IOException, ParserException {
        try {
            Parser.parserID(new File("./tst/testFilesToParse/1.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("The config file does not exist") : "testParserID no file : a échoué";
        }
        // Pas de ligne indiquand l'ID
        try {
            Parser.parserID(new File("./tst/testFilesToParse/11.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("No ID") : "testParserID 11 : a échoué";
        }
        // Aucune exception à lever
        assert Parser.parserID(new File("./tst/testFilesToParse/0.txt")).equals("N2") : "testParserID 0 : a échoué";
        // N n'est pas suivi d'un nombre
        try {
            Parser.parserID(new File("./tst/testFilesToParse/255.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("Invalid ID Format") : "testParserID 255 : a échoué";
        }
        // Pas de N au début
        try {
            Parser.parserID(new File("./tst/testFilesToParse/01.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("ID do not begin with N") : "testParserID 01 : a échoué";
        }
        // Seulement un N et pas de numéro
        try {
            Parser.parserID(new File("./tst/testFilesToParse/02.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("Invalid size of ID") : "testParserID 02 : a échoué";
        }
    }

    void testParserPort() throws IOException, ParserException {
        try {
            Parser.parserPort(new File("./tst/testFilesToParse/1.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("The config file does not exist") : "testParserPort no file : a échoué";
        }
        // Well Known Port
        try {
            Parser.parserPort(new File("./tst/testFilesToParse/0.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("Well Known Port") : "testParserPort 0 : a échoué";
        }
        // Aucune exception à lever
        assert 34876 == Parser.parserPort(new File("./tst/testFilesToParse/11.txt")) : "testParserPort 11 a échoué";
        // Pas de ligne indiquant le numéro de port
        try {
            Parser.parserPort(new File("./tst/testFilesToParse/255.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("No port") : "testParserPort 255 : a échoué";
        }
        // Dépasse la limite
        try {
            Parser.parserPort(new File("./tst/testFilesToParse/01.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("Unknown Port Number") : "testParserPort 01 : a échoué";
        }
        // Comprend des lettres
        try {
            Parser.parserPort(new File("./tst/testFilesToParse/02.txt"));
        } catch (ParserException e) {
            assert e.getMessage().equals("Port given is not a number") : "testParserPort 02 : a échoué";
        }
    }
}
