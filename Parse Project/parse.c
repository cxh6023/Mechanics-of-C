/*
** parse.c - parsing function for the log file report program
**
** Author:       A. N. Onymous
** Contributor:  YOUR_NAME_HERE
*/

#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

#include "parse.h"

/*
** Private functions
*/

static char *duplicate( const char *str ) {
    int len = strlen(str); //was sizeof(str), but this only returns the size of the data type, which in this case was a pointer, so 8 bytes. 
    char *buf = (char *) malloc( len + 1 );

    strcpy( buf, str ); 
    return( buf );
}

/*
** Public functions
*/

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
**  via 'fields':  pointers to dynamically-allocated strings
**
** the caller is responsible for freeing the returned strings
*/

int parse( char *str, char *fields[], size_t count ) {
    char *buf;

    // use our own string pointer

    buf = str;

    //
    // lines have the following format:
    //
    //     f1,f2,f3,f4,f5
    //
    // fields are separate by single ',' characters
    //
    // scan the line the old fashioned way, using character pointers
    //

    char *start, *curr;

    size_t field = 0;  // field being processed right now

    // iterate through the fields, dealing with each one in turn

    start = curr = buf;

    while( field < count ) {

        // fields except for the last one end at a comma;
        // the last field ends at the end of the string,
        // or if a newline is encountered

        while( *curr && *curr != ',' && *curr != '\n' ) {
            ++curr;
        }

        char ch = *curr;  // remember the end character

        *curr++ = '\0';   // replace it with a NUL

        fields[field++] = duplicate(start);    // duplicate the field

        start = curr;     // move on

        if( ch != ',' ) {  // found the end of the string?
            break;
        }
    }

    // all done - the index of the next available element
    // is the number of things already in the array

    return( field );

}
