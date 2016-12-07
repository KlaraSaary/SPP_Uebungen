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




int Parser_getNextWord( Parser* parser, char* nextWord,  unsigned int bufferLength ){


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

		if((wordcounter_+1)==bufferLength){
			words[wordcounter_]= '\0';


			*nextWord = (char*)malloc(256);
			strncpy(*nextWord,words, sizeof(words));
			return -1;
		}


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
		words[wordcounter_]= '\0';
		strncpy(nextWord,words, sizeof(words));
		return 1;
		}

	}


	return 1;

}
