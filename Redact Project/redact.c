/* Author: Chris Heist
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//This method is to manage the transition between not sensing capitals and sensing capitals
int equals(char, char, int);

/**
 * The main method is where the arrays are built, rearranged, then organized.
 */
int main(int argc, char *argv[]){ //argc is the amount of commands
     //sets the size of the arrays
     char givenString[2048]; 
     char givenRedact[2048][2048];
     int starLocations[2048];
     for(int i = 0; i < 2048; i++){
          starLocations[i] = 0;
     }

     fgets(givenString, 2048, stdin); //fgets is only used to get the original string
    	

     int paranoia = 0; //contains whether or not the -p flag is active
     int caseInsensitive = 0; //contains whether or not the -c flag is active
     
     //used to store the flag strings to verify that they are in fact present
     char paranoiaStr[3]; 
     char caseStr[3];
     strcpy(paranoiaStr, "-p");
     strcpy(caseStr, "-c");

     //Cycle through the commands past ./redact
     int thingsRedacted = 0; //the amount of things that are getting redacted
     for(int i = 1; i < argc; i++){
	  //If it's a -p flag
          if(strcmp(argv[i], paranoiaStr) == 0){
	       //printf("Paranoia is active.\n");
	       paranoia = 1;
	  }
	  //if it's a -c flag
	  else if(strcmp(argv[i], caseStr) == 0){
	       //printf("Case Insensitive is active\n");
	       caseInsensitive = 1;
	  }
	  //if it's not a flag
	  else{
	       strcpy(givenRedact[thingsRedacted], argv[i]);
	       thingsRedacted+=1;
	  }
     }   
 
 
     //Paranoia Mode
     int start = -1;
     int end = 0;
     //int sizeOfOne = 0;
     if(paranoia == 1){
          for(int i = 0; givenString[i] != EOF && givenString[i] != '\0'; i++){
               if(isupper(givenString[i]) != 0 && start == -1){ //if its a capital
		    start = i;
	       }
	       else if(isalpha(givenString[i]) == 0 && start != -1){ //if its not a letter
	            //printf("Found an end at %c\n", givenString[i]);
		    end = i;
		    //printf("Start: %d, End: %d\n", start, end);	    
		    for(int j = 0; j < abs(end-start); j++){
	                 starLocations[start+j] = 1;
		    }
		    start = -1;
	       }
          }	
     } 


     //For every redactable
     for(int i = 0; i < thingsRedacted; i++){
        //printf("Current Word we're redacting: %s\n", givenRedact[i]);

        int j = 0;
        for(unsigned int k = 0; k < strlen(givenRedact[i]) && givenString[j] != EOF; k++){
             if(givenString[j] == '\0'){
                  break;
             }
             else if(isalpha(givenString[j]) == 0){ //if its not a letter
                   j++;
                   k = -1;
             }
	     
	     else if(equals(givenString[j], givenRedact[i][k], caseInsensitive) == 1){  
                       if(isalpha(givenString[j+1]) == 0 &&
                                  k+1 == strlen(givenRedact[i]) &&
                                  (isalpha(givenString[j-strlen(givenRedact[i])]) == 0)){ //if the word were testing is done and up to this point has matched sucesfully
                       for(unsigned int l = 0; l < strlen(givenRedact[i]); l++){ //unsigned is used throghout the rest of the file to avoid a sign comparing warning
                            starLocations[j-l] = 1;
                            //printf("%d ", j-l);
                       }
                       k = -1;
                  }
                  if(strlen(givenRedact[i]) == 1){
                       k = -1;
                  }
                  j++;
                  if(k == strlen(givenRedact[i])-1){ //if the word is finished, but is a substring of another word
                      k = -1;
                  }
             }
             else{ //if the letters dont match

                  //Move up a letter in the original string
                  j++;

                  //Reset the word
                  k = -1;
             }
        }
     }

    for(unsigned long int i = 0; i < sizeof(givenString); i++){
         if(givenString[i] == '\0'){
              break;
	 }
	 else if(starLocations[i] == 1){
	      givenString[i] = '*';
	 }
	 printf("%c", givenString[i]);
	 
    }

    return 0;
}

/**
 * The equals method takes in two charactyers and comapres them against eachother differently based on whether or not the -c flag is active or not
 */
int equals(char c1, char c2, int c){
     if(c == 0){ //if its case sensitive
          if(c1==c2){
	       return 1;
	  }
	  else{
	       return 0;
	  }
     }
     else{ //if its not case sensitive
          if(toupper((int)c1) == toupper((int)c2)){
	       return 1;
	  } 
	  else{
	       return 0;
	  }
     }

}
