#ifndef _CFG_FILE_PARSER_H_
#define _CFG_FILE_PARSER_H_

#include "parser.h"

/* returns the parsed elements from the file f */
struct parse *parse_file(FILE *f);

#endif // _CFG_FILE_PARSER_H_