/*
** emp.c - employee record processing routines
**
** Author:       A. N. Onymous
** Contributor:  YOUR_NAME_HERE
*/

// need the getline() prototype from stdio
#define    _XOPEN_SOURCE    700

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include "emp.h"
#include "parse.h"

/*
** Global list of employees
*/

Employee *employees;
int num_employees;

/*
** Private functions
*/

/*
** Return values for add_employee()
*/

typedef enum { ADD_OK, ADD_ALLOC, ADD_DUP } Status;

/*
** add_employee() - add a new employee to the database
**
** parameters:
**    id    employee ID (unique)
**    last  last name
**    first first name
**    hire  date of hire (yyyymmdd)
**    pay   pay grade
**
** returns:
**  1 on success, otherwise 0
*/

static Status add_employee( int line, int id, char *last, char *first,
    int hire, uint8_t pay ) {
    Employee *emp, *prev, *curr;

    // see if this employee already exists
        
    emp = find_emp( id );
    if( emp != NULL ) {
        fprintf( stderr, "add_emp(): line #%d, duplicate ID '%d'", line, id );
        fputs( ", skipping new record\n", stderr );
        return( ADD_DUP );
    }

    // somebody new; allocate a new employee node
 
    emp = new_emp( id, last, first, hire, pay );
    if( emp == NULL ) {
        // no room for this one?
        fprintf( stderr, "add_emp(): can't create employee #%d\n", id );
        return( ADD_ALLOC );
    }

    //
    // insert this employee into the employee list, which
    // is ordered by ID
    //

    prev = NULL;
    curr = employees;

    while( curr && curr->id < id ) {
        prev = curr;
        curr = curr->next;
    }

    if( curr && curr->id == id ) {  // can't happen!
        fprintf( stderr, "add_emp(): duplicate id %d AFTER check!\n", id );
        exit( EXIT_FAILURE );
    }

    // at this point:
    //  curr    prev    situation   
    //  =====   =====   =======================
    //  NULL    NULL    list was empty
    //  NULL    !NULL   add at end of list
    //  !NULL   NULL    add at front of list
    //  !NULL   !NULL   add in middle of list

    if( curr == NULL ) {

        if( prev == NULL ) {   // empty list
            employees = emp;
        } else {               // add at the end of the list
            prev->next = emp;
        }

    } else {

        if( prev == NULL ) { // add at the front of the list
            emp->next = employees;
            employees = emp;
        } else {             // add in the middle of the list
            emp->next = curr;
            prev->next = emp;
        }
    }

    return( ADD_OK );

}

/*
** Public functions
*/

/*
** new_emp() - create a new employee
**
** parameters:
**    id    unique employee ID
**    last  employee last name
**    first employee first name
**    hire  date of hire (as an integer)
**    grade pay grade code
**
** return:
**    newly allocated employee record, or NULL on failure
*/

Employee *new_emp( int id, char *last, char *first, int hire, uint8_t grade ) {
    Employee *new;

    // allocate the node
    new = (Employee *) calloc( 1, sizeof(Employee) );

    // if this succeeded, fill in the fields
    if( new != NULL ) {

        // we assume that the pointers we're given can
        // just be copied into the new record
        //
        new->id = id;
        new->last = last;
        new->first = first;
        new->hire = hire;
        new->grade = grade;

        // we used calloc(), so 'next' is already NULL
    }

    return( new );
}

/*
** free_emp() - deallocate an employee node
**
** params:
**    emp    pointer to the node to deallocate
*/

void free_emp( Employee *emp ) {

    // sanity check - be sure we were given a non-NULL pointer

    if( emp ) { 
	free( emp->last ); //this frees the pointer to the pointer of the last name value
	free( emp->first ); //this frees the pointer to the pointer to the first name value
        free( emp );
    }

}

/*
** find_emp() - locate the entry for a specific employee id
**
** parameters:
**  id  the ID to locate
**
** returns:
**  a pointer to the Employee record, or NULL
*/

Employee *find_emp( int id ) {
    Employee *emp;

    emp = employees;
    while( emp && emp->id != id ) {
        emp = emp->next;
    }

    return( emp );
}

/*
** load_employees() - load the employee database
**
** params:
**    file    FILE * for the already-open input file
**
** return:
**    count of employee records loaded
*/

int load_employees( FILE *file ) {
    char *buf;
    size_t len;
    int nlines, nemps;

    nlines = nemps = 0;

    // iterate through the records in the file

    buf = NULL;
    len = 0;
    while( getline(&buf,&len,file) >= 0 ) {

        ++nlines;

        // ignore comment lines and blank/empty lines

        if( buf[0] == '#' || buf[0] == '\n' || buf[0] == '\0' ) {
            continue;
        }

        //
        // these lines have the following format:
        //
        //     id,lastname,firstname,hiredate,paygrade
        //
        // fields are separate by ',' characters; id, hiredate, and
        // paygrade are integers, names are one or more characters
        //
        // parse the line into five fields
        //

        char *fields[5]; 

        int num = parse( buf, fields, 5 );
        if( num != 5 ) {
                fprintf( stderr, "load_emp(): bad field count (%d), line %d",
                    num, nlines );
                fputs( "; ignoring line\n", stderr );
                continue;
        }

        // convert the fields as needed

	//Added fields 2, and 3 to the method
        int id, hiredate, paygrade;

        // field 1:  id
        id = (int) strtol( fields[0], NULL, 10 );
	free( fields[0] );

	char *last;
	// field 2:  last
	last = fields[1];	

	char *first;
	//field 3: first
	first = fields[2];

        // field 4:  hire date
        hiredate = (int) strtol( fields[3], NULL, 10 );
	free( fields[3] );

        // field 5:  pay grade
        paygrade = (int) strtol( fields[4], NULL, 10 );
	free( fields[4] );

        // add this employee to the database

        Status result;
        result = add_employee( nlines, id, last, first, hiredate, paygrade ); //changed the add_employee parameters so it takes in a variable and not just the field values.

        if( result == ADD_OK ) {
            ++nemps;
        }

    }

    // release the getline() buffer

    if( buf != NULL ) {
        free( buf );
    }

    num_employees = nemps;

    return( num_employees );
}

/*
** delete_all_employees() - delete all employee entries
*/

void delete_all_employees( void ) {
    Employee *emp = employees;

    while( emp ) { //was just 'while(emp)'
	
        Employee *placeHolder = emp->next; //didnt exist

	free_emp( emp );
	emp = placeHolder; //emp = emp->next;
    }
}
