import utils.*;
import java.io.File;
import java.io.IOException;

public class CommandTest {
    public static void main(String[] args) throws IOException, ParserException {
        new CommandTest().testParserAddFish();
    }
    void testParserAddFish () throws IOException, ParserException {
        String start = "startFish PoissonNain";
        ParserResult result = Parser.parse(start);
        assert Parser.PossibleResponses.STARTFISH == result.getFunction() : "testAddFish PoissonNain a échoué";
        System.out.println("testAddFish PoissonNain a réussi");
    }
}
