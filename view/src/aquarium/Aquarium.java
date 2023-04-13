package aquarium;
import java.util.ArrayList;

public class Aquarium {
    private ArrayList<Fish> fishesList;

    public Aquarium() {
        fishesList = new ArrayList<Fish>();
    }

    public void addFish(Fish fish) {
        fishesList.add(fish);
    }

    public void removeFish(String name) throws IllegalArgumentException {
        Fish fish = getFish(name);
        if (fish == null) {
            throw new IllegalArgumentException("Fish not found");
        }
        fishesList.remove(fish);
    }

    public ArrayList<Fish> getFishes() {
        return fishesList;
    }

    public void setFishDestination(Fish fish, int destinationX, int destinationY, int movementDuration) {
        fish.setDestination(destinationX, destinationY, movementDuration);
    }

    public void setFishPosition(Fish fish, int positionX, int positionY) {
        fish.setPosition(positionX, positionY);
    }

    public Fish getFish(String name) throws IllegalArgumentException {
        for (Fish fish : fishesList) {
            if (fish.getName().equals(name)) {
                return fish;
            }
        }
        throw new IllegalArgumentException("Fish not found");
    }

    public void status() {
        System.out.println("-> OK : Connecté au contrôleur, " + fishesList.size() + " poissons trouvés");
        for (Fish fish : fishesList) {
            String status = fish.getStatus() == statusEnum.STOPPED ? "not started" : "started";
            System.out.println("Fish " + fish.getName() + " at " + fish.getPosition().getX() + "x" + fish.getPosition().getY() + ", " + fish.getLength() + ", " + fish.getHeight() + " " + status);
        }
    }

    public void startFish(String name) throws IllegalArgumentException {
        Fish fish = getFish(name);
        fish.start();
    }
    
}
