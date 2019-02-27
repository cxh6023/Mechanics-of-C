/**
 * Author: Chris Heist
 * tokenize.c reads in input from the argument and sorts them into a 1D array
 * This 1D Array is then analyzed one by one and arranges the structs into 
 * the correct order they should be. 
 * The program then reads the characters from the input file and goes through
 * the path created from the 2d matrix and prints the resulated tokens and/or
 * lack their of out to the user.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"
#include "classes.h"


/*
 * This function repeatedly calls the scanner to obtain tokens
 */
int main(int argc, char * argv[]){ //argc is the amount of commands

	FILE *fp;

	fp = fopen(argv[argc-1], "r");
	if(fp == NULL){
		//somethign went wrong
		perror(argv[argc-1]);
		exit(EXIT_FAILURE);
	}

	char *contents, buf[256];
	//char *ssa;
	//char *matrix;
	//int states = 0;
	int start = 0;
	//int accept = 0;
	const char seperator[2] = " ";
	char *sections;
	int notMatrixCounter = 0; //this is to count to 3 and used to seperate the matric lines from the ssa line at the start
	
	int numberOfStates = 0;
	//node matrix[75][12]; //this is the matrix, but we dont know how many states are here yet
	node nodes[75*12]; //this wil hold all the nodes, but they will inot be organized
	int nodeCounter = -1; //keep strack of how many nodes we have

	//Struct Variables are below
        int state; //what state(row)
        //int name; //CC_WS, CC_NEWLINE, etc..
        char save; //d to not save, or s to save
        //int change; //the integer before the slash

	while((contents = fgets(buf, 256, fp)) != NULL){
		sections = strtok(buf, seperator);
		while(sections != NULL){
			if(notMatrixCounter == 1){
				//states = atoi(sections);
			}
			else if(notMatrixCounter == 3){
				start = atoi(sections);
			}
			else if(notMatrixCounter == 5){
				//accept = atoi(sections);
			}
			else if(notMatrixCounter > 5){
				if(strlen(sections) < 3){ //its a row description
					numberOfStates++;
					state = atoi(sections);
				}
				else{ //its not a row description(its on of the 2/6s thingies)i
					nodeCounter++; //this keepstrack of how many total nodes we have

					int slashIndex = 0;
					
					int afterSlash;
					int beforeSlash = 0;
					for(unsigned int i = 0; i < strlen(sections); i++){
						if(sections[i] == '/'){
							//Dealing with getting information before the slash
							slashIndex = i;
							if(slashIndex == 2){ //the number before the / is two digits long
								if(sections[1] == '0')
									beforeSlash = 10;
								else
									beforeSlash = 11;
							}
							else if(slashIndex == 1){ //the number before the / is one digit long
								beforeSlash = sections[0]-'0';
							}
							
							//Dealing with getting information after the slash
							if(isalpha(sections[i+2]) !=  0){ //the number behind the slash is a number TWO DIGITS
								afterSlash = sections[i+1]-'0';
								save = sections[i+2];
							}
							else{ //its a number (THE NUMBER BEHIND THE SLASH IS ONE DIGITs
								if(sections[i+2] == '0')
                                                                        afterSlash = 10;
                                                                else
                                                                        afterSlash = 11;
								save = sections[i+3];
							}
						}
					}
				        
					nodes[nodeCounter].state = state;
					nodes[nodeCounter].name = beforeSlash;
					nodes[nodeCounter].save = save;
					nodes[nodeCounter].change = afterSlash;
				}			
			}

			sections = strtok(NULL, seperator);
			notMatrixCounter++;
		}

	}
	//Builds a 1D array of all the nodes/structs
	for(int k = 0; k < 75*12 && k <= nodeCounter; k++){
		//printf("\nContents are(State: %d, Name: %d, Save: %c, Change: %d)\n", nodes[k].state, nodes[k].name, nodes[k].save, nodes[k].change);
        }

	//Building the Matrix 2D Array by transferring the 1d array into a 2d array
	//int oneDCounter = 0;
	node matrix[numberOfStates+1][12];

	for(int i = 0; i < numberOfStates+1; i++){
		for(int j = 0; j < 12; j++){
			matrix[i][j].state = i;
                        matrix[i][j].name = 9;
                        matrix[i][j].save = 'd';
                        matrix[i][j].change = 99;
		}
	}

	for(int i = 0; i < numberOfStates+1; i++){
		//printf("\nNew Line!\n");
		for(int j = 0; j < 12; j++){
			for(int k = 0; k < nodeCounter+1; k++){
				if(nodes[k].state == i && nodes[k].name == j){ //if the node were currently on belongs to this specific coordinate
					matrix[i][j] = nodes[k]; //adds the node to that state
					break;
				}
			}
		}
	}

	//Printing the Matix
	printf("Scanning using the following matrix:\n     ");
	//printing the top 12 numbers
	for(int i = 0; i < 12; i++){
		if(i < 9){
			printf("%d    ", i);
		}
		else if((i < 11)){
			printf("%d   ", i);
		}
		else{
			printf("%d\n", i);
		}
	}
	//printing each row
	for(int i = 0; i < numberOfStates+1; i++){
		if(i < 10){
			printf(" %d", i);
		}
		else{
			printf("%d", i);
		}
		for(int j = 0; j < 12; j++){
			if(matrix[i][j].change > 9){
				printf("  %d%c", matrix[i][j].change, matrix[i][j].save);
			}
			else if(matrix[i][j].change < 10){
				printf("   %d%c", matrix[i][j].change, matrix[i][j].save);
			}
			if(j == 11){
				printf("\n");
			}
		}
	}

	//Processing Characters from the standard input
	char result[2048]; //make sure its the same size as the input.t length

	int resultIndex = 0;
	int currentState = start;
	char current;
	int charClass;

	while((current = getchar())){
		if(!feof(stdin)){
			result[resultIndex] = current;
			charClass = classDecider(current);

			resultIndex++;
	
			if(resultIndex == 1){ //if it's the first one in a potential token
				if(matrix[currentState][charClass].save == 's'){
					printf("%d", currentState);
					printf(" %d", matrix[currentState][charClass].change);
					currentState = matrix[currentState][charClass].change;
				}
				else{
					currentState = 0;
					resultIndex = 0;
				}
			}
			else{
				printf(" %d", matrix[currentState][charClass].change);
				currentState = matrix[currentState][charClass].change;
			}

			if(currentState == 99){ //it's an error
				printf(" rejected\n");
				resultIndex = 0;
			}
			else if(currentState == 9){
				printf(" recognized '");

				//Print the saved array
				for(int j = 0; j < resultIndex-1; j++){
                                        if(resultIndex-(j+2))
                                                printf("%c", result[j]);
                                        else
                                                printf("%c'\n", result[j]);
                                }
		
				resultIndex = 0;
				currentState = 0;
			}

		}
		else{
			if(resultIndex == 0)
				printf("%d 9 EOF\n", start);
			else
				printf("9 EOF\n");
			break;
		}
	}
}

/**
 * This function returns the class(described through a number) when given a char value
 */
int classDecider(char current){
	int charClass = 11;
	if(current == ' ' || current == '\t')
		charClass = 0;
	else if(current == '\n')
		charClass = 1;
	else if(isalpha(current) > 0 || current == '_')
		charClass = 2;
	else if(current == '0')
		charClass = 3;
	else if(current == '8' || current == '9')
		charClass = 5;
	else if(current == '/')
		charClass = 6;
	else if(current == '*')
		charClass = 7;
	else if(current == '+' || current == '-' || current == '%') //1-7
		charClass = 8;
	else if(__isascii(current) && isdigit(current) == 0)
		charClass = 9;
	else if(!__isascii(current))
		charClass = 11;
	else if(current == '\n')
		charClass = 10;
	else if(current == '1' || current == '2' || current == '3' ||
			current == '4' || current == '5' ||
			current == '6' || current == '7'){ //1-7
		charClass = 4;
	}
	return charClass;
}
