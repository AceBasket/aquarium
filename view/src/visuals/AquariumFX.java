package visuals;

// Aquarium.java
// A JavaFX application that displays an aquarium with fishes
import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.scene.Scene;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;
import utils.Log;
import utils.Log.LogLevel;
import javafx.collections.ObservableList;
import javafx.collections.FXCollections;

import java.io.PrintWriter;

import aquarium.Aquarium;
import aquarium.Fish;

public class AquariumFX extends Application {
    // The width and height of the aquarium window
    private static double width = 500;
    private static double height = 500;
    private static ObservableList<FishImage> fishesInAquarium = FXCollections.observableArrayList();

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
        Log.logMessage(logFile, LogLevel.INFO, "Stopping JavaFX");
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
        String imageFileName = "";
        if (fish.getName().contains("Rouge")) {
            imageFileName = "img/red_fish.png";
        } else if (fish.getName().contains("Jaune")) {
            imageFileName = "img/yellow_fish.png";
        } else if (fish.getName().contains("Bleu")) {
            imageFileName = "img/blue_fish.png";
        } else {
            imageFileName = "img/rainbow_fish.png";
        }
        FishImage fishImage = new FishImage(imageFileName, fish, width, height, id);
        fishImage.getImageView().getStyleClass().add("fish-image");
        getListFishImages().add(fishImage);
        Platform.runLater(() -> {
            if (fish.getPosition().getX() < 0 || fish.getPosition().getY() < 0) {
                fishImage.getImageView().setVisible(false);
            }
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
        scene.getRoot().setId("aquarium-root");

        // Set the title and icon of the stage to "Aquarium" and a fish icon
        // respectively
        primaryStage.setTitle("Aquarium");

        // Set the scene of the stage to the scene created above
        primaryStage.setScene(scene);
        // Show the stage
        primaryStage.show();

        synchronized (lock) {
            while (aquarium == null) {
                try {
                    lock.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }

        setupFishImageListener();

        // Create an animation timer that updates the position and direction of each
        // fish every frame
        timer = new AnimationTimer() {

            @Override
            public void handle(long now) {

                for (FishImage fishImage : getListFishImages()) {
                    if (fishImage.getFishData().isStarted() && !fishImage.isMoving()) {
                        fishImage.move(width, height);
                    }
                }
            }
        };
        // Start the animation timer
        timer.start();

    }
}
