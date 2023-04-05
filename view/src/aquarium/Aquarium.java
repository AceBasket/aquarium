package aquarium;
import java.util.ArrayList;

public class Aquarium {
    private ArrayList<Fish> fishes_list;

    public Aquarium() {
        fishes_list = new ArrayList<Fish>();
    }

    public void addFish(Fish fish) {
        fishes_list.add(fish);
    }

    public void removeFish(String name) throws IllegalArgumentException {
        Fish fish = getFish(name);
        if (fish == null) {
            throw new IllegalArgumentException("Fish not found");
        }
        fishes_list.remove(fish);
    }

    public ArrayList<Fish> getFishes() {
        return fishes_list;
    }

    public void setFishDestination(Fish fish, int destinationX, int destinationY, int movementDuration) {
        fish.setDestination(destinationX, destinationY, movementDuration);
    }

    public void setFishPosition(Fish fish, int positionX, int positionY) {
        fish.setPosition(positionX, positionY);
    }

    public Fish getFish(String name) throws IllegalArgumentException {
        for (Fish fish : fishes_list) {
            if (fish.getName().equals(name)) {
                return fish;
            }
        }
        throw new IllegalArgumentException("Fish not found");
    }

    public void status() {
        System.out.println("-> OK : Connecté au contrôleur, " + fishes_list.size() + " poissons trouvés");
        for (Fish fish : fishes_list) {
            String status = fish.getStatus() == statusEnum.STOPPED ? "not started" : "started";
            System.out.println("Fish " + fish.getName() + " at " + fish.getPositionX() + "x" + fish.getPositionY() + ", " + fish.getLength() + ", " + fish.getHeight() + " " + status);
        }
    }

    public void startFish(String name) throws IllegalArgumentException {
        Fish fish = getFish(name);
        fish.start();
    }
    
}
