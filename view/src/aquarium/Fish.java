package aquarium;

enum statusEnum {
    STARTED, STOPPED
}

public class Fish {
    private final String name;
    private Coordinate position;
    private Coordinate destination;
    private final int length;
    private final int height;
    private int movementDuration;
    private statusEnum status;

    public Fish(String name, int positionX, int positionY, int destinationX, int destinationY, int length, int height,
            int movementDuration) {
        this.name = name;
        this.position = new Coordinate(positionX, positionY);
        this.destination = new Coordinate(destinationX, destinationY);
        this.length = length;
        this.height = height;
        this.movementDuration = movementDuration;
        this.status = statusEnum.STOPPED;
    }

    public Fish(String name, int positionX, int positionY, int length, int height) {
        this.name = name;
        this.length = length;
        this.height = height;
        this.status = statusEnum.STOPPED;
    }

    public String getName() {
        return name;
    }

    public Coordinate getPosition() {
        return position;
    }

    public Coordinate getDestination() {
        return destination;
    }

    public int getLength() {
        return length;
    }

    public int getHeight() {
        return height;
    }

    public int getMovementDuration() {
        return movementDuration;
    }

    public statusEnum getStatus() {
        return status;
    }

    public void setPosition(int positionX, int positionY) {
        this.position = new Coordinate(positionX, positionY);
    }

    public void setDestination(int destinationX, int destinationY, int movementDuration) {
        this.destination = new Coordinate(destinationX, destinationY);
        this.movementDuration = movementDuration;
    }

    public void start() {
        this.status = statusEnum.STARTED;
    }
}
