/**
 * Author: Chris Heist
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "functions.h"
#include "tree.h"

int linedx = 0;

/**
 * The main method takes in from the input file and sorts out if each character in each line
 * is valid enough to be apart of a word. it then seperates out those wordsa dn individually 
 * sends them to the inser_word method.
 */
int main(int argc){
	if(argc > 1){
		printf("usage: concordance\n");
	}

	char *buf = NULL; //important: must initially be NULL
	size_t len = 0; //should initially be 0
	TreeNode *node = malloc(sizeof(TreeNode));
	node = NULL;

	while(getline(&buf, &len, stdin) > 0 && buf != NULL){
		//do something with each input line
		
		linedx++;
		int start = 0;
		int end = start;

		while(buf[end] != '\0'){ //while its not the end of the line
			if(isValidChar(buf[end]) == 1){ //if the char is valid
				end++;		
			}
			else{
				if(end-start > 0){
					char str[end-start+1];
					for(int i = 0; i < end-start; i++){
						str[i] = buf[i+start];
					}
					str[end-start] = '\0';
					insert_word(&node, str);	 			
				}
				end++;
				start = end;
			}
		}
	}
	traverse_tree(node);

	if(buf != NULL){
		free(buf); //must deallocate this space!
	}

	return (0);
}

/**
 * This insert_word method places a word in the root by going through all of the tree 
 * until the word fits into a new space, or it has already been made and meets the word at that location
 */
void insert_word(TreeNode **root, const char *word){
	if((*root) == NULL){ //it's a new entry
		*root = malloc(sizeof(TreeNode));
		(*root)->word = strdup(word);
		(*root)->frequency = 1;
		(*root)->refLength = 100000;
		(*root)->references = malloc((*root)->refLength*sizeof(int));
		(*root)->references[0] = linedx;
		(*root)->left == NULL;
		(*root)->right == NULL;
	} 
	else{ //it's not the first entry
		if(strcasecmp(word, ((*root)->word)) == 0){ //the words are the same
			(*root)->frequency++;
			(*root)->references[(*root)->frequency] = linedx;
			//printf("\nAlready added, but frequency was imcremented.\n");
		}
		else if(strcasecmp(word, ((*root)->word)) < 0){
			//printf("\nGoing deeper in..<..\n");
			insert_word(&(*root)->left, word);
		}
		else if(strcasecmp(word, ((*root)->word)) > 0){
			//printf("\nGoing deeper in ..>..\n");
			insert_word(&(*root)->right, word);
		}
	}	
}

/**
 * This method goes through the entire tree using the left, middle, right fashion
 */
void traverse_tree(const TreeNode *root){
	//traverse tree
	
	if((*root).left != NULL){
		traverse_tree((*root).left);
	}
	printf("%s (%d):  ", (*root).word, (*root).frequency);
	/*
	for(int i = 0; i < (*root).refLength; i++){
		if((*root).refLength-(i+1) == 0){
			printf("%d\n", (*root).references[i]);
		}	
		else
			printf("%d, ", (*root).references[i]);
	}
	*/
	int correctAmount = 0;
	for(int i = 0; i < (*root).refLength + (10-(*root).refLength); i++){
		if((*root).references[i] != 0){
			correctAmount++;
			if((*root).frequency != correctAmount){
                        	printf("%d, ", (*root).references[i]);
                	}
                	else
                        	printf("%d\n", (*root).references[i]);
			}
	}

	if((*root).right != NULL){
		traverse_tree((*root).right);
	}
}

/**
 * This method frees all of the allocated space made up all over the program
 */
void cleanup_tree(TreeNode *root){
	//cleanup the tree
	if((*root).left != NULL){
                traverse_tree((*root).left);
        }
        
	free(root->word);
        free(root->frequency);
        free(root->refLength);
        free(root->references);
        free(root->left);
        free(root->right);

        if((*root).right != NULL){
                traverse_tree((*root).right);
        }
}

/**
 * This method simply tests if the incoming char is a valid character to make up a word
 */
int isValidChar(char argA){
	if((isalpha(argA)) || (isdigit(argA)) || argA == '\'' || argA == '-') {
		return 1;
	}
	else{
		//printf("%c", argA);
		return 0;
	}
}
