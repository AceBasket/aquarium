package aquarium;

import java.util.ArrayList;

public class Aquarium {
    private ArrayList<Fish> fishesList;

    public Aquarium() {
        fishesList = new ArrayList<Fish>();
    }

    public synchronized void addFish(Fish fish) {
        if (!fishesList.contains(fish)) {
            fishesList.add(fish);
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
    }

    public synchronized ArrayList<Fish> getFishes() {
        return fishesList;
    }

    public synchronized void setFishDestination(Fish fish, int destinationX, int destinationY, int movementDuration) {
        fish.addNewDestination(destinationX, destinationY, movementDuration);
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
