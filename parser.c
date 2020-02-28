#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename
          struct matrix * transform,
          struct matrix * pm,
          screen s
Returns:

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix -
               takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
         ident: set the transform matrix to the identity matrix -
         scale: create a scale matrix,
                then multiply the transform matrix by the scale matrix -
                takes 3 arguments (sx, sy, sz)
         translate: create a translation matrix,
                    then multiply the transform matrix by the translation matrix -
                    takes 3 arguments (tx, ty, tz)
         rotate: create a rotation matrix,
                 then multiply the transform matrix by the rotation matrix -
                 takes 2 arguments (axis, theta) axis should be x y or z
         apply: apply the current transformation matrix to the edge matrix
         display: clear the screen, then
                  draw the lines of the edge matrix to the screen
                  display the screen
         save: clear the screen, then
               draw the lines of the edge matrix to the screen
               save the screen to a file -
               takes 1 argument (file name)
         quit: end parsing

See the file script for an example of the file format

IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  clear_screen(s);
  color c;
  c.red = 255;
  c.green = 255;
  c.blue = 255;

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");

  enum script_function current_fxn;
  double args[6]; // the maximum number of number args is 6
  char c_arg;
  struct matrix *indiv_transform;
  
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);
    // NEW COMMAND STARTS
    if      (!strcmp(line,"line")){
      // command LINE: don't do anything but indicate function for next line's args
      current_fxn = FN_LINE;
    }else if(!strcmp(line,"ident")){
      // command IDENT
      ident(transform);
    }else if(!strcmp(line,"scale")){
      // command SCALE: wait for args, next line
      current_fxn = FN_SCALE;
    }else if(!strcmp(line,"move")){
      // command MOVE: wait for args, next line
      current_fxn = FN_MOVE;
    }else if(!strcmp(line,"rotate")){
      // command ROTATE: wait for args, next line
      current_fxn = FN_ROTATE;
    }else if(!strcmp(line,"apply")){
      // command APPLY
      matrix_mult(transform,edges);
    }else if(!strcmp(line,"display")){
      // command DISPLAY
      clear_screen(s);
      draw_lines(edges,s,c);
    }else if(!strcmp(line,"save")){
      // command SAVE: wait for args, next line
      current_fxn = FN_SAVE;
    }else{
      // if we get here, we don't have a new function call; use args!
      switch(current_fxn){

      case FN_LINE:
	sscanf(line,"%lf %lf %lf %lf %lf %lf",&args[0],&args[1],&args[2],&args[3],&args[4],&args[5]);
	add_edge(edges,args[0],args[1],args[2],args[3],args[4],args[5]);
	break;
      case FN_MOVE:
	sscanf(line,"%lf %lf %lf",&args[0],&args[1],&args[2]);
	// free_matrix(transform)
	indiv_transform = make_translate(args[0],args[1],args[2]);
	matrix_mult(indiv_transform,transform);
	break;
      case FN_SCALE:
	sscanf(line,"%lf %lf %lf",&args[0],&args[1],&args[2]);
	indiv_transform = make_scale(args[0],args[1],args[2]);
	matrix_mult(indiv_transform,transform);
	break;
      case FN_ROTATE:
	sscanf(line,"%c %lf",&c_arg,&args[0]);
	args[0] = args[0] * M_PI / 180.0;
	free_matrix(transform);
	// print_matrix(transform);
	switch(c_arg){
	case 'x':
	  indiv_transform = make_rotX(args[0]);
	  matrix_mult(indiv_transform,transform);
	  break;
	case 'y':
	  indiv_transform = make_rotY(args[0]);
	  matrix_mult(indiv_transform,transform);
	  break;
	case 'z':
	  indiv_transform = make_rotZ(args[0]);
	  matrix_mult(indiv_transform,transform);
	  break;
	}
	break;
      case FN_SAVE:
	save_extension(s,line);
	break;
      }
    }
    print_matrix(transform);
  }
}
  
