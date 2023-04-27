package aquarium;

import java.time.Instant;
import java.util.LinkedList;

class FishDestination {
    private final Coordinates destination;
    private final int deadline;

    public FishDestination(int destinationX, int destinationY, int movementDuration) {
        this.destination = new Coordinates(destinationX, destinationY);
        this.deadline = movementDuration + (int) Instant.now().getEpochSecond();
    }

    public Coordinates getDestination() {
        return destination;
    }

    public int getDeadline() {
        return deadline;
    }

    public boolean equals(FishDestination destination) {
        return this.destination.equals(destination.getDestination()) && this.deadline == destination.getDeadline();
    }
}

public class Fish {
    enum statusEnum {
        STARTED, STOPPED
    }

    private final String name;
    private Coordinates position;
    private final int length;
    private final int height;
    private statusEnum status;
    private LinkedList<FishDestination> destinations;

    public Fish(String name, int positionX, int positionY, int length, int height) {
        this.name = name;
        this.position = new Coordinates(positionX, positionY);
        this.destinations = new LinkedList<FishDestination>();
        this.length = length;
        this.height = height;
        this.status = statusEnum.STOPPED;
    }

    public String getName() {
        return name;
    }

    public Coordinates getPosition() {
        return position;
    }

    public Coordinates getFirstDestination() {
        return destinations.getFirst().getDestination();
    }

    public int getLength() {
        return length;
    }

    public int getHeight() {
        return height;
    }

    public int getTimeToGetToFirstDestination() {
        return destinations.getFirst().getDeadline();
    }

    public statusEnum getStatus() {
        return status;
    }

    public void setPosition(int positionX, int positionY) {
        this.position = new Coordinates(positionX, positionY);
    }

    public void addNewDestination(int destinationX, int destinationY, int movementDuration) {
        FishDestination newDestination = new FishDestination(destinationX, destinationY, movementDuration);
        destinations.addLast(newDestination);
    }

    public void removeExpiredDestinations() {
        while (!destinations.isEmpty()
                && destinations.getFirst().getDeadline() <= (int) Instant.now().getEpochSecond()) {
            destinations.removeFirst();
        }
    }

    public int getSizeDestinations() {
        return destinations.size();
    }

    public void start() {
        this.status = statusEnum.STARTED;
    }
}
