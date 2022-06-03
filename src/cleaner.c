#include "pgdocs.h"
#include "deps/internals.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef enum {
    TO_LOWERCASE   	 	=       0b00001,
    REMOVE_SPECIAL 		=       0b00010,
    REMOVE_NUMBER   	=     	0b00100,
    CLEAN_WHITESPACE 	=		0b01000
} CLEANER_OPTIONS_INDEX;

const CleanerOption CLEANER_OPTIONS[] = {
    {"To Lowercase", to_lowercase},
    {"Remove Special Characters", remove_special},
    {"Remove Numbers", remove_numbers},
    {"Clean Whitespace", clean_whitespace}
};

void clean_data(Summary *summary, Config config)
{
	unsigned int options = summary->options;

    for(int i = 0; i < MAX_CLEANER_OPTIONS; i++) {
        int current = pow(2, i);

        if((current & options) != 0) {
           CLEANER_OPTIONS[i].clean(summary, config);
            // CLEANER_OPTIONS[i].report_analysis(summary, config);
        }
    }
}

/*	
 *	_convert_string_to_lowercase
 *	converts a string to lowercase
 *  precondition: all string elements are alpha
 */
void _convert_string_to_lowercase (char *str)
{
	for (int i = 0; i < (strlen(str)); i++){
   		if (str[i] < 'a')
   			str[i] += 32;
   	}
}


void to_lowercase (Summary *summary, Config config)
{
	TokenNode *currentNode = summary->tokenList->head;
    
	while(currentNode != NULL) {
		char *currentTokenStr = currentNode->tokenString;
		
		if (currentNode->tokenType == ALPHA)					//if the node is of type alpha
			_convert_string_to_lowercase (currentTokenStr);		

        currentNode = currentNode->next;		//switches current node to next  
	}
}


void remove_special (Summary *summary, Config config)
{
	TokenNode *currentNode = summary->tokenList->head;
	TokenNode *specialNode;
	bool isPreviousSpecial = false;
	
	while(currentNode != NULL) 
	{
		if (currentNode->tokenType == SPECIAL){ 
			if (isPreviousSpecial)
				remove_token(summary->tokenList, specialNode->tokenString);

			else
				isPreviousSpecial = true;

			specialNode = currentNode;
		}
		
		else if (currentNode->tokenType == WHITESPACE 
				 && isPreviousSpecial){
			remove_token(summary->tokenList, specialNode->tokenString);
			isPreviousSpecial = false;
		}

		else if ((currentNode->tokenType == ALPHA || 
				  currentNode->tokenType == NUMERIC) && 
				  isPreviousSpecial){
			strcpy(specialNode->tokenString, " ");
			isPreviousSpecial = false;
		}
	
		currentNode = currentNode->next;
	}
}


void remove_numbers (Summary *summary, Config config)
{
	TokenNode *currentNode = summary->tokenList->head;
	TokenNode *numericNode;
	int flag = 0;

	while(currentNode != NULL) {
		if (flag == 1){
			remove_token(summary->tokenList, numericNode->tokenString);
			flag = 0;
		}

		if (currentNode->tokenType == NUMERIC){
			numericNode = currentNode;
			flag = 1;
		}
			
		currentNode = currentNode->next;
	}
}


void clean_whitespace (Summary *summary, Config config)
{
	TokenNode *currentNode = summary->tokenList->head;
	TokenNode *whitespaceNode;
	bool isPreviousNodeWhitespace = false;
	
	while(currentNode != NULL) {
		if (currentNode->tokenType == WHITESPACE){ 
			if (isPreviousNodeWhitespace){
				remove_token(summary->tokenList, whitespaceNode->tokenString);
				isPreviousNodeWhitespace = false;		
			} else{
				whitespaceNode = currentNode;
				isPreviousNodeWhitespace = true;
			}
		}
		
		else
			isPreviousNodeWhitespace = false;
	
		currentNode = currentNode->next;
	}
}


















