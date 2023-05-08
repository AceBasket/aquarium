#ifndef _PROMPT_HANDLER_FUNCTIONS_H_
#define _PROMPT_HANDLER_FUNCTIONS_H_

#include "../../aquarium/aquarium.h"
#include "../../parser/prompt_parser.h"
#include "../../parser/cfg_file_parser.h"
// #include "utils.h"

void load_handler(FILE *log, struct parse *parser, struct aquarium **aquarium);
void show_handler(FILE *log, struct aquarium *aquarium);
void add_view_handler(FILE *log, struct parse *parser, struct aquarium *aquarium);
void del_view_handler(FILE *log, struct parse *parser, struct aquarium *aquarium);
void save_handler(FILE *log, struct parse *parser, struct aquarium *aquarium);

#endif // _PROMPT_HANDLER_FUNCTIONS_H_