package threads;

import aquarium.Aquarium;
import aquarium.Fish;
import aquarium.View;
import utils.ServerResponseParserResult;

public class ServerThreadHandlers {
    public static void doHello(View view) {
        if (view.getId().equals("")) {
            view.talkToServer("hello");
        } else {
            view.talkToServer("hello in as " + view.getId());
        }
    }

    public static void doLs(View view) {
        view.talkToServer("ls");
    }

    public static void doGetFishesContinuously(View view) {
        view.talkToServer("getFishesContinuously");
    }

    public static void listHandler(View view, Aquarium fishesList, ServerResponseParserResult parsedAnswer) {
        for (int i = 0; i < parsedAnswer.getArgs().size(); i += 4) {
            try {
                Fish fish_to_update = fishesList.getFish(parsedAnswer.getArgs().get(i));
                fishesList.setFishDestination(fish_to_update, Integer.parseInt(parsedAnswer.getArgs().get(i + 1)),
                        Integer.parseInt(parsedAnswer.getArgs().get(i + 2)),
                        Integer.parseInt(parsedAnswer.getArgs().get(i + 3)));
            } catch (IllegalArgumentException e) {
                Fish fish_to_create = new Fish(parsedAnswer.getArgs().get(i),
                        Integer.parseInt(parsedAnswer.getArgs().get(i + 1)), // position.x
                        Integer.parseInt(parsedAnswer.getArgs().get(i + 1)), // position.y
                        Integer.parseInt(parsedAnswer.getArgs().get(i + 2)), // destination.x
                        Integer.parseInt(parsedAnswer.getArgs().get(i + 2)), // destination.y
                        Integer.parseInt(parsedAnswer.getArgs().get(i + 3)), // width
                        Integer.parseInt(parsedAnswer.getArgs().get(i + 4)), // height
                        Integer.parseInt(parsedAnswer.getArgs().get(i + 5))); // timeToDestination
                fishesList.addFish(fish_to_create);
            }
        }
    }
}
