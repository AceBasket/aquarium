import utils.*;
import aquarium.*;
import java.io.*;
public class Main {
    public static void main(String[] argv) {
        try {

            View view = new View(new File("affichage.cfg"));

            // view.talkToServer("Testing connection");
            // view.talkToServer("hello in as " + view.getId());
            view.talkToServer("hello");
            // ServerResponseParserResult answer = Parse.parserServerResponse(view.listenToServer());

            System.out.println("END");
            // view.talkToServer("END");
            view.close();
        } catch (IOException | ParserException e) {
            System.out.println(e.getMessage());
        }
    }
}