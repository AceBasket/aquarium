package aquarium;

public class FishTest {
    public static void main(String[] args) {
        boolean missingAssertion = false;
        assert missingAssertion = true;

        if (!missingAssertion) {
            System.out.println("Option -ea needed");
            return;
        }
        FishTest test = new FishTest();
        System.out.print('.');
        test.testGetName();
        System.out.print('.');
        test.testGetPosition();
        System.out.print('.');
        test.testGetFirstDestination();
        System.out.print('.');
        test.testAddNewDestination();
        System.out.print('.');
        test.testStart();
        System.out.print('.');
        test.testSetPosition();
        System.out.print('.');
        test.testGetStatus();
        System.out.print('.');
        test.testGetLength();
        System.out.print('.');
        test.testGetHeight();
        System.out.print('.');
        test.testGetTimeToGetToFirstDestination();
        System.out.print('.');
        test.testGetSizeDestinations();
        System.out.print('.');
        test.testRemoveExpiredDestinations();

        System.out.println(" OK");
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
        f.start();
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
        f.start();
        assert f.getSizeDestinations() == 2;
        f.setDisplayedFirstDestination();
        f.removeExpiredDestinations();
        assert f.getFirstDestination().equals(new Coordinates(5, 6));
        f.setDisplayedFirstDestination();
        f.removeExpiredDestinations();
        assert f.getSizeDestinations() == 0;
    }

    void testGetSizeDestinations() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        f.addNewDestination(3, 4, 1);
        f.addNewDestination(5, 6, 2);
        f.start();
        assert f.getSizeDestinations() == 2;
    }

    void testGetLength() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        assert f.getWidth() == 3;
    }

    void testGetHeight() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        assert f.getHeight() == 4;
    }

    void testGetStatus() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        assert f.getStatus() == Fish.statusEnum.STOPPED;
    }

    void testGetTimeToGetToFirstDestination() {
        Fish f = new Fish("Nemo", 1, 2, 3, 4);
        f.addNewDestination(3, 4, 5);
        assert f.getTimeToGetToFirstDestination() == System.currentTimeMillis() + 5000;
    }

}
