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

import java.io.File;
import java.util.Random;
import java.util.List;

public class FishAnimationFX extends Application {

    private final int VIEW_WIDTH = 600;
    private final int VIEW_HEIGHT = 400;
    private final int FISH_WIDTH = 150;
    private final int FISH_HEIGHT = 100;
    //private static final int BUFFER_WIDTH = 50;

    private ImageView fish;
    private double fishX, fishY;
    private double fishVX, fishVY;
    private ImageView view1, view2;
    private double viewX1, viewY1, viewX2, viewY2;
    private Random random = new Random();
    private Pane pane1, pane2;


    private DoubleProperty fishYProperty() {
        return fish.yProperty();
    }

    

    

    @Override
    public void start(Stage primaryStage) {
        //récupérer les arguments avec getParameters.
        List<String> parameters = getParameters().getRaw();

        //String arg1 = parameters.get(0);
        //String arg2 = parameters.get(1);
        //String arg3 = parameters.get(2);
        
        //view1 image
        File fileA1 = new File("./img/ocean.jpeg");
        
        Image viewImage1 = new Image(fileA1.toURI().toString());
        double pixelWidth = viewImage1.getWidth();
        double pixelHeight = viewImage1.getHeight();
        // System.out.println(viewImage1.getWidth());
        // System.out.println(viewImage1.getHeight());
        //Rectangle2D croppedPortion = new Rectangle2D(pixelWidth/2, pixelHeight/2, pixelWidth/2, pixelHeight/2);//x, y , width, height
        view1 = new ImageView(viewImage1);
        // target width and height:
        // double scaledWidth = VIEW_WIDTH/2 ;
        // double scaledHeight = VIEW_HEIGHT/2 ;
        //view1.setViewport(croppedPortion);
        // view1.setFitWidth(scaledWidth);
        // view1.setFitHeight(scaledHeight);
        //view1.setSmooth(true);
        viewX1 = VIEW_WIDTH;

        viewY1 = VIEW_HEIGHT;
        view1.setFitWidth(viewX1);
        view1.setFitHeight(viewY1);
        

        //view2
        File fileA2 = new File("./img/ocean.jpeg");
        Image viewImage2 = new Image(fileA2.toURI().toString());
        view2 = new ImageView(viewImage2);
        viewX2 = VIEW_WIDTH;
        viewY2 = VIEW_HEIGHT;
        view2.setFitWidth(viewX2);
        view2.setFitHeight(viewY2);

        

        //fish image
        File fileF = new File("./img/fish4.png");
        Image fishImage = new Image(fileF.toURI().toString());
        fish = new ImageView(fishImage);
        fishX = (VIEW_WIDTH - FISH_WIDTH) / 2;
        fishY = (VIEW_HEIGHT - FISH_HEIGHT) / 2;
        fish.setFitWidth(FISH_WIDTH);
        fish.setFitHeight(FISH_HEIGHT);

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
                    Pane newPane = (oldPane == pane1) ? pane2 : pane1;
                    newPane.getChildren().add(fish);

                    // Reset fish position and orientation
                    fishX = (VIEW_WIDTH - FISH_WIDTH) / 2;
                    fishY = (VIEW_HEIGHT - FISH_HEIGHT) / 2;
                    fish.setRotate(0);
                    fish.setScaleX(1);
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
        pane1.getChildren().add(fish);

       
        
        Scene scene1 = new Scene(pane1, VIEW_WIDTH, VIEW_HEIGHT);
        
        // Set up the stage
        primaryStage.setTitle("view1");
        primaryStage.setScene(scene1);
        primaryStage.show();

        
        //Set up the next view
        pane2 = new Pane();
        pane2.getChildren().add(view2);
        pane2.getChildren().add(fish);

        // Set up the stage
        Scene scene2 = new Scene(pane2, VIEW_WIDTH, VIEW_HEIGHT);
        // //Scene scene3 = new Scene(pane2, VIEW_WIDTH, VIEW_HEIGHT);
        Stage secondStage = new Stage();
        // //Stage thirdStage = new Stage();
        secondStage.setX(-50);
        secondStage.setY(-50);
        secondStage.setTitle("view2");
        secondStage.setScene(scene2);
        // //secondStage.setScene(scene3);
        secondStage.show();
        

    }

    public static void main(String[] args) {
        launch(args);
    }
}
