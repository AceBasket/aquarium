package visuals;

// Aquarium.java
// A JavaFX application that displays an aquarium with fishes
import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.stage.Stage;

import javafx.collections.ObservableList;
import javafx.collections.FXCollections;

import java.io.PrintWriter;

import aquarium.Aquarium;
import aquarium.Fish;

public class AquariumFX extends Application {
    // The width and height of the aquarium window
    private static double width = 800;
    private static double height = 600;
    private static ObservableList<FishImage> fishesInAquarium = FXCollections.observableArrayList();
    // private ArrayList<FishImage> fishesNotInAquarium = new
    // ArrayList<FishImage>();
    private static Object lock = new Object();
    private static Aquarium aquarium;
    private PrintWriter logFile;
    private static long id;

    private Stage primaryStage;
    private AnimationTimer timer;
    private static Pane pane;

    public AquariumFX() {
        try {
            logFile = new PrintWriter("log_fx.log");
        } catch (Exception e) {
            System.out.println("Error creating log file for main thread");
        }
    }

    // Get the singleton instance
    public static synchronized ObservableList<FishImage> getListFishImages() {
        return fishesInAquarium;
    }

    public double getWidth() {
        return width;
    }

    public double getHeight() {
        return height;
    }

    public void stopAquariumFX() {
        if (timer != null) {
            timer.stop();
            Platform.runLater(() -> {
                Stage stage = (Stage) pane.getScene().getWindow();
                stage.close();
            });
        }
        Platform.exit();
    }

    public Stage getPrimaryStage() {
        return primaryStage;
    }

    public static Aquarium getAquarium() {
        return aquarium;
    }

    public static void setId(long id) {
        AquariumFX.id = id;
    }

    public static void setAquarium(Aquarium aquarium) {
        synchronized (lock) {
            AquariumFX.aquarium = aquarium;
            lock.notifyAll();
        }
    }

    private void setupFishImageListener() {
        FishImageListener listener = new FishImageListener(pane);
        fishesInAquarium.addListener(listener);
    }

    public static void addFishImage(Fish fish) {
        FishImage fishImage = new FishImage("img/fish1.png", fish, width, height, id);
        fishImage.getImageView().getStyleClass().add("fish-image");
        getListFishImages().add(fishImage);
        Platform.runLater(() -> {
            fishImage.getImageView().setX(fish.getPosition().getX());
            fishImage.getImageView().setY(fish.getPosition().getY());
            pane.getChildren().add(fishImage.getImageView());
        });
    }

    @Override
    public void start(Stage primaryStage) throws Exception {

        // Set the primary stage reference
        this.primaryStage = primaryStage;

        // Create a pane to hold the aquarium elements
        pane = new Pane();
        pane.setPrefSize(width, height); // Set preferred size

        // Set the background image using CSS
        String backgroundImageUrl = getClass().getResource("/img/ocean.jpeg").toExternalForm();
        pane.setStyle("-fx-background-image: url('" + backgroundImageUrl + "'); -fx-background-size: cover;");

        // Create a scene with the pane and set its size to match the aquarium window
        // size
        Scene scene = new Scene(pane, width, height);
        scene.getStylesheets().add(getClass().getResource("../visuals/styles.css").toExternalForm());
        scene.getRoot().setId("aquarium-root");

        // Set the title and icon of the stage to "Aquarium" and a fish icon
        // respectively
        primaryStage.setTitle("Aquarium");
        // primaryStage.getIcons().add(new Image("img/fish1.png"));

        // Set the scene of the stage to the scene created above
        primaryStage.setScene(scene);
        // Show the stage
        primaryStage.show();

        // Add listeners for window resize
        /*
         * primaryStage.widthProperty().addListener((obs, oldWidth, newWidth) -> {
         * double width = (double) newWidth;
         * double height = primaryStage.getHeight();
         * for (FishImage fishImage : fishesInAquarium) {
         * fishImage.handleWindowResize(width, height);
         * }
         * });
         * 
         * primaryStage.heightProperty().addListener((obs, oldHeight, newHeight) -> {
         * double width = primaryStage.getWidth();
         * double height = (double) newHeight;
         * for (FishImage fishImage : fishesInAquarium) {
         * fishImage.handleWindowResize(width, height);
         * }
         * });
         */

        synchronized (lock) {
            while (aquarium == null) {
                try {
                    lock.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }

        // setupFishImageListener();

        /*
         * for (Fish fish : aquarium.getFishes()) {
         * FishImage fishImage = new FishImage("img/fish1.png", fish, width, height);
         * fishImage.getImageView().getStyleClass().add("fish-image");
         * 
         * pane.getChildren().add(fishImage.getImageView());
         * fishesInAquarium.add(fishImage);
         * }
         */

        // Create an animation timer that updates the position and direction of each
        // fish every frame
        timer = new AnimationTimer() {

            @Override
            public void handle(long now) {
                /*
                 * for (Fish fish : aquarium.getFishes()) {
                 * // System.out.println("1st for loop");
                 * boolean fishIsInAquarium = false;
                 * for (FishImage fishImage : fishesInAquarium) {
                 * // System.out.println("2nd for loop");
                 * // System.out.println(fishImage.getFishData().getName() +
                 * " is in aquarium : "
                 * // + fishImage.getFishData().equals(fish) + " " +
                 * // fishImage.getFishData().getName() + " "
                 * // + fish.getName());
                 * // System.out.println(fishImage.getFishData());
                 * if (fishImage.getFishData().equals(fish)) {
                 * fishIsInAquarium = true;
                 * break;
                 * }
                 * }
                 * // System.out.println(fish.getName() + " is in aquarium: " +
                 * fishIsInAquarium);
                 * if (!fishIsInAquarium) {
                 * System.out.println("Adding fish " + fish + " to fishesInAquarium + image");
                 * FishImage fishImage = new FishImage("./img/fish1.png", fish, width, height);
                 * fishImage.getImageView().getStyleClass().add("fish-image");
                 * pane.getChildren().add(fishImage.getImageView());
                 * 
                 * fishesInAquarium.add(fishImage);
                 * }
                 * }
                 */

                // System.out.println("3rd for loop");
                for (FishImage fishImage : getListFishImages()) {
                    // System.out.println("inside 3rd for loop");

                    // if (fishImage.getFishData().getFirstDestination().getX()) {
                    // pane.getChildren().remove(fishImage.getImageView());
                    // fishesNotInAquarium.add(fishImage);
                    // } else {
                    // if (fishImage.getFishData().isStarted()) {
                    if (fishImage.getFishData().isStarted() && !fishImage.isMoving()) {
                        fishImage.move(width, height);
                    }
                    // }
                }
            }
        };
        // Start the animation timer
        timer.start();

    }
}
