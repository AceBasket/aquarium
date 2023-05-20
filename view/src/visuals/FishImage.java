package visuals;

import javafx.animation.Interpolator;
import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.event.ActionEvent;

import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.util.Duration;

import java.io.PrintWriter;
import java.time.Instant;
import java.util.List;

import aquarium.Coordinates;
import aquarium.Fish;

// A class that represents a fish in the aquarium
public class FishImage {
    // The image of the fish
    private Image image;
    // The image view of the fish
    private ImageView imageView;

    private Fish fishData;
    private boolean isMoving = false;

    private PrintWriter logFile;

    // A constructor that takes the image file name, the initial position, the
    // speed, the direction and the aquarium size
    public FishImage(String fileName, Fish fishData, double aquariumWidth, double aquariumHeight, long id) {
        try {

            // Load the image from the file
            image = new Image(fileName);
            // Create an image view with the image
            imageView = new ImageView(image);
            // Set the image view dimensions to match the image dimensions
            imageView.setFitWidth(percentagesToPixel(fishData.getWidth(), aquariumWidth));
            imageView.setFitHeight(percentagesToPixel(fishData.getHeight(), aquariumHeight));

            // Set the image view to preserve the image ratio
            imageView.setPreserveRatio(true);

            // Set the image view to be visible
            imageView.setVisible(true);

            // Set the image view to be always on top of other nodes
            imageView.toFront();

            // Initialize the log file
            try {
                logFile = new PrintWriter("log_image_" + fishData.getName() + id + ".log");
            } catch (Exception e) {
                System.out.println("Error creating log file for fish image " + fishData.getName());
            }
        } catch (Exception e) {
            System.out.println("Error loading image. Current working directory: " + System.getProperty("user.dir"));
            System.out.println("Trying to access: " + System.getProperty("user.dir") + "/" + fileName);
        }
        this.fishData = fishData;
        // Set the initial position of the image view
        imageView.setX(fishData.getPosition().getX());
        imageView.setY(fishData.getPosition().getY());
    }

    // A method that returns the image view of the fish
    public ImageView getImageView() {
        return imageView;
    }

    public boolean isMoving() {
        return isMoving;
    }

    double pixelToPercentages(double pixel, double aquariumSize) {
        return pixel / aquariumSize * 100;
    }

    double percentagesToPixel(double percentage, double aquariumSize) {
        return percentage / 100 * aquariumSize;
    }

    // A method that updates the position and direction of the fish
    public void move(double width, double height) {

        if (fishData.getSizeDestinations() <= 0) {
            return;
        }

        isMoving = true;

        double startX = percentagesToPixel(fishData.getPosition().getX(), width);
        double startY = percentagesToPixel(fishData.getPosition().getY(), height);
        double endX = percentagesToPixel(fishData.getFirstDestination().getX(), width);
        double endY = percentagesToPixel(fishData.getFirstDestination().getY(), height);
        double duration = fishData.getTimeToGetToFirstDestination() - Instant.now().getEpochSecond();

        /*
         * We are going to display movement to next destination, main thread can remove
         * the destination
         */
        fishData.setDisplayed();

        /*
         * Set visible if the fish's trajectory crosses the screen and doesn't follow
         * one side
         */
        if (startX <= 0 && endX > 0 || startX > 0 && endX <= 0) {
            logFile.println("Fish " + fishData.getName() + " is visible");
            logFile.flush();
            imageView.setVisible(true);
        }

        /* If fish's trajectory follows along one side, hide the fish */
        if (startX <= 0 && endX <= 0 || startY <= 0 && endY <= 0) {
            logFile.println("Fish " + fishData.getName() + " is hidden");
            logFile.flush();
            imageView.setVisible(false);
        }

        if (duration <= 0) {
            imageView.setX(endX);
            imageView.setY(endY);
            fishData.setPosition((int) pixelToPercentages(endX, width), (int) pixelToPercentages(endY, height));
            isMoving = false;
            return;
        }

        logFile.println(Instant.now());
        logFile.println(fishData.getName() + ": " + fishData.getPosition().toString() + " --> "
                + fishData.getFirstDestination().toString() + " in " + duration + " seconds");
        logFile.flush();
        // Create a timeline animation
        Timeline timeline = new Timeline();

        // Create key frames for the start and end positions
        KeyFrame start = new KeyFrame(Duration.ZERO,
                new KeyValue(imageView.xProperty(), startX, Interpolator.LINEAR),
                new KeyValue(imageView.yProperty(), startY, Interpolator.LINEAR));
        KeyFrame end = new KeyFrame(Duration.seconds(duration),
                new KeyValue(imageView.xProperty(), endX, Interpolator.LINEAR),
                new KeyValue(imageView.yProperty(), endY, Interpolator.LINEAR));

        // Set an event handler to be called when the animation finishes
        EventHandler<ActionEvent> onAnimationFinished = e -> {
            // Update the fish coordinates to the destination coordinates
            // fishData.setPosition((int) pixelToPercentages(endX, width), (int)
            // pixelToPercentages(endY, height));
            fishData.setPosition((int) pixelToPercentages(imageView.getX(), width),
                    (int) pixelToPercentages(imageView.getY(), height));
            logFile.println("Fish " + fishData.getName() + " is now at " + (int) imageView.getX() + "x"
                    + (int) imageView.getY());
            logFile.flush();
            // logFile.println("pane size = " + (int) width + "x" + (int) height);
            imageView.setX(endX);
            imageView.setY(endY);

            isMoving = false;

            if (endX <= 0 || endY <= 0) {
                logFile.println("Fish " + fishData.getName() + " is now hidden");
                logFile.flush();
                imageView.setVisible(false);
            }
        };

        // Add the key frames to the timeline
        timeline.getKeyFrames().addAll(start, end);

        // Set the event handler to be called when the animation finishes
        timeline.setOnFinished(onAnimationFinished);

        // Play the animation
        timeline.play();
    }

    void handleWindowResize(double width, double height) {
        // Update the fish's coordinates based on the new window size
        int currentX = fishData.getPosition().getX();
        int currentY = fishData.getPosition().getY();
        int newX = (int) pixelToPercentages(imageView.getX(), width);
        int newY = (int) pixelToPercentages(imageView.getY(), height);
        fishData.setPosition(newX, newY);

        // Recalculate the start and end coordinates for the fish's movement animation
        double startX = percentagesToPixel(fishData.getPosition().getX(), width);
        double startY = percentagesToPixel(fishData.getPosition().getY(), height);
        double endX = percentagesToPixel(fishData.getFirstDestination().getX(), width);
        double endY = percentagesToPixel(fishData.getFirstDestination().getY(), height);

        // Update the fish's position on the screen
        imageView.setX(startX);
        imageView.setY(startY);

        // Adjust the fish's movement animation to the new coordinates
        adjustMovementAnimation(startX, startY, endX, endY);
    }

    private void adjustMovementAnimation(double startX, double startY, double endX, double endY) {
        // Retrieve the existing key frames from the animation
        Timeline timeline = (Timeline) imageView.getScene().getWindow().getUserData();
        List<KeyFrame> keyFrames = timeline.getKeyFrames();

        // Modify the start and end key frames with the updated coordinates
        KeyFrame startFrame = keyFrames.get(0);
        KeyFrame endFrame = keyFrames.get(1);
        startFrame = new KeyFrame(startFrame.getTime(),
                new KeyValue(imageView.xProperty(), startX, Interpolator.LINEAR),
                new KeyValue(imageView.yProperty(), startY, Interpolator.LINEAR));
        endFrame = new KeyFrame(endFrame.getTime(),
                new KeyValue(imageView.xProperty(), endX, Interpolator.LINEAR),
                new KeyValue(imageView.yProperty(), endY, Interpolator.LINEAR));

        // Update the key frames in the animation
        keyFrames.set(0, startFrame);
        keyFrames.set(1, endFrame);
    }

    public Fish getFishData() {
        return fishData;
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof FishImage) && !(o instanceof Fish)) {
            return false;
        }
        FishImage fishImage = (FishImage) o;
        return this.fishData.equals(fishImage.fishData);
    }
}
