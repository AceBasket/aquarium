import utils.*;
import java.io.File;
import java.io.IOException;

public class UnitTestsCommand {
    public static void main(String[] args) throws IOException, ParserException {
        new UnitTestsCommand().testParserAddFish();
    }
    void testParserAddFish () throws IOException, ParserException {
        String start = "startFish PoissonNain";
        new PromptParserResult(Parse.parserCommand(start));
        assert "STARTFISH" == 
    }
}
