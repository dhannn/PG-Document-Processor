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
    // for(int i = 0; i < MAX_CLEANER_OPTIONS; i++) {
    //     int current = pow(2, i);

    //     if((current & options) != 0) {
	// 		summary->mode.commands[i].execute_command(summary, config);
	// 		summary->mode.commands[i].report_results(summary, config);
	// 	}
    // }
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
	int flag = 0;
	
	// removes all leading whitespaces
	while(currentNode->tokenType == WHITESPACE){
		if (flag == 1)
			remove_token(summary->tokenList, whitespaceNode->tokenString);

		whitespaceNode = currentNode;
		currentNode = currentNode->next;
		flag = 1;
	}

	if (flag == 1)
		remove_token(summary->tokenList, whitespaceNode->tokenString);

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

/*	
 *	_merge_alpha_node
 *	merges all neighboring alpha nodes
 */
void _merge_alpha_nodes (Summary *summary)
{
	TokenNode *currentNode = summary->tokenList->head;
	TokenNode *alphaNode, *deleteNode;
	bool isPreviousAlpha = false; 
	int flag = 0;
	
	while(currentNode != NULL) 
	{
		if (currentNode->tokenType == ALPHA){ 
			if (isPreviousAlpha){
				strcat(alphaNode->tokenString, currentNode->tokenString);
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

		if (flag == 1){
			remove_token(summary->tokenList, deleteNode->tokenString);
			flag = 0;
		}
	}
}


void remove_stopword (Summary *summary, Config config)
{
	_merge_alpha_nodes(summary);
	
	FILE *file = fopen("dat/stopwords", "r");
	TokenNode *currentNode = summary->tokenList->head;
	TokenNode *stopwordNode;
	char currentString[MAX_CHAR], currentStopword[MAX_CHAR];
	bool isPreviousStopword = false; 
	int flag;

	while(currentNode != NULL) 
	{
		if (currentNode->tokenType == ALPHA)
		{ 
			flag = 0;
			strcpy(currentString, currentNode->tokenString);
			_convert_string_to_lowercase(currentString);

			while (strcmp(currentString, currentStopword) != 0 && flag != EOF)
				flag = fscanf(file, "%s", currentStopword);

			fseek(file, 0, SEEK_SET);

			if (isPreviousStopword){
				remove_token(summary->tokenList, stopwordNode->tokenString);
				isPreviousStopword = false;
			} 

			if (strcmp(currentString, currentStopword) == 0){
				isPreviousStopword = true;	
				stopwordNode = currentNode;
			}
		}		

		currentNode = currentNode->next;
	}		

	clean_whitespace(summary, config);

	fclose(file);
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
