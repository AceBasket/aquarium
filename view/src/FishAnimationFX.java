import aquarium.*;
import java.util.Random;
import java.util.List;
import java.io.*;
import java.time.Instant;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.ArrayList;

import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;
import javafx.beans.property.DoubleProperty;
import javafx.animation.Timeline;
import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Interpolator;
import javafx.util.Duration;
import javafx.geometry.Rectangle2D;
import javafx.scene.Group;



import threads.*;
import utils.ParserResult;


// public class Main {
//     PrintWriter logFile;

//     public Main() {
//         try {
//             logFile = new PrintWriter("log_main.log");
//         } catch (IOException e) {
//             System.out.println("Error creating log file for main thread");
//         }
//     }
// }
public class FishAnimationFX extends Application {

    private final int VIEW_WIDTH = 600;
    private final int VIEW_HEIGHT = 400;
    private final int FISH_WIDTH = 150;
    private final int FISH_HEIGHT = 100;
    //private static final int BUFFER_WIDTH = 50;

    private ImageView fish;
    private double fishX, fishY;
    private double fishVX, fishVY;
    private ImageView view1;//, view2;
    private double viewX1, viewY1, viewX2, viewY2;
    private Random random = new Random();
    private Pane pane1;//, pane2;

    PrintWriter logFile;
    private ArrayList<String> fishesName;
    private ArrayList<ImageView> fishesList;

    public FishAnimationFX() {
        fishesName = new ArrayList<String>();
        fishesList = new ArrayList<ImageView>();
        try {
            logFile = new PrintWriter("log_main.log");
        } catch (IOException e) {
            System.out.println("Error creating log file for main thread");
        }
    }

    private DoubleProperty fishYProperty() {
        return fish.yProperty();
    }
    public synchronized void addFish(ImageView fish, String name) {
        if (!fishesName.contains(name)) {
            fishesName.add(name);
            fishesList.add(fish);
            pane1.getChildren().add(fish);
        }
    }
    public ImageView createView(){//(String fileName){
        File fileA = new File("./img/ocean.jpeg");     
        Image viewImage = new Image(fileA.toURI().toString());
        ImageView view = new ImageView(viewImage);
        double viewX = VIEW_WIDTH;
        double viewY = VIEW_HEIGHT;
        view.setFitWidth(viewX);
        view.setFitHeight(viewY);
        return view;
    }

    public ImageView createFish(int fishWidth, int fishHeight){
        File fileF = new File("./img/fish4.png");
        Image fishImage = new Image(fileF.toURI().toString());
        ImageView fish = new ImageView(fishImage);
        fishX = (VIEW_WIDTH - fishWidth) / 2;
        fishY = (VIEW_HEIGHT - fishHeight) / 2;
        fish.setFitWidth(fishWidth);
        fish.setFitHeight(fishHeight);
        return fish;
    }

    @Override
    public void start(Stage primaryStage) {
        //récupérer les arguments avec getParameters.
        List<String> parameters = getParameters().getRaw();

        //String arg1 = parameters.get(0);
        //String arg2 = parameters.get(1);
        //String arg3 = parameters.get(2);
        
        //views
        view1 = createView();//new ImageView(viewImage1);
        //view2 = createView();
        

        //fish image
        //fish = createFish( FISH_WIDTH);

        // Timeline swimAnimation = new Timeline(
        // new KeyFrame(Duration.ZERO, new KeyValue(fishYProperty(), fishY)),
        // new KeyFrame(Duration.seconds(2), new KeyValue(fishYProperty(), fishY + 50, Interpolator.EASE_BOTH)),
        // new KeyFrame(Duration.seconds(4), new KeyValue(fishYProperty(), fishY - 50, Interpolator.EASE_BOTH)),
        // new KeyFrame(Duration.seconds(6), new KeyValue(fishYProperty(), fishY))
        // );

        // swimAnimation.setCycleCount(Timeline.INDEFINITE);
        // swimAnimation.play();

        // Set up the animation timer
        AnimationTimer timer = new AnimationTimer() {
            @Override
            public void handle(long now) {
   
             if (random.nextDouble() < 0.02) {
                    fishVX = random.nextDouble() * 6 - 3; 
                    fishVY = random.nextDouble() * 6 - 3; 
                }
                fishX += fishVX;
                fishY += fishVY;

                fish.setX(fishX);
                fish.setY(fishY);
                //this should change depending on the view
                // Check if fish is outside the bounds of view1
                if (fish.getBoundsInParent().getMaxX() < 0 || fish.getBoundsInParent().getMinX() > VIEW_WIDTH ||
                        fish.getBoundsInParent().getMaxY() < 0 || fish.getBoundsInParent().getMinY() > VIEW_HEIGHT) {
                    // Remove fish from old view
                    Pane oldPane = (Pane) fish.getParent();
                    oldPane.getChildren().remove(fish);

                    // Add fish to new view
                    //Pane newPane = (oldPane == pane1) ? pane2 : pane1;
                    //newPane.getChildren().add(fish);

                    // Reset fish position and orientation
                    // fishX = (VIEW_WIDTH - FISH_WIDTH) / 2;
                    // fishY = (VIEW_HEIGHT - FISH_HEIGHT) / 2;
                    // fish.setRotate(0);
                    // fish.setScaleX(1);
                }
                // Flip the image horizontally if the fish is moving left
                if (fishVX < 0) {
                    fish.setScaleX(-1);
                } else {
                    fish.setScaleX(1);
                }
                
            }           

        };
        timer.start();

        // Set up the first view
        pane1 = new Pane();
        pane1.getChildren().add(view1);
        //pane1.getChildren().add(fish);
        for (ImageView fish : fishesList){
            pane1.getChildren().add(fish);
        }
       
        
        Scene scene1 = new Scene(pane1, VIEW_WIDTH, VIEW_HEIGHT);
        
        // Set up the stage
        primaryStage.setTitle("view1");
        primaryStage.setScene(scene1);
        primaryStage.show();

        
        // //Set up the next view
        // pane2 = new Pane();
        // pane2.getChildren().add(view2);
        // pane2.getChildren().add(fish);

        // // Set up the stage
        // Scene scene2 = new Scene(pane2, VIEW_WIDTH, VIEW_HEIGHT);
        // // //Scene scene3 = new Scene(pane2, VIEW_WIDTH, VIEW_HEIGHT);
        // Stage secondStage = new Stage();
        // // //Stage thirdStage = new Stage();
        // secondStage.setX(-50);
        // secondStage.setY(-50);
        // secondStage.setTitle("view2");
        // secondStage.setScene(scene2);
        // // //secondStage.setScene(scene3);
        // secondStage.show();
        

    }

    public static void main(String[] args) {
        FishAnimationFX main = new FishAnimationFX();
        try {
            // main.logFile.println("Main");
            // main.logFile.flush();
            // View view = new View("192.168.191.78", 8888);
            View view = new View("0.0.0.0", 8000);
            // View view = new View(new File("src/affichage.cfg"));
            Aquarium aquarium = new Aquarium();
            ConcurrentLinkedQueue<ParserResult> receivedQueue = new ConcurrentLinkedQueue<ParserResult>();
            ConcurrentLinkedQueue<String> sendQueue = new ConcurrentLinkedQueue<String>();
            Runnable readFromServerThread = new ReadFromServerThread(view, receivedQueue, sendQueue);
            Runnable serverThread = new ServerThread(view, aquarium, receivedQueue, sendQueue);
            Runnable promptThread = new PromptThread(view, aquarium, receivedQueue, sendQueue);
            Thread prompt = new Thread(promptThread);
            Thread server = new Thread(serverThread);
            Thread io = new Thread(readFromServerThread);
            io.start();
            server.start();
            prompt.start();

            main.logFile.println("All threads running");
            main.logFile.flush();
            //créer une liste des poissons Image view et les passer en paramètre de launch.
            while (true) {
                // System.out.println("Main thread running");
                if (!aquarium.getFishes().isEmpty()) {
                    for (Fish fish : aquarium.getFishes()) {
                        if (fish.getSizeDestinations() > 0) {
                            //créer le poisson 
                            ImageView fishIV = main.createFish(fish.getLength(), fish.getHeight());
                            //l'ajouter dans la view
                            main.addFish(fishIV, fish.getName());
                            main.logFile.println("It is " + Instant.now().getEpochSecond() + " and Fish "
                                    + fish.getName() + " is at " + fish.getPosition().toString()
                                    + " and needs to go to " + fish.getFirstDestination().toString() + " before "
                                    + fish.getTimeToGetToFirstDestination());
                            main.logFile.flush();
                        }
                    }
                }
                Thread.sleep(500);
                launch(args);
            }

            // io.join();
            // server.join();
            // prompt.join();
        } catch (IOException | InterruptedException e) {
            // TODO: handle exception
            main.logFile.println(e);
            main.logFile.flush();
        }
    
    }
    
}
