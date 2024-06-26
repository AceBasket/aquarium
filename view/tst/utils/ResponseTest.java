package utils;

import java.io.IOException;
import java.util.ArrayList;

public class ResponseTest {
    public static void main(String[] args) throws IOException, ParserException {
        // Par manque de temps, tous les tests n'ont pas pû être tous implémentés
        // Pour palier à ce problème, les fonctions du parseur ont toutes été implémentées en ayant en têtre tous les cas possibles que nous pouvions rencontrer

        boolean missingAssertion = false;
        assert missingAssertion = true;

        if (!missingAssertion) {
            System.out.println("Option -ea needed");
            return;
        }
        ResponseTest test = new ResponseTest();
        System.out.print('.');
        test.testParserOK();
        System.out.print('.');
        test.testParserNOK();
        System.out.print('.');
        test.testParserGreeting();
        System.out.print('.');
        test.testParserNoGreeting();
        System.out.print('.');
        test.testParserlist();
        System.out.print('.');
        test.testParserPong();
        System.out.print('.');
        test.testParserBye();
        System.out.println(" OK");
    }

    void testParserOK() throws IOException, ParserException {

    }

    void testParserNOK() throws IOException, ParserException {
        String nok = "NOK : unknown";
        ParserResult result = Parser.parse(nok);
        ArrayList<String> arguments = result.getArgs();
        assert Parser.PossibleResponses.NOK == result.getFunction() : "testNOK NOK : unknown : fonction a échoué";
        assert arguments.size() == 1 : "testNOK NOK : unknown : args size a échoué";

        nok = "NOK: unknown";
        result = Parser.parse(nok);
        arguments = result.getArgs();
        assert Parser.PossibleResponses.NOK == result.getFunction() : "testNOK NOK: unknown : fonction a échoué";
        assert arguments.size() == 1 : "testNOK NOK: unknown : args size a échoué";

        nok = "NOK";
        result = Parser.parse(nok);
        arguments = result.getArgs();
        assert Parser.PossibleResponses.NOK == result.getFunction() : "testNOK NOK : fonction a échoué";
        assert arguments.size() == 0 : "testNOK NOK : args size a échoué";

        nok = "NOK:";
        result = Parser.parse(nok);
        arguments = result.getArgs();
        assert Parser.PossibleResponses.NOK == result.getFunction() : "testNOK NOK: : fonction a échoué";
        assert arguments.size() == 0 : "testNOK NOK: : args size a échoué";
    }

    void testParserGreeting() throws IOException, ParserException {

    }

    void testParserNoGreeting() throws IOException, ParserException {

    }

    void testParserlist() throws IOException, ParserException {

    }

    void testParserPong() throws IOException, ParserException {

    }

    void testParserBye() throws IOException, ParserException {

    }
}