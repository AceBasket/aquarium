package aquarium;

import java.time.Instant;

public class FishTest {
    public static void main(String[] args) {
        boolean missingAssertion = false;
        assert missingAssertion = true;

        if (!missingAssertion) {
            System.out.println("Option -ea needed");
            return;
        }
        FishTest test = new FishTest();
        int nbTest = 0;
        System.out.print('.');
        nbTest++;
        test.testGetName();
        System.out.print('.');
        nbTest++;
        test.testGetPosition();
        System.out.print('.');
        nbTest++;
        test.testGetFirstDestination();
        System.out.print('.');
        nbTest++;
        test.testAddNewDestination();
        System.out.print('.');
        nbTest++;
        test.testStart();
        System.out.print('.');
        nbTest++;
        test.testSetPosition();
        System.out.print('.');
        nbTest++;
        test.testGetStatus();
        System.out.print('.');
        nbTest++;
        test.testGetLength();
        System.out.print('.');
        nbTest++;
        test.testGetHeight();
        // System.out.print('.');
        // nbTest++;
        // test.testGetTimeToGetToFirstDestination();
        System.out.print('.');
        nbTest++;
        test.testGetSizeDestinations();
        System.out.print('.');
        nbTest++;
        test.testRemoveExpiredDestinations();

        System.out.println("(FishTest) All " + nbTest + " tests passed");
    }

    void testGetName() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        assert f.getName().equals("Nemo");
    }

    void testGetPosition() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        assert f.getPosition().equals(new Coordinates(1, 2));
    }

    void testGetFirstDestination() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        f.addNewDestination(3, 4, 5);
        assert f.getFirstDestination().equals(new Coordinates(3, 4));
    }

    void testAddNewDestination() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        f.addNewDestination(3, 4, 5);
        assert f.getSizeDestinations() == 1;
        assert f.getFirstDestination().equals(new Coordinates(3, 4));
    }

    void testStart() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        f.addNewDestination(3, 4, 5);
        f.start();
        assert f.getStatus() == Fish.statusEnum.STARTED;
    }

    void testSetPosition() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        f.setPosition(3, 4);
        assert f.getPosition().equals(new Coordinates(3, 4));
    }

    void testRemoveExpiredDestinations() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        f.addNewDestination(3, 4, 0);
        f.addNewDestination(5, 6, 1);
        assert f.getSizeDestinations() == 2;
        f.removeExpiredDestinations();
        assert f.getFirstDestination().equals(new Coordinates(5, 6));
        try {
            Thread.sleep(1050);
        } catch (Exception e) {
        }
        f.removeExpiredDestinations();
        assert f.getSizeDestinations() == 0;
    }

    void testGetSizeDestinations() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        f.addNewDestination(3, 4, 1);
        f.addNewDestination(5, 6, 2);
        assert f.getSizeDestinations() == 2;
    }

    void testGetLength() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        assert f.getLength() == 3;
    }

    void testGetHeight() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        assert f.getHeight() == 4;
    }

    void testGetStatus() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        assert f.getStatus() == Fish.statusEnum.STOPPED;
    }

    // void testGetTimeToGetToFirstDestination() {
    // Fish f = new Fish("Nemo", 1, 2, 3, 4);
    // f.addNewDestination(3, 4, 5);
    // assert f.getTimeToGetToFirstDestination() < Instant.now().getEpochSecond() +
    // 5;
    // assert Instant.now().getEpochSecond() + 4 <
    // f.getTimeToGetToFirstDestination();
    // }

}
