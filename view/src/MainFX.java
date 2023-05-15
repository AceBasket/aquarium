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

    private final int VIEW_WIDTH = 600;
    private final int VIEW_HEIGHT = 400;
    private final int FISH_WIDTH = 150;
    private final int FISH_HEIGHT = 100;
    //private static final int BUFFER_WIDTH = 50;

    //private ImageView fish;
    private double fishX, fishY;
    //private double fishVX, fishVY;
    private ImageView view;//, view2;
    private double viewX1, viewY1, viewX2, viewY2;
    private Random random = new Random();
    private Pane pane;//, pane2;

    PrintWriter logFile;
    private ArrayList<String> fishesName;
    private ArrayList<ImageView> fishesList;

    public MainFX() {
        fishesName = new ArrayList<String>();
        fishesList = new ArrayList<ImageView>();
        try {
            logFile = new PrintWriter("log_main.log");
        } catch (IOException e) {
            System.out.println("Error creating log file for main thread");
        }
    }

    //mainly to crop the image 
    public double[] imageProportionsToPixel(double xI, double yI, Image img, int viewWidth, int viewHeight){// x and y are coordinates in the image
        double pixelWidth = img.getWidth();
        double pixelHeight = img.getHeight();

        double xP = xI * pixelWidth/viewWidth;
        double yP = yI * pixelHeight/viewHeight;

        double[] coordinates = new double[2];
        coordinates[0] = xP;
        coordinates[1] = yP;
        return coordinates;
    }
    // private DoubleProperty fishYProperty() {
    //     return fish.yProperty();
    // }
    public void addFish(ImageView fish, String name) {
        if (!fishesName.contains(name)) {
            fishesName.add(name);
            fishesList.add(fish);
            pane.getChildren().add(fish);
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

    public ImageView createFish(int fishWidth, int fishHeight){
        File fileF = new File("./img/fish4.png");
        Image fishImage = new Image(fileF.toURI().toString());
        ImageView fish = new ImageView(fishImage);
        fishX = (VIEW_WIDTH - FISH_WIDTH)/2 ; //fishWidth) / 2;
        fishY = (VIEW_HEIGHT - FISH_HEIGHT)/2; //fishHeight) / 2;
        fish.setFitWidth(FISH_WIDTH);//fishWidth);
        fish.setFitHeight(FISH_HEIGHT);//fishHeight);
        return fish;
    }

    @Override
    public void start(Stage primaryStage) {
        //récupérer les arguments avec getParameters.
        //List<String> parameters = getParameters().getRaw();

        
        //views
        view = createView();//new ImageView(viewImage1);
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
                for (ImageView fish : fishesList){
                    double fishVX = 0;
                    double fishVY = 0;
                    double fishX = fish.getX();
                    double fishY = fish.getY();
                    if (random.nextDouble() < 0.02) {
                        fishVX = random.nextDouble() * 6 - 3; 
                        fishVY = random.nextDouble() * 6 - 3; 
                    }
                    fishX += fishVX;
                    fishY += fishVY;

                    fish.setX(fishX);
                    fish.setY(fishY);
                    //this should change depending on the view
                    // Check if fish is outside the bounds of view
                    if (fish.getBoundsInParent().getMaxX() < 0 || fish.getBoundsInParent().getMinX() > VIEW_WIDTH ||
                            fish.getBoundsInParent().getMaxY() < 0 || fish.getBoundsInParent().getMinY() > VIEW_HEIGHT) {
                        System.out.println("the fish is outside the bounds\n");
                        // Remove fish from old view
                        //Pane oldPane = (Pane) fish.getParent();
                        //oldPane.getChildren().remove(fish);
                        //pane.getChildren().remove(fish);
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
             
                
            }           

        };
        timer.start();

        // Set up the first view
        pane = new Pane();
        pane.getChildren().add(view);
        //pane.getChildren().add(fish);
        // for (ImageView fish : fishesList){
        //     pane.getChildren().add(fish);
        // }
       
        
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
                                ImageView fishIV = createFish(fish.getLength(), fish.getHeight());
                                addFish(fishIV, fish.getName());
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
