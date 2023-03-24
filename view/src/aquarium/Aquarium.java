package aquarium;

public class Aquarium {
    private Fish[][] aquarium;

    public Aquarium(int width, int height) {
        this.aquarium = new Fish[width][height];
    }

    public int getWidth() {
        return aquarium.length;
    }

    public int getHeight() {
        return aquarium[0].length;
    }

    public Fish getFishFromAquarium(int x, int y) {
        return aquarium[x][y];
    }
}
