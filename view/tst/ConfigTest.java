import utils.*;
import java.io.File;
import java.io.IOException;

public class ConfigTest {
    public static void main(String[] args) throws IOException, ParserException {
        new ConfigTest().testParserTimeout();
        new ConfigTest().testParserPort();
        new ConfigTest().testParserID();
        new ConfigTest().testParserIpAddress();
        new ConfigTest().testParserResources();
    }

    void testParserIpAddress() throws IOException, ParserException {
        try{
            Parser.parserIP(new File("./tst/testFilesToParse/1.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "The config file does not exist" : "testParserIpAddress no file : a échoué";
            System.out.println("testParserIpAddress no file : a réussi");
        }
        // Pas de ligne indiquand l'adresse IP
        try{
            Parser.parserIP(new File("./tst/testFilesToParse/0.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "No IP" : "testParserIpAddress 0 : a échoué";
            System.out.println("testParserIpAddress 0 : a réussi");
        }
        // Aucune exception à lever
        assert "11.11.11.11" == Parser.parserIP(new File("./tst/testFilesToParse/11.txt")) : "testParserIpAddress 11 a échoué";
        System.out.println("testParserIpAddress 11 a réussi");
        // Aucune exception à lever
        assert "255.255.255.255" == Parser.parserIP(new File("./tst/testFilesToParse/255.txt")) : "testParserIpAddress 255 a échoué";
        System.out.println("testParserIpAddress 255 a réussi");
        // Adresse IP qui ne comporte pas 4 parties
        try{
            Parser.parserIP(new File("./tst/testFilesToParse/01.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "Wrong IP Format" : "testParserIpAddress 01 : a échoué";
            System.out.println("testParserIpAddress 01 : a réussi");
        }
        // Adresse IP avec des lettres
        try{
            Parser.parserIP(new File("./tst/testFilesToParse/02.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "IP contains letters" : "testParserIpAddress 02 : a échoué";
            System.out.println("testParserIpAddress 02 : a réussi");
        }
    }

    void testParserTimeout() throws IOException, ParserException {
        try{
            Parser.parserTimeout(new File("./tst/testFilesToParse/1.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "The config file does not exist" : "testParserTimeout no file : a échoué";
            System.out.println("testParserTimeout no file : a réussi");
        }
        // Négatif
        try{
            Parser.parserTimeout(new File("./tst/testFilesToParse/0.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "Negative Timeout" : "testParserTimeout 0 : a échoué";
            System.out.println("testParserTimeout 0 : a réussi");
        }
        // Aucune exception à lever
        assert 2 == Parser.parserTimeout(new File("./tst/testFilesToParse/11.txt")) : "testParserTimeout 11 : a échoué";
        System.out.println("testParserTimeout 11 : a réussi");
        // Pas de ligne indiquant le timeout
        try{
            Parser.parserTimeout(new File("./tst/testFilesToParse/255.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "No timeout" : "testParserTimeout255 : a échoué";
            System.out.println("testParserTimeout 255 : a réussi");
        }
        // Contient une lettre
        try{
            Parser.parserTimeout(new File("./tst/testFilesToParse/01.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "Timeout given is not a number" : "testParserTimeout 01 : a échoué";
            System.out.println("testParserTimeout 01 : a réussi");
        }
        // Acune exception à lever
        assert 25 == Parser.parserTimeout(new File("./tst/testFilesToParse/02.txt")) : "testParserTimeout 02 : a échoué";
        System.out.println("testParserTimeout 02 a réussi");
    }

    void testParserResources() throws IOException, ParserException {
        try{
            Parser.parserResources(new File("./tst/testFilesToParse/1.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "The config file does not exist" : "testParserResources no file : a échoué";
            System.out.println("testParserResources no file : a réussi");
        }
        // Pas de ligne indiquand le dossier de ressources
        try{
            Parser.parserResources(new File("./tst/testFilesToParse/11.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "No resources" : "testParserResources 11 : a échoué";
            System.out.println("testParserIpAddress 11 : a réussi");
        }
        // Aucune exception à lever
        assert "blabla" == Parser.parserResources(new File("./tst/testFilesToParse/0.txt")) : "testParserResources 0 a échoué";
        System.out.println("testParserResources 0 a réussi");
        // Aucune exception à lever
        assert "0toto" == Parser.parserResources(new File("./tst/testFilesToParse/255.txt")) : "testParserResources 255 a échoué";
        System.out.println("testParserResources 255 a réussi");
        // Pas de ligne indiquand le dossier de ressources
        try{
            Parser.parserResources(new File("./tst/testFilesToParse/01.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "No resources" : "testParserResources 01 : a échoué";
            System.out.println("testParserIpAddress 01 : a réussi");
        }
        // Pas de ligne indiquand le dossier de ressources
        try{
            Parser.parserResources(new File("./tst/testFilesToParse/02.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "No resources" : "testParserResources 02 : a échoué";
            System.out.println("testParserIpAddress 02 : a réussi");
        }
    }

    void testParserID() throws IOException, ParserException {
        try{
            Parser.parserID(new File("./tst/testFilesToParse/1.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "The config file does not exist" : "testParserID no file : a échoué";
            System.out.println("testParserID no file : a réussi");
        }
        // Pas de ligne indiquand l'ID
        try{
            Parser.parserID(new File("./tst/testFilesToParse/11.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "No ID" : "testParserID 11 : a échoué";
            System.out.println("testParserID 11 : a réussi");
        }
        // Aucune exception à lever
        assert "N2" == Parser.parserID(new File("./tst/testFilesToParse/0.txt")) : "testParserID 0 : a échoué";
        System.out.println("testParserID 0 : a réussi");
        // N n'est pas suivi d'un nombre
        try{
            Parser.parserID(new File("./tst/testFilesToParse/255.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "Invalid ID Format" : "testParserID 255 : a échoué";
            System.out.println("testParserID 255 : a réussi");
        }
        // Pas de N au début
        try{
            Parser.parserID(new File("./tst/testFilesToParse/01.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "ID do not begin with N" : "testParserID 01 : a échoué";
            System.out.println("testParserID 01 : a réussi");
        }
        // Seulement un N et pas de numéro
        try{
            Parser.parserID(new File("./tst/testFilesToParse/02.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "Invalid size of ID" : "testParserID 02 : a échoué";
            System.out.println("testParserID 02 : a réussi");
        }
    }

    void testParserPort() throws IOException, ParserException {
        try{
            Parser.parserPort(new File("./tst/testFilesToParse/1.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "The config file does not exist" : "testParserPort no file : a échoué";
            System.out.println("testParserPort no file : a réussi");
        }
        // Well Known Port
        try{
            Parser.parserPort(new File("./tst/testFilesToParse/0.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "Well Known Port" : "testParserPort 0 : a échoué";
            System.out.println("testParserPort 0 : a réussi");
        }
        // Aucune exception à lever
        assert 34876 == Parser.parserPort(new File("./tst/testFilesToParse/11.txt")) : "testParserPort 11 a échoué";
        System.out.println("testParserPort 11 a réussi");
        // Pas de ligne indiquant le numéro de port
        try{
            Parser.parserPort(new File("./tst/testFilesToParse/255.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "No port" : "testParserPort 255 : a échoué";
            System.out.println("testParserPort 255 : a réussi");
        }
        // Dépasse la limite
        try{
            Parser.parserPort(new File("./tst/testFilesToParse/01.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "Unknown Port Number" : "testParserPort 01 : a échoué";
            System.out.println("testParserPort 01 : a réussi");
        }
        // Comprend des lettres
        try{
            Parser.parserPort(new File("./tst/testFilesToParse/02.txt"));
        } catch (ParserException e) {
            assert e.getMessage() == "Port given is not a number" : "testParserPort 02 : a échoué";
            System.out.println("testParserPort 02 : a réussi");
        }
    }
}
