#ifndef PARSER_H
#define PARSER_H

#include "matrix.h"
#include "ml6.h"

enum script_function {
  // only includes the multi line functions (those with args)
  FN_LINE,
  FN_SCALE,
  FN_MOVE,
  FN_ROTATE,
  FN_SAVE
};

void parse_file ( char * filename, 
		  struct matrix * transform, 
		  struct matrix * edges,
		  screen s);

#endif
