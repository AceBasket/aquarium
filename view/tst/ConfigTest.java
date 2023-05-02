import utils.*;
import java.io.File;
import java.io.IOException;

public class ConfigTest {
    public static void main(String[] args) throws IOException, ParserException {
        new ConfigTest().testParserTimeout();
        new ConfigTest().testParserResources();
        new ConfigTest().testParserID();
        new ConfigTest().testParserPort();
        new ConfigTest().testParserIpAddress();
    }

    void testParserIpAddress() throws IOException, ParserException {
        // Pas de ligne indiquand l'adresse IP
        assert "" == Parser.parserIP(new File("./tst/testFilesToParse/0.txt")) : "testParserIpAddress 0 a échoué";
        System.out.println("testParserIpAddress 0 a réussi");
        // Aucune exception à lever
        assert "11.11.11.11" == Parser.parserIP(new File("./tst/testFilesToParse/11.txt"))
                : "testParserIpAddress 11 a échoué";
        System.out.println("testParserIpAddress 11 a réussi");
        // Aucune exception à lever
        assert "255.255.255.255" == Parser.parserIP(new File("./tst/testFilesToParse/255.txt"))
                : "testParserIpAddress 255 a échoué";
        System.out.println("testParserIpAddress 255 a réussi");
        // Adresse IP qui ne comporte pas 4 parties
        assert "" == Parser.parserIP(new File("./tst/testFilesToParse/01.txt")) : "testParserIpAddress 01 a échoué";
        System.out.println("testParserIpAddress 01 a réussi");
        // Adresse IP avec des lettres
        assert "" == Parser.parserIP(new File("./tst/testFilesToParse/02.txt")) : "testParserIpAddress 02 a échoué";
        System.out.println("testParserIpAddress 02 a réussi");
    }

    void testParserTimeout() throws IOException, ParserException {
        // Négatif
        assert 0 == Parser.parserTimeout(new File("./tst/testFilesToParse/0.txt")) : "testParserTimeout 0 a échoué";
        System.out.println("testParserTimeout 0 a réussi");
        // Aucune exception à lever
        assert 2 == Parser.parserTimeout(new File("./tst/testFilesToParse/11.txt")) : "testParserTimeout 11 a échoué";
        System.out.println("testParserTimeout 11 a réussi");
        // Pas de ligne indiquant le timeout
        assert 0 == Parser.parserTimeout(new File("./tst/testFilesToParse/255.txt")) : "testParserTimeout 255 a échoué";
        System.out.println("testParserTimeout 255 a réussi");
        // Contient une lettre
        assert 0 == Parser.parserTimeout(new File("./tst/testFilesToParse/01.txt")) : "testParserTimeout 01 a échoué";
        System.out.println("testParserTimeout 01 a réussi");
        // Acune exception à lever
        assert 25 == Parser.parserTimeout(new File("./tst/testFilesToParse/02.txt")) : "testParserTimeout 02 a échoué";
        System.out.println("testParserTimeout 02 a réussi");
    }

    void testParserResources() throws IOException, ParserException {
        // Aucune exception à lever
        assert "blabla" == Parser.parserResources(new File("./tst/testFilesToParse/0.txt"))
                : "testParserResources 0 a échoué";
        System.out.println("testParserResources 0 a réussi");
        // Pas de ligne indiquand le dossier de ressources
        assert "" == Parser.parserResources(new File("./tst/testFilesToParse/11.txt"))
                : "testParserResources 11 a échoué";
        System.out.println("testParserResources 11 a réussi");
        // Aucune exception à lever
        assert "0toto" == Parser.parserResources(new File("./tst/testFilesToParse/255.txt"))
                : "testParserResources 255 a échoué";
        System.out.println("testParserResources 255 a réussi");
        // Pas de ligne indiquand le dossier de ressources
        assert "" == Parser.parserResources(new File("./tst/testFilesToParse/01.txt"))
                : "testParserResources 01 a échoué";
        System.out.println("testParserResources 01 a réussi");
        // Pas de ligne indiquand le dossier de ressources
        assert "" == Parser.parserResources(new File("./tst/testFilesToParse/02.txt"))
                : "testParserResources 02 a échoué";
        System.out.println("testParserResources 02 a réussi");
    }

    void testParserID() throws IOException, ParserException {
        // Aucune exception à lever
        assert "N2" == Parser.parserID(new File("./tst/testFilesToParse/0.txt")) : "testParserID 0 a échoué";
        System.out.println("testParserID 0 a réussi");
        // Pas de ligne indiquand l'ID
        assert "" == Parser.parserID(new File("./tst/testFilesToParse/11.txt")) : "testParserID 11 a échoué";
        System.out.println("testParserID 11 a réussi");
        // N n'est pas suivi d'un nombre
        assert "" == Parser.parserID(new File("./tst/testFilesToParse/255.txt")) : "testParserID 255 a échoué";
        System.out.println("testParserID 255 a réussi");
        // Pas de N au début
        assert "" == Parser.parserID(new File("./tst/testFilesToParse/01.txt")) : "testParserID 01 a échoué";
        System.out.println("testParserID 01 a réussi");
        // Seulement un N et pas de numéro
        assert "" == Parser.parserID(new File("./tst/testFilesToParse/02.txt")) : "testParserID 02 a échoué";
        System.out.println("testParserID 02 a réussi");
    }

    void testParserPort() throws IOException, ParserException {
        // Well Known Port
        assert 0 == Parser.parserPort(new File("./tst/testFilesToParse/0.txt")) : "testParserPort 0 a échoué";
        System.out.println("testParserPort 0 a réussi");
        // Aucune exception à lever
        assert 34876 == Parser.parserPort(new File("./tst/testFilesToParse/11.txt")) : "testParserPort 11 a échoué";
        System.out.println("testParserPort 11 a réussi");
        // Pas de ligne indiquant le numéro de port
        assert 0 == Parser.parserPort(new File("./tst/testFilesToParse/255.txt")) : "testParserPort 255 a échoué";
        System.out.println("testParserPort 255 a réussi");
        // Dépasse la limite
        assert 0 == Parser.parserPort(new File("./tst/testFilesToParse/01.txt")) : "testParserPort 01 a échoué";
        System.out.println("testParserPort 01 a réussi");
        // Comprend des lettres
        assert 0 == Parser.parserPort(new File("./tst/testFilesToParse/02.txt")) : "testParserPort 02 a échoué";
        System.out.println("testParserPort 02 a réussi");
    }
}