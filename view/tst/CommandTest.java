import utils.*;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

public class CommandTest {
    public static void main(String[] args) throws IOException, ParserException {
        System.out.println("======STARTFISH======");
        new CommandTest().testParserStartFish();
        System.out.println("======DELFISH======");
        new CommandTest().testParserDelFish();
    }
    void testParserStartFish () throws IOException, ParserException {
        String start = "startFish PoissonNain";
        ParserResult result = Parser.parse(start);
        ArrayList<String> arguments = result.getArgs();
        assert Parser.PossibleResponses.STARTFISH == result.getFunction() : "testStartFish PoissonNain : fonction a échoué";
        System.out.println("testStartFish PoissonNain : fonction a réussi");
        assert arguments.size() == 1 : "testStartFish PoissonNain : args size a échoué";
        System.out.println("testStartFish PoissonNain : args size a réussi");
        assert arguments.contains("PoissonNain") && arguments.indexOf("PoissonNain") == 0 : "testStartFish PoissonNain : argument a échoué";
        System.out.println("testStartFish PoissonNain : argument a réussi");

        start = "startFish PoissonRouge PoissonNain";
        result = Parser.parse(start);
        arguments = result.getArgs();
        assert Parser.PossibleResponses.STARTFISH == result.getFunction() : "testStartFish PoissonRouge PoissonNain : fonction a échoué";
        System.out.println("testStartFish PoissonRouge PoissonNain : fonction a réussi");
        assert arguments.size() == 2 : "testStartFish PoissonRouge PoissonNain : args size a échoué";
        System.out.println("testStartFish PoissonRouge PoissonNain : args size a réussi");
        assert arguments.contains("PoissonRouge") && arguments.indexOf("PoissonRouge") == 0 : "testStartFish PoissonRouge PoissonNain : argument1 a échoué";
        System.out.println("testStartFish PoissonRouge PoissonNain : argument1 a réussi");
        assert arguments.contains("PoissonNain") && arguments.indexOf("PoissonNain") == 1 : "testStartFish PoissonRouge PoissonNain : argument2 a échoué";
        System.out.println("testStartFish PoissonRouge PoissonNain : argument2 a réussi");

        start = "startFish";
        try{
            Parser.parse(start);
        } catch (ParserException e) {
            assert e.getMessage() == "There is no argument after startFish" : "testStartFish : a échoué";
            System.out.println("testStartFish : a réussi");
        }

        start = "startFish Rouge";
        try{
            Parser.parse(start);
        } catch (ParserException e) {
            assert e.getMessage() == "The fish name is not Poisson*" : "testStartFish Rouge : a échoué";
            System.out.println("testStartFish Rouge : a réussi");
        }

        start = "startFish PoissonNain RougeGorge";
        try{
            Parser.parse(start);
        } catch (ParserException e) {
            assert e.getMessage() == "The fish name is not Poisson*" : "testStartFish PoissonNain RougeGorge : a échoué";
            System.out.println("testStartFish PoissonNain RougeGorge : a réussi");
        }

        start = "startFish Rouge PoissonNain";
        try{
            Parser.parse(start);
        } catch (ParserException e) {
            assert e.getMessage() == "The fish name is not Poisson*" : "testStartFish Rouge PoissonNain : a échoué";
            System.out.println("testStartFish Rouge PoissonNain : a réussi");
        }
    }

    void testParserDelFish () throws IOException, ParserException {
        String del = "delFish PoissonNain";
        ParserResult result = Parser.parse(del);
        ArrayList<String> arguments = result.getArgs();
        assert Parser.PossibleResponses.DELFISH == result.getFunction() : "testDelFish PoissonNain : fonction a échoué";
        System.out.println("testDelFish PoissonNain : fonction a réussi");
        assert arguments.size() == 1 : "testDelFish PoissonNain : args size a échoué";
        System.out.println("testDelFish PoissonNain : args size a réussi");
        assert arguments.contains("PoissonNain") && arguments.indexOf("PoissonNain") == 0 : "testDelFish PoissonNain : argument a échoué";
        System.out.println("testDelFish PoissonNain : argument a réussi");

        del = "delFish PoissonRouge PoissonNain";
        result = Parser.parse(del);
        arguments = result.getArgs();
        assert Parser.PossibleResponses.DELFISH == result.getFunction() : "testDelFish PoissonRouge PoissonNain : fonction a échoué";
        System.out.println("testDelFish PoissonRouge PoissonNain : fonction a réussi");
        assert arguments.size() == 2 : "testDelFish PoissonRouge PoissonNain : args size a échoué";
        System.out.println("testDelFish PoissonRouge PoissonNain : args size a réussi");
        assert arguments.contains("PoissonRouge") && arguments.indexOf("PoissonRouge") == 0 : "testDelFish PoissonRouge PoissonNain : argument1 a échoué";
        System.out.println("testDelFish PoissonRouge PoissonNain : argument1 a réussi");
        assert arguments.contains("PoissonNain") && arguments.indexOf("PoissonNain") == 1 : "testDelFish PoissonRouge PoissonNain : argument2 a échoué";
        System.out.println("testDelFish PoissonRouge PoissonNain : argument2 a réussi");

        del = "delFish";
        try{
            Parser.parse(del);
        } catch (ParserException e) {
            assert e.getMessage() == "There is no argument after delFish" : "testDelFish : a échoué";
            System.out.println("testDelFish : a réussi");
        }

        del = "delFish Rouge";
        try{
            Parser.parse(del);
        } catch (ParserException e) {
            assert e.getMessage() == "The fish name is not Poisson*" : "testDelFish Rouge : a échoué";
            System.out.println("testDelFish Rouge : a réussi");
        }

        del = "delFish PoissonNain RougeGorge";
        try{
            Parser.parse(del);
        } catch (ParserException e) {
            assert e.getMessage() == "The fish name is not Poisson*" : "testDelFish PoissonNain RougeGorge : a échoué";
            System.out.println("testDelFish PoissonNain RougeGorge : a réussi");
        }

        del = "delFish Rouge PoissonNain";
        try{
            Parser.parse(del);
        } catch (ParserException e) {
            assert e.getMessage() == "The fish name is not Poisson*" : "testDelFish Rouge PoissonNain : a échoué";
            System.out.println("testDelFish Rouge PoissonNain : a réussi");
        }
    }
}
