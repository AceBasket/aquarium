import utils.Parse;
import java.io.File;
import java.io.IOException;

public class UnitTests {
    public static void main(String[] args) throws IOException {
        // testParserTimeout();
        // testParserResources();
        // testParserID();
        // testParserPort();
        new UnitTests().testParserIpAddress();
    }

    void testParserIpAddress() throws IOException {
        assert "" == Parse.parserIP(new File("./tst/testFilesToParse/0.txt")) : "testParserIpAddress 0 a échoué";
        System.out.println("testParserIpAddress 0 a réussi");
    }
}
