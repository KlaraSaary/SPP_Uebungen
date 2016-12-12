/*
 * Lab 1
 * Lösung der Aufgabe 1_e
 *
 * Created on: 10.12.2016
 *      Author: Klara Saary, Felix Staniek
 */

#include "../header_praktikum_c/dictionary.h"
#include "../header_praktikum_c/parser.h"
#include "../header_praktikum_c/list.h"
#include "../header_praktikum_c/file_reader.h"

#include <stdlib.h>
#include <stdio.h>

int main(){

	Dictionary* dict_ = Dictionary_create();
		LinkedList* reference_ = LinkedList_create();
		reference_ = read_text_file("text1.txt",16000);
		LinkedList* testtext_= LinkedList_create();
		testtext_ = read_text_file("text3.txt",16000);

		char* parserpointer1_ = malloc(sizeof(char));
		char* parserpointer2_ = malloc(sizeof(char));

		Parser* parser1_ ;
		Parser* parser2_ ;


		LinkedListNode* acctualnode1_ = LinkedList_getFirst(reference_);
		while(acctualnode1_!= NULL){
			parser1_ = Parser_create(LinkedList_getData(acctualnode1_));
			while(Parser_getNextWord(parser1_,parserpointer1_,20)!=0){
				Dictionary_insert(dict_,parserpointer1_);
				parserpointer1_ = malloc(sizeof(char)*12);

			}
			acctualnode1_ = LinkedList_getNext(acctualnode1_);
		}

		int counter_=0 ;

		LinkedListNode* acctualnode2_ = LinkedList_getFirst(testtext_);
		while(acctualnode2_ != NULL){
			parser2_ = Parser_create(LinkedList_getData(acctualnode2_));
			while(Parser_getNextWord(parser2_,parserpointer2_,12)!=0){
					if(!Dictionary_isIn(dict_, parserpointer2_)){
						Dictionary_insert(dict_,parserpointer1_);
						counter_++;
					}
			}
			acctualnode2_ = LinkedList_getNext(acctualnode2_);
		}
		printf("%i words are found exclusively in the second text!\n", counter_);
		return 0;


};
