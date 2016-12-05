//#include <parser.h>
#include "../header_praktikum_c/parser.h"
#include "../header_praktikum_c/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>


struct Parser{
	//TODO whats important
	//pointer and content of current parser
	const char* text_;
	int current_word_;

};

Parser* Parser_create( const char* text ){

	Parser* parser_ = malloc(sizeof(Parser));

	(*parser_).text_= text;
	(*parser_).current_word_ = 0;



		//TODO parsing,parsing and parsing

	return parser_;
}

void Parser_delete( Parser* parser ){
	(*parser).text_= NULL;
	(*parser).current_word_= 0;

}




int Parser_getNextWord( Parser* parser, char** nextWord,  unsigned int bufferLength ){

/*
	 * if text ist zu ende return 0 nextWord = \0
	 *
	 * if size of next word + \0 > buffelength return 2 halfes
	 *
	 * while(nextchar is alpha){
	 * 	toupper char
	 * 	append(nextword, char)
	 * 	}
	 *
	 *	wenn sonderzeichen aufhören und 1 zurückgeben
	 *
	 *	position speichern in current_position_
	 *
	 *	sonderzeichen vor neuem wort wegzählen
	 *
	 *
	 *
	 *
*/

	char i[16000];
	//char* passing_;
	//printf("this ist he passed posiotion: %i\n", parser->current_word_);

	strcpy(i, parser->text_);
	char words[bufferLength];
	int wordcounter_ = 0;

	if(i[parser->current_word_]=='\0'){
		return 0;
	}



	while(isalpha(i[parser->current_word_])==0){
		parser->current_word_++;

	}


	while(i[parser->current_word_]!='\0'){
		if(isalpha(i[parser->current_word_])==0){
			parser->current_word_++;
			//printf("space or questionmark found \n");
		}
		else{while(isalpha(i[parser->current_word_])!=0){
				//int temp_ = toupper(i[parser->current_word_]);
			words[wordcounter_]= (char) toupper(i[parser->current_word_]);
				//printf("counter %i\n", parser->current_word_);
			wordcounter_++;
				//nextWord[wordcounter_]=(char) toupper(i[parser->current_word_]);
			parser->current_word_++;
				//printf("isalpha %i \n", isalpha(i[parser->current_word_]));
		}

		*nextWord = (char*)malloc(256);
		strncpy(*nextWord,words, sizeof(words));
		//printf("%s \n", *nextWord);
		//printf("counter %i\n", parser->current_word_);
		//printf("%s \n", words);
		//free(nextWord);
		return 1;
		}

	}
	//words[wordcounter_+1]='\0';
	//nextWord[wordcounter_+1]='\0';
	//strncpy(nextWord,words,bufferLength);

	//printf("%s \n", nextWord);
	//nextWord = malloc(256);

	//nextWord = "fick die UNi";






	return 1;

}
/*

void strreverse(char* string)
{
 int i, len;
 char c;
 len=strlen(string);
 char string2[len];

 for(i=0; i<len; i++)
 {
  c=string[i];
  string2[len-i-1]=string[i];

 }
 string2[len]='\0';
 *string=string2;
 printf("%s\n", string);//<-- this would work,but should be in main

}
*/
