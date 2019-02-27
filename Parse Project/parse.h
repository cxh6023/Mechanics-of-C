/*
** parse.h - parse function for the log file report program
**
** Author:       A. N. Onymous
** Contributor:  YOUR_NAME_HERE
*/

#ifndef _PARSE_H_
#define _PARSE_H_

// need size_t declaration
#include <stddef.h>

/*
** parse() - parse a comma-separated string of up to 'count' fields
**
** params:
**    str     string to be parsed
**    fields  array of pointers into which fields are extracted
**    count   number of elements in fields[]
**
** return:
**    intrinsic:  count of fields converted, or -1 on error
**    via 'fields':  pointers to dynamically-allocated strings
**
** the caller is responsible for freeing the returned strings
*/

int parse( char *str, char *fields[], size_t count );

#endif
