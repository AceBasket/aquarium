package visuals;

import javafx.application.Platform;
import javafx.collections.ListChangeListener;
import javafx.scene.image.ImageView;
import javafx.scene.layout.Pane;

class FishImageListener implements ListChangeListener<FishImage> {
    private Pane pane;

    FishImageListener(Pane pane) {
        this.pane = pane;
    }

    @Override
    public void onChanged(Change<? extends FishImage> change) {
        while (change.next()) {
            if (change.wasRemoved()) {
                for (FishImage fishImage : change.getRemoved()) {
                    // Perform actions for each removed FishImage
                    ImageView imageView = fishImage.getImageView();
                    Platform.runLater(() -> pane.getChildren().remove(imageView));
                }
            }
        }
    }
}
