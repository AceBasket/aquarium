#include "prompt_handler_functions.h"
#include "view.h"

void load_handler(struct parse *parser, struct aquarium **aquarium) {
    FILE *fd = fopen(parser->arguments[0], "r");
    if (fd == NULL) {
        fprintf(stderr, "ERROR: opening file\n");
        return;
    }

    struct parse *parsed_file = parse_file(fd);
    struct coordinates coord;
    struct view *view;

    *aquarium = create_aquarium(atoi(parsed_file->arguments[0]), atoi(parsed_file->arguments[1]));
    exit_if(*aquarium == NULL || (*aquarium)->width == 0 || (*aquarium)->height == 0, "ERROR creating aquarium\n");
    if (*aquarium == NULL || (*aquarium)->width == 0 || (*aquarium)->height == 0) {
        fprintf(stderr, "ERROR: creating aquarium\n");
        return;
    }

    for (int i = 2; i < parsed_file->size; i += 5) {
        coord.x = atoi(parsed_file->arguments[i + 1]);
        coord.y = atoi(parsed_file->arguments[i + 2]);
        view = create_view(parsed_file->arguments[i], coord, atoi(parsed_file->arguments[i + 3]), atoi(parsed_file->arguments[i + 4]));
        if (view == NULL) {
            fprintf(stderr, "ERROR: creating view\n");
            return;
        }
        if (!add_view(*aquarium, view)) {
            fprintf(stderr, "ERROR: adding view\n");
            return;
        }
    }
    fprintf(stdout, "Aquarium loaded (%d display view)\n", len_views(*aquarium));
}

void show_handler(struct aquarium *aquarium) {
    if (aquarium == NULL) {
        fprintf(stderr, "ERROR: no aquarium\n");
        return;
    }
    fprintf(stdout, "Showing aquarium\n");
    show_aquarium(aquarium, stdout);
}

void add_view_handler(struct parse *parser, struct aquarium *aquarium) {
    struct coordinates coord = {atoi(parser->arguments[2]), atoi(parser->arguments[3])}; 
    struct view *view = create_view(parser->arguments[1], coord, atoi(parser->arguments[4]), atoi(parser->arguments[5]));
    add_view(aquarium, view);
    fprintf(stdout, "View added\n");
}

void del_view_handler(struct parse *parser, struct aquarium *aquarium) {
    remove_view(aquarium, get_view(aquarium, parser->arguments[1]));
    fprintf(stdout, "View %s deleted\n", parser->arguments[1]);
}

void save_handler(struct parse *parser, struct aquarium *aquarium) {
    if (aquarium == NULL) {
        fprintf(stderr, "ERROR: no aquarium\n");
        return;
    }
    save_aquarium(aquarium, parser->arguments[0]);
    fprintf(stdout, "Aquarium saved (%d display view)\n", len_views(aquarium));
}
