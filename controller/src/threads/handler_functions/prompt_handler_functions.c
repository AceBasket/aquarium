#include "prompt_handler_functions.h"
#include "../../aquarium/view.h"
#include "../../utils.h"

int handle_prompt_error(FILE *log, struct parse *parser) {
    if (strcmp(parser->status, "OK\n") != 0) {
        log_message(log, LOG_INFO, "%s", parser->status);
        return OK;
    }
    return NOK;
}

void load_handler(FILE *log, struct parse *parser, struct aquarium **aquarium) {
    if (handle_prompt_error(log, parser)) {
        return;
    }

    if (*aquarium != NULL) {
        log_message(log, LOG_WARNING, "Can't change aquarium when views are connected");
        return;
    }
    
    FILE *fd = fopen(parser->arguments[0], "r");
    if (fd == NULL) {
        log_message(log, LOG_ERROR, "Opening file");
        return;
    }

    struct parse *parsed_file = parse_file(fd);
    struct coordinates coord;
    struct view *view;

    *aquarium = create_aquarium(atoi(parsed_file->arguments[0]), atoi(parsed_file->arguments[1]));
    if (*aquarium == NULL || (*aquarium)->width == 0 || (*aquarium)->height == 0) {
        log_message(log, LOG_WARNING, "Aquarium not created");
        return;
    }

    for (int i = 2; i < parsed_file->size; i += 5) {
        coord.x = atoi(parsed_file->arguments[i + 1]);
        coord.y = atoi(parsed_file->arguments[i + 2]);
        view = create_view(parsed_file->arguments[i], coord, atoi(parsed_file->arguments[i + 3]), atoi(parsed_file->arguments[i + 4]));
        if (view == NULL) {
            log_message(log, LOG_WARNING, "View not created");
            return;
        }
        if (!add_view(*aquarium, view)) {
            log_message(log, LOG_WARNING, "View not added");
            return;
        }
    }
    fprintf(stdout, "Aquarium loaded (%d display view)\n", len_views(*aquarium));
    fclose(fd);
}

void show_handler(FILE *log, struct aquarium *aquarium) {
    if (aquarium == NULL) {
        log_message(log, LOG_WARNING, "No aquarium");
        return;
    }
    fprintf(stdout, "Showing aquarium\n");
    show_aquarium(aquarium, stdout);
}

void add_view_handler(__attribute__((unused))FILE *log, struct parse *parser, struct aquarium *aquarium) {
    if (handle_prompt_error(log, parser)) {
        return;
    }
    struct coordinates coord = { atoi(parser->arguments[1]), atoi(parser->arguments[2]) };
    struct view *view = create_view(parser->arguments[0], coord, atoi(parser->arguments[3]), atoi(parser->arguments[4]));
    if (add_view(aquarium, view) == OK) {
        fprintf(stdout, "View %s added\n", parser->arguments[0]);
    } else {
        fprintf(stdout, "View %s already in the aquarium\n", parser->arguments[0]);
    }
}

void del_view_handler(__attribute__((unused))FILE *log, struct parse *parser, struct aquarium *aquarium) {
    if (handle_prompt_error(log, parser)) {
        return;
    }
    if (remove_view(aquarium, get_view(aquarium, parser->arguments[0])) == OK) {
        fprintf(stdout, "View %s deleted\n", parser->arguments[0]);
    } else {
        fprintf(stdout, "View %s not found\n", parser->arguments[0]);
    }
}

void save_handler(FILE *log, struct parse *parser, struct aquarium *aquarium) {
    if (handle_prompt_error(log, parser)) {
        return;
    }
    if (aquarium == NULL) {
        log_message(log, LOG_WARNING, "No aquarium");
        return;
    }
    save_aquarium(aquarium, parser->arguments[0]);
    fprintf(stdout, "Aquarium saved (%d display view)\n", len_views(aquarium));
}
