package aquarium;

public class AquariumTest {
    public static void main(String[] args) {
        boolean missingAssertion = false;
        assert missingAssertion = true;

        if (!missingAssertion) {
            System.out.println("Option -ea needed");
            return;
        }
        AquariumTest test = new AquariumTest();
        int nbTest = 0;
        System.out.print('.');
        nbTest++;
        test.testAddFish();
        System.out.print('.');
        nbTest++;
        test.testRemoveFish();
        System.out.print('.');
        nbTest++;
        test.testSetFishPosition();
        System.out.print('.');
        nbTest++;
        test.testStartFish();
        System.out.println("(AquariumTest) All " + nbTest + " tests passed");
    }

    void testAddFish() {
        Aquarium aquarium = new Aquarium();
        Fish fish = new Fish("Nemo", 0, 0, 1, 1);
        aquarium.addFish(fish);
        assert aquarium.getFishes().contains(fish);
    }

    void testRemoveFish() {
        Aquarium aquarium = new Aquarium();
        Fish fish = new Fish("Nemo", 0, 0, 1, 1);
        aquarium.addFish(fish);
        aquarium.removeFish("Nemo");
        assert !aquarium.getFishes().contains(fish);
    }

    void testGetFish() {
        Aquarium aquarium = new Aquarium();
        Fish fish = new Fish("Nemo", 0, 0, 1, 1);
        aquarium.addFish(fish);
        assert aquarium.getFish("Nemo").equals(fish);
    }

    void testSetFishDestination() {
        Aquarium aquarium = new Aquarium();
        Fish fish = new Fish("Nemo", 0, 0, 1, 1);
        aquarium.addFish(fish);
        aquarium.setFishDestination(fish, 1, 1, 1);
        assert fish.getFirstDestination().equals(new Coordinates(1, 1));
    }

    void testSetFishPosition() {
        Aquarium aquarium = new Aquarium();
        Fish fish = new Fish("Nemo", 0, 0, 1, 1);
        aquarium.addFish(fish);
        aquarium.setFishPosition(fish, 1, 1);
        assert fish.getPosition().equals(new Coordinates(1, 1));
    }

    void testStartFish() {
        Aquarium aquarium = new Aquarium();
        Fish fish = new Fish("Nemo", 0, 0, 1, 1);
        aquarium.addFish(fish);
        aquarium.startFish("Nemo");
        assert fish.getStatus().equals(Fish.statusEnum.STARTED);
    }
}
