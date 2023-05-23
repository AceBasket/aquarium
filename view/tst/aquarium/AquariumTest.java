package aquarium;

public class AquariumTest {
    void removeAllFishes() {
        Aquarium aquarium = Aquarium.getInstance();
        aquarium.getFishes().clear();
    }

    public static void main(String[] args) {
        boolean missingAssertion = false;
        assert missingAssertion = true;

        if (!missingAssertion) {
            System.out.println("Option -ea needed");
            return;
        }
        AquariumTest test = new AquariumTest();
        System.out.print('.');
        test.testAddFish();
        System.out.print('.');
        test.testRemoveFish();
        System.out.print('.');
        test.testSetFishPosition();
        System.out.print('.');
        test.testStartFish();
        System.out.println(" OK");
    }

    void testAddFish() {
        removeAllFishes();
        Aquarium aquarium = Aquarium.getInstance();
        Fish fish = new Fish("Nemo", 0, 0, 1, 1);
        aquarium.addFishWithoutImage(fish);
        assert aquarium.getFishes().contains(fish);
        Fish fish2 = new Fish("Nemo", 0, 0, 1, 1);
        try {
            aquarium.addFishWithoutImage(fish2);
            assert false;
        } catch (IllegalArgumentException e) {
            assert true;
        }
    }

    void testRemoveFish() {
        removeAllFishes();
        Aquarium aquarium = Aquarium.getInstance();
        Fish fish = new Fish("Nemo", 0, 0, 1, 1);
        aquarium.addFishWithoutImage(fish);
        aquarium.removeFishWithoutImage("Nemo");
        assert !aquarium.getFishes().contains(fish);
        try {
            aquarium.removeFishWithoutImage("Dori");
            assert false;
        } catch (IllegalArgumentException e) {
            assert true;
        }
    }

    void testGetFish() {
        removeAllFishes();
        Aquarium aquarium = Aquarium.getInstance();
        Fish fish = new Fish("Nemo", 0, 0, 1, 1);
        aquarium.addFishWithoutImage(fish);
        assert aquarium.getFish("Nemo").equals(fish);
        try {
            aquarium.getFish("Dori");
            assert false;
        } catch (IllegalArgumentException e) {
            assert true;
        }
    }

    void testSetFishDestination() {
        removeAllFishes();
        Aquarium aquarium = Aquarium.getInstance();
        Fish fish = new Fish("Nemo", 0, 0, 1, 1);
        aquarium.addFishWithoutImage(fish);
        aquarium.setFishDestination(fish, 1, 1, 1);
        assert fish.getFirstDestination().equals(new Coordinates(1, 1));
    }

    void testSetFishPosition() {
        removeAllFishes();
        Aquarium aquarium = Aquarium.getInstance();
        Fish fish = new Fish("Nemo", 0, 0, 1, 1);
        aquarium.addFishWithoutImage(fish);
        aquarium.setFishPosition(fish, 1, 1);
        assert fish.getPosition().equals(new Coordinates(1, 1));
    }

    void testStartFish() {
        removeAllFishes();
        Aquarium aquarium = Aquarium.getInstance();
        Fish fish = new Fish("Nemo", 0, 0, 1, 1);
        aquarium.addFishWithoutImage(fish);
        aquarium.startFish("Nemo");
        assert fish.getStatus().equals(Fish.statusEnum.STARTED);
    }
}
