package aquarium;

import java.time.Instant;
import java.util.LinkedList;

class FishDestination {
    private final Coordinates destination;
    private final long deadline;
    private boolean displayed = false;

    public FishDestination(int destinationX, int destinationY, int movementDuration) {
        this.destination = new Coordinates(destinationX, destinationY);
        this.deadline = movementDuration + (int) Instant.now().getEpochSecond();
    }

    public Coordinates getDestination() {
        return destination;
    }

    public long getDeadline() {
        return deadline;
    }

    public boolean isDisplayed() {
        return displayed;
    }

    public void setDisplayed() {
        this.displayed = true;
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof FishDestination)) {
            return false;
        }
        FishDestination destination = (FishDestination) o;
        return this.destination.equals(destination.getDestination()) && this.deadline == destination.getDeadline();
    }
}

public class Fish {
    enum statusEnum {
        STARTED, STOPPED
    }

    private final String name;
    private Coordinates position;
    private final int width;
    private final int height;
    private statusEnum status;
    private LinkedList<FishDestination> destinations;

    public Fish(String name, int positionX, int positionY, int width, int height) {
        this.name = name;
        this.position = new Coordinates(positionX, positionY);
        this.destinations = new LinkedList<FishDestination>();
        this.width = width;
        this.height = height;
        this.status = statusEnum.STOPPED;
    }

    public synchronized String getName() {
        return name;
    }

    public synchronized Coordinates getPosition() {
        return position;
    }

    public synchronized Coordinates getFirstDestination() {
        return destinations.getFirst().getDestination();
    }

    public int getWidth() {// width
        return width;
    }

    public int getHeight() {
        return height;
    }

    public synchronized long getTimeToGetToFirstDestination() {
        return destinations.getFirst().getDeadline();
    }

    /* TODO: remove this method */
    public statusEnum getStatus() {
        return status;
    }

    public boolean isStarted() {
        return status == statusEnum.STARTED;
    }

    public synchronized void setPosition(int positionX, int positionY) {
        this.position = new Coordinates(positionX, positionY);
    }

    public synchronized void addNewDestination(int destinationX, int destinationY, int movementDuration) {
        FishDestination newDestination = new FishDestination(destinationX, destinationY, movementDuration);
        if (!destinations.contains(newDestination)) {
            destinations.addLast(newDestination);
        }
    }

    public synchronized void removeExpiredDestinations() {
        // while (!destinations.isEmpty()
        // && destinations.getFirst().getDeadline() <= Instant.now().getEpochSecond()
        // && destinations.getFirst().isDisplayed()) {
        while (!destinations.isEmpty() && destinations.getFirst().isDisplayed()) {
            destinations.removeFirst();
        }
    }

    public synchronized int getSizeDestinations() {
        /* Only counts STARTED fishes */
        return getStatus() == statusEnum.STARTED ? destinations.size() : -1;
    }

    public synchronized void setDisplayed() {
        destinations.getFirst().setDisplayed();
    }

    public void start() {
        this.status = statusEnum.STARTED;
    }

    @Override
    public String toString() {
        return "Fish " + name + " at " + position + ", " + width + "x" + height + ", "
                + (status == statusEnum.STARTED ? "started" : "notstarted");
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof Fish)) {
            return false;
        }
        Fish fish = (Fish) o;
        return this.name.equals(fish.getName());
    }
}
