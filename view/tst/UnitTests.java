import utils.*;
import java.io.File;
import java.io.IOException;

public class UnitTests {
    public static void main(String[] args) throws IOException, ParserException {
        // testParserTimeout();
        // testParserResources();
        // testParserID();
        // testParserPort();
        new UnitTests().testParserIpAddress();
    }

    void testParserIpAddress() throws IOException, ParserException {
        assert "" == Parse.parserIP(new File("./tst/testFilesToParse/0.txt")) : "testParserIpAddress 0 a échoué";
        System.out.println("testParserIpAddress 0 a réussi");
        assert "11.11.11.11" == Parse.parserIP(new File("./tst/testFilesToParse/11.txt")) : "testParserIpAddress 11 a échoué";
        System.out.println("testParserIpAddress 11 a réussi");
        assert "255.255.255.255" == Parse.parserIP(new File("./tst/testFilesToParse/255.txt")) : "testParserIpAddress 255 a échoué";
        System.out.println("testParserIpAddress 255 a réussi");
    }

    void testParserTimeout() throws IOException {

    }
}
