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
    {"Remove Numbers", remove_number},
    {"Clean Whitespace", clean_whitespace}
};

/*	
 *	_convert_string_to_lowercase
 *	converts a string to lowercase
 */
void _convert_string_to_lowercase (char *str)
{
	int i;
	
	for (i = 0; i < (strlen(str)); i++){
   		if (str[i] < 'a' || str[i] > 'z')
   			str[i] += 32;
   	}
}

/*	
 *
 *
 *
 */
void to_lowercase (Summary *summary, Config config)
{
	TokenNode *currentNode = summary->tokenList->head;
    
	while(currentNode != NULL) {
		char *currentTokenStr = currentNode->tokenString;
		
		if (currentNode.tokenType == ALPHA)					//if the node is of type alpha
			convert_string_to_lowercase (currentTokenStr);		

        currentNode = currentNode->next;		//switches current node to next  
	}
}


void remove_special (Summary *summary, Config config)
{
	TokenNode *currentNode = summary->tokenList->head;

	while(currentNode != NULL) {
		if (currentNode.tokenType == SPECIAL)	
			remove_token(summary->tokenList, currentNode->tokenString);
			
		currentNode = currentNode->next;
	}
}


void remove_numbers (Summary *summary, Config config)
{
	TokenNode *currentNode = summary->tokenList->head;

	while(currentNode != NULL) {
		if (currentNode.tokenType == NUMERIC)	
			remove_token(summary->tokenList, currentNode->tokenString);
			
		currentNode = currentNode->next;
	}
}


void clean_whitespace (Summary *summary, Config config)
{
	int isPreviousNodeWhitespace = 0;
	TokenNode *currentNode = summary->tokenList->head;
	
	while(currentNode != NULL) {
		if (currentNode.tokenType == NUMERIC){
			if (isPreviousNodeWhiteSpace == 1)
				remove_token(summary->tokenList, currentNode->tokenString);
			
			isPreviousNodeWhitespace = 1;	
		}
		
		else
			isPreviousNodeWhitespace = 0;
	
		currentNode = currentNode->next;
	}
}


















