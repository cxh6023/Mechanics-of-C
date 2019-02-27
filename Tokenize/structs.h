/**
 * structs.h simply holds a struct definition and a function definition.
 */

#ifndef STRUCTS_H
#define STRUCTS_H "structs.h"

int classDecider(char);

typedef struct Class{
	int state; //what state(row are you in)
	int name; //CC_WS, CC_NEWLINE, ETC...	
	char save; //s = record the character in a buffer //d = do not record the character
	int change; //the integer before the slash
} node;

#endif
