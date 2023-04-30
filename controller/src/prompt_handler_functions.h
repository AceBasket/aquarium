#ifndef _PROMPT_HANDLER_FUNCTIONS_H_
#define _PROMPT_HANDLER_FUNCTIONS_H_

#include "aquarium.h"
#include "utils.h"

void load_handler(struct parse *parser, struct aquarium **aquarium);
void show_handler(struct aquarium *aquarium);
void add_view_handler(struct parse *parser, struct aquarium *aquarium);
void del_view_handler(struct parse *parser, struct aquarium *aquarium);
void save_handler(struct parse *parser, struct aquarium *aquarium);

#endif // PROMPT_HANDLER_FUNCTIONS_H_