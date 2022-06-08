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

void clean_data(Summary *summary, Config config)
{
	unsigned int options = summary->options - 1;

	summary->mode.commands[options].execute_command(summary, config);
	summary->mode.commands[options].report_results(summary, config);
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
		
		if (currentNode->tokenType == ALPHA)	//if the node is of type alpha
			_convert_string_to_lowercase (currentTokenStr);		

        currentNode = currentNode->next;		//switches current node to next  
	}
}


//void _


void remove_special (Summary *summary, Config config)
{
	TokenList *oldTokenlist = summary->tokenList;
	TokenList *newTokenlist = initialize_tokenlist();

	TokenNode *currentNode = summary->tokenList->head;
	TokenNode *previousNode = NULL;
	
	while(currentNode != NULL) 
	{
		if (currentNode->tokenType != SPECIAL){ 
			// TODO: 	@Gwen extract method
			//			takes a string as a parameter,
			//			gets the length, allocates a string
			//			with that length + 1 and copies the 
			//			input string; return yung allocated memory
			/******/
			// int length = strlen(tokenString) + 1;
			// char *temp = calloc(length, 1);

			// strcpy(temp, tokenString);
			// return temp;
			/******/
			int length = strlen(currentNode->tokenString) + 1;
			char *temp = calloc(length, 1);

			strcpy(temp, currentNode->tokenString);
			add_token(newTokenlist, temp);
		} else if(previousNode->tokenType == WHITESPACE)
			currentNode = currentNode->next;

		previousNode = currentNode;
		currentNode = currentNode->next;
	}

	delete_token_strings(oldTokenlist);
	destroy_tokenList(oldTokenlist);

	summary->tokenList = newTokenlist;
}


void remove_numbers (Summary *summary, Config config)
{
	TokenList *oldTokenlist = summary->tokenList;
	TokenList *newTokenlist = initialize_tokenlist();

	TokenNode *currentNode = summary->tokenList->head;
	TokenNode *numericNode;
	int isNumeric = (currentNode->tokenType == NUMERIC);

	while(currentNode != NULL) {
		if (currentNode->tokenType != NUMERIC){
			int length = strlen(currentNode->tokenString) + 1;
			char *temp = calloc(length, 1);

			strcpy(temp, currentNode->tokenString);
			add_token(newTokenlist, temp);
			isNumeric = 0;
		} else if(currentNode->next->tokenType == WHITESPACE)
			currentNode = currentNode->next;
			
		currentNode = currentNode->next;
	}

	summary->tokenList = newTokenlist;

	delete_token_strings(oldTokenlist);
	destroy_tokenList(oldTokenlist);
}


void clean_whitespace (Summary *summary, Config config)
{
	return;
	// TokenList *oldTokenlist = summary->tokenList;
	// TokenList *newTokenlist = initialize_tokenlist();

	// TokenNode *currentNode = summary->tokenList->head;
	// TokenNode *whitespaceNode;

	// bool isPreviousNodeWhitespace = false;
	// int isWhitespace = (currentNode->tokenType == WHITESPACE);
	
	// // removes all leading whitespaces
	// while(currentNode->tokenType == WHITESPACE){
	// 	// int length = strlen(whitespaceNode->tokenString);
	// 	// char *temp = malloc(length);

	// 	// strcpy(temp, currentNode->tokenString);
	// 	// add_token(newTokenlist, whitespaceNode->tokenString);

	// 	currentNode = currentNode->next;
	// }

	// while(currentNode != NULL) {
	// 	if (currentNode->tokenType != WHITESPACE){ 
	// 		int length = strlen(currentNode->tokenString) + 1;
	// 		char *temp = calloc(length, 1);

	// 		strcpy(temp, currentNode->tokenString);
	// 		add_token(newTokenlist, currentNode->tokenString);
			
	// 		isPreviousNodeWhitespace = false;
	// 	}
	// 	else {
	// 		if (!isPreviousNodeWhitespace){
	// 			int length = strlen(currentNode->tokenString) + 1;
	// 			char *temp = calloc(length, 1);
				
	// 			strcpy(temp, currentNode->tokenString);
	// 			add_token(newTokenlist, currentNode->tokenString);
	// 		}

	// 		isPreviousNodeWhitespace = true;
	// 	}
	
	// 	currentNode = currentNode->next;
	// }
	
	// delete_token_strings(oldTokenlist);
	// destroy_tokenList(oldTokenlist);
	// summary->tokenList = newTokenlist;
}

/*	
 *	_merge_alpha_node
 *	merges all neighboring alpha nodes
 */	
void _merge_alpha_nodes (Summary *summary)
{
	TokenList *newTokens = initialize_tokenlist();
	TokenNode *currentNode = summary->tokenList->head;
	TokenNode *alphaNode, *deleteNode;
	bool isPreviousAlpha = false; 
	int flag = 0;
	
	while(currentNode != NULL) 
	{
		if (currentNode->tokenType == ALPHA){ 
			if (isPreviousAlpha){
				int length = strlen(alphaNode->tokenString) + strlen(currentNode->tokenString) + 1;

				char *temp = calloc(length, 1);
				strcpy(temp, alphaNode->tokenString);
				strcat(temp, currentNode->tokenString);

				add_token(newTokens, temp);
				// strcat(alphaNode->tokenString, currentNode->tokenString);
				deleteNode = currentNode;
				flag = 1;
			} else{
				isPreviousAlpha = true;
				alphaNode = currentNode;
			}
		}

		else 
			isPreviousAlpha = false;
		
		currentNode = currentNode->next;
	}
}


void remove_stopword (Summary *summary, Config config)
{
	_merge_alpha_nodes(summary);
	
	TokenList *oldTokenlist = summary->tokenList;
	TokenList *newTokenlist = initialize_tokenlist();

	TokenNode *currentNode = oldTokenlist->head;

	FILE *file = fopen("dat/stopwords", "r");
	char currentString[MAX_CHAR], matchedStopword[MAX_CHAR];
	bool isPreviousStopword = false; 
	int flag;

	while(currentNode != NULL) 
	{
		if (currentNode->tokenType == ALPHA)
		{ 
			flag = 0;
			strcpy(currentString, currentNode->tokenString);
			_convert_string_to_lowercase(currentString);

			while (strcmp(currentString, matchedStopword) != 0 && flag != EOF)
				flag = fscanf(file, "%s", matchedStopword);

			fseek(file, 0, SEEK_SET);

			if (strcmp(currentString, matchedStopword) != 0){
				int length = strlen(currentNode->tokenString) + 1;
				char *temp = calloc(length, 1);

				strcpy(temp, currentNode->tokenString);
				add_token(newTokenlist, temp);

				isPreviousStopword = false;
			} else if(currentNode->next->tokenType == WHITESPACE) {
				isPreviousStopword = true;
				currentNode = currentNode->next; // to remove extra space
			}
		} else {
			int length = strlen(currentNode->tokenString) + 1;
			char *temp = calloc(length, 1);

			strcpy(temp, currentNode->tokenString);
			add_token(newTokenlist, temp);
		}	

		currentNode = currentNode->next;
	}

	fclose(file);

	delete_token_strings(oldTokenlist);
	destroy_tokenList(oldTokenlist);
	summary->tokenList = newTokenlist;
}

void clean_all(Summary *summary, Config config)
{
	void (*commands[])(Summary*, Config) = {
		to_lowercase,
		remove_special,
		remove_numbers,
		clean_whitespace,
		remove_stopword
	};

	int length = sizeof(commands) / sizeof(commands[0]);

	for(int i = 0; i < length; i++) {
		commands[i](summary, config);
	}
}

void report_cleaned(Summary *summary, Config config)
{
    TokenList *list = summary->tokenList;
    TokenNode *tokenNode = next_token(list);

    int runningTotal = 0;
    int currentSize = MAX_CHAR;
    char *temp = calloc(currentSize, 1);
    char buff[MAX_CHAR] = "";

    while(tokenNode != NULL) {
        sprintf(buff, "%s", tokenNode->tokenString);
        buff[strlen(buff)] = '\0';

        runningTotal += strlen(buff);

        if(runningTotal + 1 > currentSize) {
            currentSize *= 5;
            temp = realloc(temp, currentSize);
        }

        strcat(temp, buff);
        temp[strlen(temp)] = '\0';
        tokenNode = tokenNode->next;
    }

    summary->outData = temp;
}
