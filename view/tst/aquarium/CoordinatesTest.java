package aquarium;

public class CoordinatesTest {
    public static void main(String[] args) {
        boolean missingAssertion = false;
        assert missingAssertion = true;

        if (!missingAssertion) {
            System.out.println("Option -ea needed");
            return;
        }
        CoordinatesTest test = new CoordinatesTest();
        int nbTest = 0;
        System.out.print('.');
        nbTest++;
        test.testGetX();
        System.out.print('.');
        nbTest++;
        test.testGetY();
        System.out.print('.');
        nbTest++;
        test.testEquals();
        System.out.print('.');
        nbTest++;
        test.testDistance();
        System.out.println("(CoordinatesTest) All " + nbTest + " tests passed");
    }

    void testGetX() {
        Coordinates c = new Coordinates(1, 2);
        assert c.getX() == 1;
    }

    void testGetY() {
        Coordinates c = new Coordinates(1, 2);
        assert c.getY() == 2;
    }

    void testEquals() {
        Coordinates c1 = new Coordinates(1, 2);
        Coordinates c2 = new Coordinates(1, 2);
        Coordinates c3 = new Coordinates(2, 1);
        assert c1.equals(c2);
        assert !c1.equals(c3);
    }

    void testDistance() {
        Coordinates c1 = new Coordinates(1, 2);
        Coordinates c2 = new Coordinates(1, 2);
        Coordinates c3 = new Coordinates(2, 1);
        assert c1.distance(c2) == 0;
        assert c1.distance(c3) == Math.sqrt(2);
    }
}
