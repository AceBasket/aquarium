package aquarium;

import java.util.ArrayList;

import visuals.AquariumFX;
import visuals.FishImage;

public class Aquarium {
    private ArrayList<Fish> fishesList;

    private Aquarium() {
        // Private constructor to prevent instantiation
        fishesList = new ArrayList<Fish>();
    }

    private static class AquariumHolder {
        private static final Aquarium INSTANCE = new Aquarium();
    }

    public static Aquarium getInstance() {
        return AquariumHolder.INSTANCE;
    }

    public synchronized void addFish(Fish fish) {
        if (!fishesList.contains(fish)) {
            fishesList.add(fish);
            /*
             * // Create a corresponding FishImage and add it to the existing AquariumFX
             * // instance
             * FishImage fishImage = new FishImage("img/fish1.png", fish, 20, 10);
             * fishImage.getImageView().getStyleClass().add("fish-image");
             * 
             * // Access the existing AquariumFX instance and add the FishImage to its list
             * AquariumFX.getListFishImages().add(fishImage);
             */

            AquariumFX.addFishImage(fish);
            return;
        }

        throw new IllegalArgumentException("Fish already exists");
    }

    public synchronized void removeFish(String name) throws IllegalArgumentException {
        Fish fish = getFish(name);
        if (fish == null) {
            throw new IllegalArgumentException("Fish not found");
        }
        fishesList.remove(fish);

        // Access the existing AquariumFX instance and remove the corresponding
        // FishImage
        AquariumFX.getListFishImages().removeIf(fishImage -> fishImage.getFishData() == fish);
    }

    public synchronized ArrayList<Fish> getFishes() {
        return fishesList;
    }

    public synchronized void setFishDestination(Fish fish, int destinationX, int destinationY, int deadline) {
        fish.addNewDestination(destinationX, destinationY, deadline);
    }

    public synchronized void setFishPosition(Fish fish, int positionX, int positionY) {
        fish.setPosition(positionX, positionY);
    }

    public synchronized Fish getFish(String name) throws IllegalArgumentException {
        for (Fish fish : fishesList) {
            if (fish.getName().equals(name)) {
                return fish;
            }
        }
        throw new IllegalArgumentException("Fish not found");
    }

    public synchronized void startFish(String name) throws IllegalArgumentException {
        Fish fish = getFish(name);
        fish.start();
    }

}
