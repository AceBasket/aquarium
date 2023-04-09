package aquarium;
enum statusEnum { STARTED, STOPPED }
enum movementEnum { RANDOM }

public class Fish {
    private final String name;
    private int positionX;
    private int positionY;
    private int destinationX;
    private int destinationY;
    private final int length;
    private final int height;
    private movementEnum movement;
    private int movementDuration;
    private statusEnum status;

    public Fish(String name, int positionX, int positionY, int destinationX, int destinationY, int length, int height, String movement, int movementDuration) throws IllegalArgumentException {
        if (movement.equals("RandomWaypoint")) {
            this.movement = movementEnum.RANDOM;
        } else{
            throw new IllegalArgumentException("Unknown movement");
        }
        this.name = name;
        this.positionX = positionX;
        this.positionY = positionY;
        this.destinationX = destinationX;
        this.destinationY = destinationY;
        this.length = length;
        this.height = height;
        this.movementDuration = movementDuration;
        this.status = statusEnum.STOPPED;
    }

    public Fish(String name, int positionX, int positionY, int length, int height, String movement) throws IllegalArgumentException{
        if (movement.equals("RandomWaypoint")) {
            this.movement = movementEnum.RANDOM;
        } else{
            throw new IllegalArgumentException("Unknown movement");
        }
        this.name = name;
        this.length = length;
        this.height = height;
        this.status = statusEnum.STOPPED;
    }

    public String getName() {
        return name;
    }

    public int getPositionX() {
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
        if (movement == movementEnum.RANDOM) {
            return "RandomWaypoint";
        } else {
            return "Unknown";
        }
    }

    public int getMovementDuration() {
        return movementDuration;
    }
    
    public statusEnum getStatus() {
        return status;
    }

    public void setPosition(int positionX, int positionY) {
        this.positionX = positionX;
        this.positionY = positionY;
    }

    public void setDestination(int destinationX, int destinationY, int movementDuration) {
        this.destinationX = destinationX;
        this.destinationY = destinationY;
        this.movementDuration = movementDuration;
    }

    public void start() {
        this.status = statusEnum.STARTED;
    }
}

