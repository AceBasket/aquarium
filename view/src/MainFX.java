import aquarium.*;
import java.util.Random;
import java.util.List;
import java.io.*;
import java.time.Instant;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.ArrayList;



import threads.*;
import utils.ParserResult;

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
import javafx.application.Platform;


import threads.*;
import utils.ParserResult;



public class MainFX extends Application {

    private final int VIEW_WIDTH = 1280;
    private final int VIEW_HEIGHT = 720;
    //private final int FISH_WIDTH = 150;
    //private final int FISH_HEIGHT = 100;
    //private static final int BUFFER_WIDTH = 50;

    private double viewX1, viewY1, viewX2, viewY2;
    private Random random = new Random();
    private Pane pane;//, pane2;

    PrintWriter logFile;
    //private ArrayList<String> fishesName;
    private ArrayList<ImageView> fishesImage;
    private ArrayList<Fish> fishes;

    public MainFX() {
        //fishesName = new ArrayList<String>();
        fishesImage = new ArrayList<ImageView>();
        fishes = new ArrayList<Fish>();
        try {
            logFile = new PrintWriter("log_main.log");
        } catch (IOException e) {
            System.out.println("Error creating log file for main thread");
        }
    }

    //prends une cordonnée X en pixel et renvoie une valeur entre 0 et 100
    public double pixelToPourcentageX(double pixelX){
        return (pixelX * 100)/ VIEW_WIDTH;
    }
    //prends une cordonnée Y en pixel et renvoie une valeur entre 0 et 100
    public double pixelToPourcentageY(double pixelY){
        return (pixelY * 100)/ VIEW_WIDTH;
    }
    //prends une cordonnée X entre 0 et 100 et renvoie une valeur en pixel
    public double pourcentageToPixelX(int pourcenatgeX){
        return (pourcenatgeX * VIEW_WIDTH)/100;
    }
    //prends une cordonnée Y entre 0 et 100 et renvoie une valeur en pixel
    public double pourcentageToPixelY(int pourcentageY){
        return (pourcentageY * VIEW_WIDTH)/100;
    }
    // private DoubleProperty fishYProperty() {
    //     return fish.yProperty();
    // }
    public void addFish(ImageView fishIV, String name, Fish fish) {
        if (!fishes.contains(fish)){//(!fishesName.contains(name)) {
            //fishesName.add(name);
            fishes.add(fish);
            fishesImage.add(fishIV);
            pane.getChildren().add(fishIV);
            //System.out.println("the fish was added in the ImageView\n");
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

    public ImageView createFish(Coordinates position, int fishWidth, int fishHeight){
        File fileF = new File("./img/fish4.png");
        Image fishImage = new Image(fileF.toURI().toString());
        ImageView fish = new ImageView(fishImage);

        double fishX = pourcentageToPixelX( position.getX());
        double fishY = pourcentageToPixelY(position.getY());
        fish.setFitWidth(pourcentageToPixelX(fishWidth));
        fish.setFitHeight(pourcentageToPixelY(fishHeight));
        fish.setX(fishX);
        fish.setY(fishY);
        return fish;
    }

    @Override
    public void start(Stage primaryStage) {

        
        //views
        view = createView();
        // Set up the animation timer
        AnimationTimer timer = new AnimationTimer() {
            @Override
            public void handle(long now) {
                for (int i = 0; i < fishes.size(); i++){
                    System.out.println(fishes.size());
                    // Assuming you have a target coordinate (targetX, targetY) and a duration in seconds
                    
                    Fish fish = fishes.get(i);
                    Coordinates destination = fish.getFirstDestination();
                    Coordinates destinationP = new Coordinates((int)pourcentageToPixelX(destination.getX()), (int)pourcentageToPixelY(destination.getY()));
                    double duration = (double)fish.getTimeToGetToFirstDestination() - Instant.now().getEpochSecond();

                    ImageView fishIV = fishesImage.get(i);
                    double fishX = fishIV.getX();
                    double fishY = fishIV.getY();
                    System.out.println("fishX "+fishX + " fishY "+fishY);
                    // Calculate the distance between the current position and the target position
                    double distanceX = destinationP.getX() - fishX;
                    double distanceY = destinationP.getY()  - fishY;
                   
                    //System.out.println("destination  " + destinationP );
                    // Calculate the velocity components
                     System.out.println("DX "+distanceX + " DY "+ distanceY+ "\n");
                    System.out.println("duration " +duration+"\n");
                    
                    double errorMargin = 1.0;  // Define a small margin of error
                    if (Math.abs(distanceX) <= errorMargin && Math.abs(distanceY) <= errorMargin) {
                        // Set the position of the fish to the destination
                        fishX = destinationP.getX();
                        fishY = destinationP.getY();
                    }

                    else if (duration !=0){
                        double velocityX = distanceX / (duration ); // Divide by the number of frames per second (e.g., 60)
                        double velocityY = distanceY / (duration ); // Adjust the frame rate as needed
                        System.out.println("VX "+velocityX + " vY "+ velocityY+ "\n");
                        // Update the fish's position based on the velocity components
                        fishX += velocityX;
                        fishY += velocityY;
                    }

     



                    // Set the new position of the fish
                    fishIV.setX(fishX);
                    fishIV.setY(fishY);
                    
                    // Check if the fish has reached (or passed) the target position
                    // if ((velocityX > 0 && fishX >= destinationP.getX()) || (velocityX < 0 && fishX <= destinationP.getX())
                    //         || (velocityY > 0 && fishY >= destinationP.getY()) || (velocityY < 0 && fishY <= destinationP.getY())) {
                    //     // Fish has reached (or passed) the target position, do something
                    //     fishX = destinationP.getX();
                    //     fishY = destinationP.getY();
                    // }
                    //////////////////////////////////////////////////////////
                    

                    // Check if fish is outside the bounds of view
                    // if (fishIV.getBoundsInParent().getMaxX() < 0 || fishIV.getBoundsInParent().getMinX() > VIEW_WIDTH ||
                    //         fishIV.getBoundsInParent().getMaxY() < 0 || fishIV.getBoundsInParent().getMinY() > VIEW_HEIGHT) {
                    //     pane.getChildren().remove(fishIV);
                    //     //fishesName.remove(name);
                    //     fishes.remove(fish);
                    //     fishesImage.remove(fishIV);

                    // }
                    // Flip the image horizontally if the fish is moving left
                    // if (velocityX < 0) {
                    //     fishIV.setScaleX(-1);
                    // } else {
                    //     fishIV.setScaleX(1);
                    // }

                    }
             
                
            }           

        };
        timer.start();

        // Set up the first view
        pane = new Pane();
        pane.getChildren().add(view);

        
        Scene scene = new Scene(pane, VIEW_WIDTH, VIEW_HEIGHT);
        
        // Set up the stage
        primaryStage.setTitle("view");
        primaryStage.setScene(scene);
        primaryStage.show();

        Thread mainThread = new Thread(() -> {
        try {
            View view = new View("0.0.0.0", 8000);
            Aquarium aquarium = new Aquarium();
            ConcurrentLinkedQueue<ParserResult> receivedQueue = new ConcurrentLinkedQueue<ParserResult>();
            ConcurrentLinkedQueue<String> sendQueue = new ConcurrentLinkedQueue<String>();
            Runnable readFromServerThread = new ReadFromServerThread(view, receivedQueue, sendQueue);
            Runnable serverThread = new ServerThread(view, aquarium, receivedQueue, sendQueue);
            Runnable promptThread = new PromptThread(view, aquarium, receivedQueue, sendQueue);
            Thread io = new Thread(readFromServerThread);
            Thread server = new Thread(serverThread);
            Thread prompt = new Thread(promptThread);
            io.start();
            server.start();
            prompt.start();

            while (true) {
                
                if (!aquarium.getFishes().isEmpty()) {
                    for (Fish fish : aquarium.getFishes()) {
                        
                        if (fish.getSizeDestinations() > 0) {
                            //System.out.println("creat fish view\n"+ fish.getSizeDestinations());
                            Platform.runLater(() -> {
                                ImageView fishIV = createFish(fish.getPosition(), fish.getLength(), fish.getHeight());
                                addFish(fishIV, fish.getName(), fish);
                            });
                            
                            logFile.println("It is " + Instant.now().getEpochSecond() + " and Fish "
                                    + fish.getName() + " is at " + fish.getPosition().toString()
                                    + " and needs to go to " + fish.getFirstDestination().toString() + " before "
                                    + fish.getTimeToGetToFirstDestination());
                            logFile.flush();
                        }
                    }
                }
                Thread.sleep(500);
            }
        } catch (IOException | InterruptedException e) {
            logFile.println(e);
            logFile.flush();
        }
    });
    mainThread.start();

    }

    public static void main(String[] args) {
        launch(args);
    }

    
}
