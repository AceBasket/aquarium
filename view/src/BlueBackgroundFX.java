import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.StackPane;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

import threads.*;
public class BlueBackgroundFX extends Application {
    @Override
    public void start(Stage primaryStage) {
        StackPane root = new StackPane();
        Scene scene = new Scene(root, 400, 300);
        scene.setFill(Color.BLUE);
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    public static void main(String[] args) {
        
        try {
            Runnable promptThread = new PromptThread();
            // Runnable promptThread = new PromptThread(new View(new File("src/affichage.cfg")));
            Thread prompt = new Thread(promptThread);
            prompt.start();
            launch(args);
            prompt.join();
        } catch (Exception e) {
            // TODO: handle exception
            System.out.println(e);
        }
    }
}