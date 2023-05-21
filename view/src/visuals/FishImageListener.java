package visuals;

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
            if (change.wasAdded()) {
                for (FishImage fishImage : change.getAddedSubList()) {
                    // Perform actions for each added FishImage
                    ImageView imageView = fishImage.getImageView();
                    pane.getChildren().add(imageView);
                }
            }

            if (change.wasRemoved()) {
                for (FishImage fishImage : change.getRemoved()) {
                    // Perform actions for each removed FishImage
                    ImageView imageView = fishImage.getImageView();

                    pane.getChildren().remove(imageView);
                }
            }
        }
    }
}
