
// Aquarium.java
// A JavaFX application that displays an aquarium with fishes
import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;

import java.util.ArrayList;

import aquarium.Aquarium;
import aquarium.Fish;
import visuals.FishImage;

public class AquariumFX extends Application {
    // The width and height of the aquarium window
    private static final double WIDTH = 800;
    private static final double HEIGHT = 600;
    private ArrayList<FishImage> fishesInAquarium = new ArrayList<FishImage>();
    private ArrayList<FishImage> fishesNotInAquarium = new ArrayList<FishImage>();
    private Aquarium aquarium = Aquarium.getInstance();

    @Override
    public void start(Stage primaryStage) throws Exception {
        // Create a pane to hold the aquarium elements
        Pane pane = new Pane();
        System.out.println("Current working directory: " + System.getProperty("user.dir"));

        // Set the background image using CSS
        String backgroundImageUrl = getClass().getResource("/img/ocean.jpeg").toExternalForm();
        pane.setStyle("-fx-background-image: url('" + backgroundImageUrl + "'); -fx-background-size: cover;");

        // Create a scene with the pane and set its size to match the aquarium window
        // size
        Scene scene = new Scene(pane, WIDTH, HEIGHT);
        // Set the title and icon of the stage to "Aquarium" and a fish icon
        // respectively
        primaryStage.setTitle("Aquarium");
        // primaryStage.getIcons().add(new Image("img/fish1.png"));
        // Set the scene of the stage to the scene created above
        primaryStage.setScene(scene);
        // Show the stage
        primaryStage.show();

        for (Fish fish : aquarium.getFishes()) {
            FishImage fishImage = new FishImage("img/fish1.png", fish, scene.getWidth(), scene.getHeight());
            pane.getChildren().add(fishImage.getImageView());
            fishesInAquarium.add(fishImage);
        }

        while (aquarium.getFishes().isEmpty()) {
            System.out.println("Waiting for fishes");
            Thread.sleep(1000);
        }
        /* TODO change condition */
        while (!aquarium.getFishes().get(0).isStarted()) {
            System.out.println("Waiting for one fish to start");
            Thread.sleep(1000);
        }

        // Create an animation timer that updates the position and direction of each
        // fish every frame
        AnimationTimer timer = new AnimationTimer() {

            @Override
            public void handle(long now) {
                for (Fish fish : aquarium.getFishes()) {
                    // System.out.println("1st for loop");
                    boolean fishIsInAquarium = false;
                    for (FishImage fishImage : fishesInAquarium) {
                        // System.out.println("2nd for loop");
                        // System.out.println(fishImage.getFishData().getName() + " is in aquarium : "
                        // + fishImage.getFishData().equals(fish) + " " +
                        // fishImage.getFishData().getName() + " "
                        // + fish.getName());
                        // System.out.println(fishImage.getFishData());
                        if (fishImage.getFishData().equals(fish)) {
                            fishIsInAquarium = true;
                            break;
                        }
                    }
                    // System.out.println(fish.getName() + " is in aquarium: " + fishIsInAquarium);
                    if (!fishIsInAquarium) {
                        System.out.println("Adding fish " + fish + " to fishesInAquarium + image");
                        FishImage fishImage = new FishImage("./img/fish1.png", fish, scene.getWidth(),
                                scene.getHeight());
                        pane.getChildren().add(fishImage.getImageView());
                        fishesInAquarium.add(fishImage);
                    }
                }
                // System.out.println("3rd for loop");
                for (FishImage fishImage : fishesInAquarium) {
                    // System.out.println("inside 3rd for loop");

                    // if (fishImage.getFishData().getFirstDestination().getX()) {
                    // pane.getChildren().remove(fishImage.getImageView());
                    // fishesNotInAquarium.add(fishImage);
                    // } else {
                    // if (fishImage.getFishData().isStarted()) {
                    if (fishImage.getFishData().isStarted() && !fishImage.isMoving()) {
                        // System.out.println("Moving fish " + fishImage.getFishData().getName());
                        fishImage.move(scene.getWindow().getWidth(), scene.getWindow().getHeight());
                    }
                    // }
                }
            }
        };
        // Start the animation timer
        timer.start();

    }
}
