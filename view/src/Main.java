import utils.*;
import aquarium.*;
import java.io.*;
public class Main {
    public static void main(String[] argv) {
        try {

            View view = new View(new File("src/affichage.cfg"));

            // view.talkToServer("Testing connection");
            // view.talkToServer("hello in as " + view.getId());
            // view.talkToServer("hello");
            ParserResult answer = Parse.parserServerResponse(view.listenToServer());
            switch (answer.getFunction()) {
                case GREETING:
                    view.setId(answer.getArgs().get(0));
                    break;
                case NOGREETING:
                    System.out.println("Server is full");
                    break;
                case 
            
                default:
                    break;
            }

            System.out.println("END");
            // view.talkToServer("END");
            view.close();
        } catch (IOException | ParserException e) {
            System.out.println(e.getMessage());
        }
    }
}
