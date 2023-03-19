public class Fish {
    private final String name;
    private int positionX;
    private int positionY;
    private int destinationX;
    private int destinationY;
    private final int length;
    private final int height;
    private String movement;

    public Fish(String name, int positionX, int positionY, int destinationX, int destinationY, int length, int height, String movement) {
        this.name = name;
        this.positionX = positionX;
        this.positionY = positionY;
        this.destinationX = destinationX;
        this.destinationY = destinationY;
        this.length = length;
        this.height = height;
        this.movement = movement;
    }

    public String getName() {
        return name;
    }

    public int getPosition() {
        return positionX;
    }

    public int getPositionY() {
        return positionY;
    }

    public int getDestinationX() {
        return destinationX;
    }

    public int getDestinationY() {
        return destinationY;
    }

    public int getLength() {
        return length;
    }

    public int getHeight() {
        return height;
    }

    public String getMovement() {
        return movement;
    }
}

