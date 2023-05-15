package utils;

import java.io.IOException;
import java.util.ArrayList;

public class CommandTest {
    public static void main(String[] args) throws IOException, ParserException {
        boolean missingAssertion = false;
        assert missingAssertion = true;

        if (!missingAssertion) {
            System.out.println("Option -ea needed");
            return;
        }
        CommandTest test = new CommandTest();
        System.out.print('.');
        test.testParserAddFish();
        System.out.print('.');
        test.testParserDelFish();
        System.out.print('.');
        test.testParserStartFish();
        System.out.print('.');
        test.testParserStatus();
        System.out.println(" OK");
    }

    void testParserStartFish() throws IOException, ParserException {
        String start = "startFish PoissonNain";
        ParserResult result = Parser.parse(start);
        ArrayList<String> arguments = result.getArgs();
        assert Parser.PossibleResponses.STARTFISH == result.getFunction()
                : "testStartFish PoissonNain : fonction a échoué";
        assert arguments.size() == 1 : "testStartFish PoissonNain : args size a échoué";
        assert arguments.contains("PoissonNain") && arguments.indexOf("PoissonNain") == 0
                : "testStartFish PoissonNain : argument a échoué";

        start = "startFish PoissonRouge PoissonNain";
        result = Parser.parse(start);
        arguments = result.getArgs();
        assert Parser.PossibleResponses.STARTFISH == result.getFunction()
                : "testStartFish PoissonRouge PoissonNain : fonction a échoué";
        assert arguments.size() == 2 : "testStartFish PoissonRouge PoissonNain : args size a échoué";
        assert arguments.contains("PoissonRouge") && arguments.indexOf("PoissonRouge") == 0
                : "testStartFish PoissonRouge PoissonNain : argument1 a échoué";
        assert arguments.contains("PoissonNain") && arguments.indexOf("PoissonNain") == 1
                : "testStartFish PoissonRouge PoissonNain : argument2 a échoué";

        start = "startFish";
        try {
            Parser.parse(start);
        } catch (ParserException e) {
            assert e.getMessage() == "There is no argument after startFish" : "testStartFish : a échoué";
        }

        start = "startFish Rouge";
        try {
            Parser.parse(start);
        } catch (ParserException e) {
            assert e.getMessage() == "The fish name is not Poisson*" : "testStartFish Rouge : a échoué";
        }

        start = "startFish PoissonNain RougeGorge";
        try {
            Parser.parse(start);
        } catch (ParserException e) {
            assert e.getMessage() == "The fish name is not Poisson*"
                    : "testStartFish PoissonNain RougeGorge : a échoué";
        }

        start = "startFish Rouge PoissonNain";
        try {
            Parser.parse(start);
        } catch (ParserException e) {
            assert e.getMessage() == "The fish name is not Poisson*" : "testStartFish Rouge PoissonNain : a échoué";
        }
    }

    void testParserDelFish() throws IOException, ParserException {
        String del = "delFish PoissonNain";
        ParserResult result = Parser.parse(del);
        ArrayList<String> arguments = result.getArgs();
        assert Parser.PossibleResponses.DELFISH == result.getFunction() : "testDelFish PoissonNain : fonction a échoué";
        assert arguments.size() == 1 : "testDelFish PoissonNain : args size a échoué";
        assert arguments.contains("PoissonNain") && arguments.indexOf("PoissonNain") == 0
                : "testDelFish PoissonNain : argument a échoué";

        del = "delFish PoissonRouge PoissonNain";
        result = Parser.parse(del);
        arguments = result.getArgs();
        assert Parser.PossibleResponses.DELFISH == result.getFunction()
                : "testDelFish PoissonRouge PoissonNain : fonction a échoué";
        assert arguments.size() == 2 : "testDelFish PoissonRouge PoissonNain : args size a échoué";
        assert arguments.contains("PoissonRouge") && arguments.indexOf("PoissonRouge") == 0
                : "testDelFish PoissonRouge PoissonNain : argument1 a échoué";
        assert arguments.contains("PoissonNain") && arguments.indexOf("PoissonNain") == 1
                : "testDelFish PoissonRouge PoissonNain : argument2 a échoué";

        del = "delFish";
        try {
            Parser.parse(del);
        } catch (ParserException e) {
            assert e.getMessage() == "There is no argument after delFish" : "testDelFish : a échoué";
        }

        del = "delFish Rouge";
        try {
            Parser.parse(del);
        } catch (ParserException e) {
            assert e.getMessage() == "The fish name is not Poisson*" : "testDelFish Rouge : a échoué";
        }

        del = "delFish PoissonNain RougeGorge";
        try {
            Parser.parse(del);
        } catch (ParserException e) {
            assert e.getMessage() == "The fish name is not Poisson*" : "testDelFish PoissonNain RougeGorge : a échoué";
        }

        del = "delFish Rouge PoissonNain";
        try {
            Parser.parse(del);
        } catch (ParserException e) {
            assert e.getMessage() == "The fish name is not Poisson*" : "testDelFish Rouge PoissonNain : a échoué";
        }
    }

    void testParserAddFish() throws IOException, ParserException {
        String add = "addFish PoissonNain at 61x52, 4x3, RandomWayPoint";
        ParserResult result = Parser.parse(add);
        ArrayList<String> arguments = result.getArgs();
        assert Parser.PossibleResponses.ADDFISH == result.getFunction()
                : "testAddFish PoissonNain at 61x52, 4x3, RandomWayPoint : fonction a échoué";
        assert arguments.size() == 6 : "testStartFish PoissonNain : args size a échoué";
        assert arguments.contains("PoissonNain") && arguments.indexOf("PoissonNain") == 0
                : "testAddFish PoissonNain at 61x52, 4x3, RandomWayPoint : argument a échoué";
        assert arguments.contains("61") && arguments.indexOf("61") == 1
                : "testAddFish PoissonNain at 61x52, 4x3, RandomWayPoint : argument a échoué";
        assert arguments.contains("52") && arguments.indexOf("52") == 2
                : "testAddFish PoissonNain at 61x52, 4x3, RandomWayPoint : argument a échoué";
        assert arguments.contains("4") && arguments.indexOf("4") == 3
                : "testAddFish PoissonNain at 61x52, 4x3, RandomWayPoint : argument a échoué";
        assert arguments.contains("3") && arguments.indexOf("3") == 4
                : "testAddFish PoissonNain at 61x52, 4x3, RandomWayPoint : argument a échoué";
        assert arguments.contains("RandomWayPoint") && arguments.indexOf("RandomWayPoint") == 5
                : "testAddFish PoissonNain at 61x52, 4x3, RandomWayPoint : argument a échoué";

        add = "addFish Nain at 61x52, 4x3, RandomWayPoint";
        try {
            Parser.parse(add);
        } catch (ParserException e) {
            assert e.getMessage().equals("The fish name is not Poisson*")
                    : "testAddFish Nain at 61x52, 4x3, RandomWayPoint : exception a echoué";
        }

        add = "addFish PoissonNain at ax52, 4x3, RandomWayPoint";
        try {
            Parser.parse(add);
        } catch (ParserException e) {
            assert e.getMessage().equals("Invalid Argument Format")
                    : "testAddFish PoissonNain at ax52, 4x3, RandomWayPoint : exception a echoué";
        }

        add = "addFish PoissonNain at 61xb, 4x3, RandomWayPoint";
        try {
            Parser.parse(add);
        } catch (ParserException e) {
            assert e.getMessage().equals("Invalid Argument Format")
                    : "testAddFish PoissonNain at 61xb, 4x3, RandomWayPoint : exception a echoué";
        }

        add = "addFish PoissonNain at 61x52, cx3, RandomWayPoint";
        try {
            Parser.parse(add);
        } catch (ParserException e) {
            assert e.getMessage().equals("Invalid Argument Format")
                    : "testAddFish PoissonNain at 61x52, cx3, RandomWayPoint : exception a echoué";
        }

        add = "addFish PoissonNain at 61x52, 4xd, RandomWayPoint";
        try {
            Parser.parse(add);
        } catch (ParserException e) {
            assert e.getMessage().equals("Invalid Argument Format")
                    : "testAddFish PoissonNain at 61x52, 4xd, RandomWayPoint : exception a echoué";
        }

        add = "addFish PoissonNain";
        try {
            Parser.parse(add);
        } catch (ParserException e) {
            assert e.getMessage().equals("There is not the right number of argument after addFish")
                    : "testAddFish PoissonNain : exception a echoué";
        }

        add = "addFish PoissonNain at 61x52, 4x3, WayPoint, Random";
        try {
            Parser.parse(add);
        } catch (ParserException e) {
            assert e.getMessage().equals("There is not the right number of argument after addFish")
                    : "testAddFish PoissonNain at 61x52, 4x3, WayPoint, Random : exception a echoué";
        }
    }

    void testParserStatus() throws IOException, ParserException {
        String status = "status";
        ParserResult result = Parser.parse(status);
        ArrayList<String> arguments = result.getArgs();
        assert Parser.PossibleResponses.STATUS == result.getFunction() : "testStatus status : fonction a échoué";
        assert arguments.size() == 0 : "testStatus status : args size a échoué";

        status = "status aquarium";
        result = Parser.parse(status);
        arguments = result.getArgs();
        assert Parser.PossibleResponses.STATUS == result.getFunction()
                : "testStatus status aquarium : fonction a échoué";
        assert arguments.size() == 0 : "testStatus status aquarium : args size a échoué";
    }
}
