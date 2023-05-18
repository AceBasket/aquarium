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

import java.time.Instant;

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

    // A constructor that takes the image file name, the initial position, the
    // speed, the direction and the aquarium size
    public FishImage(String fileName, Fish fishData) {
        try {

            // Load the image from the file
            image = new Image(fileName);
            // Create an image view with the image
            imageView = new ImageView(image);
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

    private double pixelToPercentages(double pixel, double aquariumSize) {
        return pixel / aquariumSize * 100;
    }

    private double percentagesToPixel(double percentage, double aquariumSize) {
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

        if (duration <= 0) {
            imageView.setX(endX);
            imageView.setY(endY);
            fishData.setPosition((int) pixelToPercentages(endX, width), (int) pixelToPercentages(endY, height));
            isMoving = false;
            return;
        }

        System.out.println("window size = " + width + ", " + height);
        System.out.println("Going from " + startX + ", " + startY + " to " + endX +
                ", " + endY + " in " + duration
                + " seconds");
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
            fishData.setPosition((int) pixelToPercentages(endX, width), (int) pixelToPercentages(endY, height));
            System.out.println("Fish " + fishData.getName() + " is now at " + fishData.getPosition().toString());
            imageView.setX(endX);
            imageView.setY(endY);
            isMoving = false;
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
