#ifndef _PROMPT_PARSER_H_
#define _PROMPT_PARSER_H_

#include "parser.h"

/* returns the parsed elements read on the prompt */
struct parse *parse_prompt(char *str);

#endif // _PROMPT_PARSER_H_