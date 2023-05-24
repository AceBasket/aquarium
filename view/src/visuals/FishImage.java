package visuals;

import javafx.animation.Interpolator;
import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.event.EventHandler;
import javafx.event.ActionEvent;

import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.util.Duration;
import utils.Log;
import utils.Log.LogLevel;

import java.io.PrintWriter;

import aquarium.Fish;

// A class that represents a fish in the aquarium
public class FishImage {
    // The image of the fish
    private Image image;
    // The image view of the fish
    private ImageView imageView;

    private Fish fishData;
    private boolean isMoving = false;

    private boolean secondToLastDestinationOnBorder = false;
    private boolean lastDestinationOnBorder = false;

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

            // Set position of the image view
            imageView.setX(percentagesToPixel(fishData.getPosition().getX(), aquariumWidth));
            imageView.setY(percentagesToPixel(fishData.getPosition().getY(), aquariumHeight));

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
            Log.logMessage(logFile, LogLevel.ERROR, "Error loading image " + fileName);
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

    boolean coordinatesOnBorder(double windowWidth, double windowHeight, double x, double y) {
        return x <= 0 || y <= 0 || x >= windowWidth || y >= windowHeight;
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
        double duration = (fishData.getTimeToGetToFirstDestination() - System.currentTimeMillis()) / 1000.0;

        // We are going to display movement to next destination, so we set the fish as displayed
        fishData.setDisplayedFirstDestination();

        Log.logMessage(logFile, LogLevel.INFO, fishData.getName() + ": " + fishData.getPosition().toString() + " --> "
                + fishData.getFirstDestination().toString() + " in " + duration + " seconds");

        // If movement lasts 0 or less seconds, teleport
        if (duration <= 0) {
            fishData.removeExpiredDestinations();
            imageView.setX(endX);
            imageView.setY(endY);
            fishData.setPosition((int) pixelToPercentages(endX, width), (int) pixelToPercentages(endY, height));
            isMoving = false;
            Log.logMessage(logFile, LogLevel.WARNING, "Duration of movement was <= 0");
            return;
        }

        if (!imageView.isVisible()) {
            // If its actual position is -1x-1, stay hidden
            if (fishData.getPosition().getX() == -1 && fishData.getPosition().getY() == -1) {
                Log.logMessage(logFile, LogLevel.INFO,
                        "Fish " + fishData.getName() + " stays hidden (actual position is -1x-1)");
                imageView.setVisible(false);
            }
            // If second to last destination was on a border and next destination is also on a border, set visible
            else if (secondToLastDestinationOnBorder && coordinatesOnBorder(width, height, endX, endY)) {
                Log.logMessage(logFile, LogLevel.INFO,
                        "Fish " + fishData.getName() + " becomes visible (from border to border by crossing view)");
                lastDestinationOnBorder = false;
            }
            // If position is on one of the borders and it goes inside the window, set visible
            else if (coordinatesOnBorder(width, height, startX, startY)
                    && !coordinatesOnBorder(width, height, endX, endY)) {
                Log.logMessage(logFile, LogLevel.INFO, "Fish " + fishData.getName() + " becomes visible (enters view)");
                imageView.setVisible(true);
            }
            // Else, if position is on one of the borders and its destination is a border, stay hidden
            else if (coordinatesOnBorder(width, height, startX, startY)
                    && coordinatesOnBorder(width, height, endX, endY)) {
                Log.logMessage(logFile, LogLevel.INFO,
                        "Fish " + fishData.getName() + " stays hidden (from border to border)");
                imageView.setVisible(false);
            }
        }

        // Create a timeline for moving the fish
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
            fishData.setPosition((int) pixelToPercentages(imageView.getX(), width),
                    (int) pixelToPercentages(imageView.getY(), height));
            Log.logMessage(logFile, LogLevel.INFO, fishData.getName() + " is now at " + (int) imageView.getX() + "x"
                    + (int) imageView.getY());
            imageView.setX(endX);
            imageView.setY(endY);

            isMoving = false;

            secondToLastDestinationOnBorder = lastDestinationOnBorder;

            if (coordinatesOnBorder(width, height, endX, endY)) {
                Log.logMessage(logFile, LogLevel.INFO, "Fish " + fishData.getName() + " is now hidden");
                imageView.setVisible(false);
                lastDestinationOnBorder = true;
            } else {
                lastDestinationOnBorder = false;
            }

            // Remove the first destination from the list of destinations
            fishData.removeExpiredDestinations();
        };

        // Add the key frames to the timeline
        timeline.getKeyFrames().addAll(start, end);

        // Set the event handler to be called when the animation finishes
        timeline.setOnFinished(onAnimationFinished);

        // Play the animation
        timeline.play();
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
