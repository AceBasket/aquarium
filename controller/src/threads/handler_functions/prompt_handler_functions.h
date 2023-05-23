#ifndef _PROMPT_HANDLER_FUNCTIONS_H_
#define _PROMPT_HANDLER_FUNCTIONS_H_

#include "../../parser/prompt_parser.h"
#include "../../parser/file_parser.h"
#include "../../aquarium/aquarium.h"

/* loads the aquarium */
void load_handler(FILE *log, struct parse *parser, struct aquarium **aquarium);
/* shows the aquarium */
void show_handler(FILE *log, struct aquarium *aquarium);
/* adds a view in the aquarium */
void add_view_handler(FILE *log, struct parse *parser, struct aquarium *aquarium);
/* deletes a view from the aquarium */
void del_view_handler(FILE *log, struct parse *parser, struct aquarium *aquarium);
/* saves the aquarium in a file */
void save_handler(FILE *log, struct parse *parser, struct aquarium *aquarium);

#endif // _PROMPT_HANDLER_FUNCTIONS_H_