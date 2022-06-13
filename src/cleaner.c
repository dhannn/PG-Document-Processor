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

void clean_data(Summary *summary)
{
	unsigned int options = summary->option;
	summary->mode.commands[options].execute_command(summary);
	summary->mode.commands[options].report_results(summary);
}

/*	
 *	__convert_string_to_lowercase
 *	converts a string to lowercase
 *  precondition: all string elements are alpha
 */
void __convert_string_to_lowercase (char *str)
{
	for (int i = 0; i < (strlen(str)); i++){
   		if (str[i] < 'a')
   			str[i] += 32;
   	}
}


void to_lowercase (Summary *summary)
{
	TokenNode *currentNode = summary->tokenList->head;
    int numTokens = 0;
    
	while(currentNode != NULL) {
		char *currentTokenStr = currentNode->tokenString;
		
		if (currentNode->tokenType == ALPHA)	//if the node is of type alpha
			__convert_string_to_lowercase (currentTokenStr);		

        currentNode = currentNode->next;		//switches current node to next  
		
		numTokens++;
        update_processing(numTokens, summary->tokenList->size);
	}
}

void remove_special_char (Summary *summary)
{
	TokenList *oldTokenlist = summary->tokenList;
	TokenList *newTokenlist = initialize_tokenlist();

	TokenNode *currentNode = summary->tokenList->head;
	TokenNode *previousNode = summary->tokenList->head;
    int numTokens = 0;
	
	while(currentNode != NULL) 
	{
		// if special character is in between 2 alphanumeric
		// if(currentNode->tokenType == SPECIAL && (previousNode->tokenType == ALPHA || previousNode->tokenType == NUMERIC) &&
		// (currentNode->next->tokenType == ALPHA || currentNode->next->tokenType == NUMERIC))	

		if(currentNode->tokenType == SPECIAL) {
			// @Gwen 	the program is segfaulting since you didn't check if the 
			//			next node is NULL; thereby accessing memory illegally 
			if(currentNode->next != NULL) {
				if((previousNode->tokenType == ALPHA || 
					previousNode->tokenType == NUMERIC) && (currentNode->next->tokenType == ALPHA || currentNode->next->tokenType == NUMERIC)) {
						
					strcpy(currentNode->tokenString, " ");
					char *temp = create_string(currentNode->tokenString);
					add_token(newTokenlist, temp);	
				}
			}
		}

		else if(currentNode->tokenType != SPECIAL)
		{
			char *temp = create_string(currentNode->tokenString);
			add_token(newTokenlist, temp);	
		}

		numTokens++;
        update_processing(numTokens, summary->tokenList->size);

		previousNode = currentNode;
		currentNode = currentNode->next;
	}

	summary->tokenList = newTokenlist;

	delete_token_strings(oldTokenlist);
	destroy_tokenList(oldTokenlist);
}


void remove_numbers (Summary *summary)
{
	TokenList *oldTokenlist = summary->tokenList;
	TokenList *newTokenlist = initialize_tokenlist();

	TokenNode *currentNode = summary->tokenList->head;
    int numTokens = 0;

	while(currentNode != NULL) {
		if (currentNode->tokenType != NUMERIC) {
			int length = strlen(currentNode->tokenString) + 1;
			char *temp = calloc(length, 1);

			strcpy(temp, currentNode->tokenString);
			add_token(newTokenlist, temp);
		} else if(currentNode->next != NULL) {
			if(currentNode->next->tokenType == WHITESPACE)
				currentNode = currentNode->next;
		}
			
		numTokens++;
        update_processing(numTokens, summary->tokenList->size);
		currentNode = currentNode->next;
	}

	summary->tokenList = newTokenlist;

	delete_token_strings(oldTokenlist);
	destroy_tokenList(oldTokenlist);
}

// removes all newlines and replaces them with a single space
void clean_whitespace (Summary *summary)
{
	TokenList *oldTokenlist = summary->tokenList;
	TokenList *newTokenlist = initialize_tokenlist();

	TokenNode *currentNode = summary->tokenList->head;
	TokenNode *previousNode = summary->tokenList->head;

	int numTokens = 0;

	while(currentNode != NULL) {
		if(strcmp(currentNode->tokenString, "\n") == 0)
			strcpy(currentNode->tokenString, " ");

		if(((strcmp(currentNode->tokenString, " ") == 0 && strcmp(previousNode->tokenString, " ") != 0)) || 
			 currentNode->tokenType != WHITESPACE){
			char *temp = create_string(currentNode->tokenString);
			add_token(newTokenlist, temp);
		}

		numTokens++;
        update_processing(numTokens, summary->tokenList->size);

		previousNode = currentNode;
		currentNode = currentNode->next;
	}

	summary->tokenList = newTokenlist;

	delete_token_strings(oldTokenlist);
	destroy_tokenList(oldTokenlist);
}

/*	
 *	_merge_alpha_node
 *	merges all neighboring alpha nodes
 */	
TokenList *__merge_alpha_nodes (Summary *summary)
{
	TokenList *oldTokens = summary->tokenList;
	TokenList *newTokens = initialize_tokenlist();
	TokenNode *currentNode = oldTokens->head;
	TokenNode *previousNode = oldTokens->head;
	char word[MAX_CHAR] = "";
	
	while(currentNode != NULL) 
	{
		if(currentNode->tokenType == ALPHA){
			if(previousNode->tokenType != ALPHA)
				strcpy(word, currentNode->tokenString);

			if(previousNode->tokenType == ALPHA) 
				strcat(word, currentNode->tokenString);

			if((currentNode->next != NULL && currentNode->next->tokenType != ALPHA) ||
				currentNode->next == NULL){
				char *temp = create_string(word);
				add_token(newTokens, temp);
				strcpy(word, "");
			}
		} 

		else {
			char *temp = create_string (currentNode->tokenString);
			add_token(newTokens, temp);
		}

		previousNode = currentNode;
		currentNode = currentNode->next;
	}

	delete_token_strings(oldTokens);
	destroy_tokenList(oldTokens);

	return newTokens;
}


void remove_stopwords (Summary *summary)
{	
	TokenList *oldTokenlist = __merge_alpha_nodes(summary);
	TokenList *newTokenlist = initialize_tokenlist();
	TokenNode *currentNode = oldTokenlist->head;

	FILE *file = fopen("dat/stopwords", "r");
	char currentString[MAX_CHAR] = "", matchedStopword[MAX_CHAR] = "";
	int flag;
    int numTokens = 0;

	while(currentNode != NULL) 
	{
		if (currentNode->tokenType == ALPHA)
		{ 
			flag = 0;
			strcpy(currentString, currentNode->tokenString);
			__convert_string_to_lowercase(currentString);

			while (strcmp(currentString, matchedStopword) != 0 && flag != EOF)
				flag = fscanf(file, "%s", matchedStopword);

			fseek(file, 0, SEEK_SET);

			if (strcmp(currentString, matchedStopword) != 0){
				char *temp = create_string (currentNode->tokenString);
				add_token(newTokenlist, temp);

			} else if(currentNode->next->tokenType == WHITESPACE) {
				currentNode = currentNode->next; // to remove extra space
			}
		} else {
			char *temp = create_string(currentNode->tokenString);
			add_token(newTokenlist, temp);
		}

		numTokens++;
        update_processing(numTokens, summary->tokenList->size);

		currentNode = currentNode->next;
	}

	fclose(file);
	
	summary->tokenList = newTokenlist;

	delete_token_strings(oldTokenlist);
	destroy_tokenList(oldTokenlist);
}

void clean_all(Summary *summary)
{
	void (*commands[])(Summary*) = {
		to_lowercase,
		remove_special_char,
		remove_numbers,
		remove_stopwords,
		clean_whitespace
	};

	int length = sizeof(commands) / sizeof(commands[0]);

	for(int i = 0; i < length; i++) {
		commands[i](summary);
	}
}

void report_cleaned(Summary *summary)
{
    TokenList *list = summary->tokenList;
    TokenNode *tokenNode = next_token(list);

    int runningTotal = 0;
    int currentSize = MAX_CHAR;
    char *temp = calloc(currentSize, 1);
    char buff[MAX_CHAR] = "";

	int numChar = 0;

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

		numChar++;
		update_reporting(numChar, summary->tokenList->size);

        tokenNode = tokenNode->next;
    }

	if(summary->outData != NULL)
		free(summary->outData);

    summary->outData = temp;
}
