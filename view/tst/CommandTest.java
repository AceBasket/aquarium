import utils.*;
import java.io.File;
import java.io.IOException;

public class CommandTest {
    public static void main(String[] args) throws IOException, ParserException {
        new CommandTest().testParserAddFish();
    }
    void testParserAddFish () throws IOException, ParserException {
        String start = "startFish PoissonNain";
        PromptParserResult result = Parse.parserCommand(start);
        assert Parse.PromptCommandType.STARTFISH == result.getCommand() : "testAddFish PoissonNain a échoué";
        System.out.println("testAddFish PoissonNain a réussi");
    }
}
