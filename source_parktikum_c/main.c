#include "../header_praktikum_c/dictionary.h"
#include "../header_praktikum_c/parser.h"
#include "../header_praktikum_c/list.h"
#include "../header_praktikum_c/file_reader.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
//main file_reader.c
/*
int main(){
	read_text_file("/home/klara/TUD/16_17WS/SPP/Uebung/Lab1/text1.txt", 16000);
	return 0;
}
*/

//main list.c
/*int main() {

	LinkedList* testlist1 = LinkedList_create();
	LinkedList_append(testlist1, "a");
	LinkedList_append(testlist1, "b");
	LinkedList_append(testlist1, "c");
	LinkedList_append(testlist1, "d");
	printf("LinkedList size: %i\n", (LinkedList_getSize(testlist1)));
	printf("LinkedList first: %s\n",
			(LinkedList_getData(LinkedList_getFirst(testlist1))));
	printf("LinkedList first: %s\n", (LinkedList_getDataAt(testlist1, 0)));
	printf("LinkedList second: %s\n", (LinkedList_getDataAt(testlist1, 1)));
	printf("LinkedList third: %s\n", (LinkedList_getDataAt(testlist1, 2)));
	printf("LinkedList last: %s\n", (LinkedList_getDataAt(testlist1, 3)));
	return 0;
*/



	//printf("%i", Parser_getNextWord(a,cash[0],8));
	//printf("hopefully a char: %s", cash[0]);

/*


	char str[256];
	printf("Type a String to reverse it.[max. 255 chars]\n");
	fgets(str, 255, stdin);
	strreverse(str);
	const char* text = "Alle.meine.  Entchen\0";
	Parser* a = Parser_create(text);
	//Parser_getNextWord(a,str,256);
	printf("%s", str);




	return 0;

}*/


//main dictionary.c
int main(){

	/*

		const char* text = "Alle.meine. juhu Entchen\0";
		Parser* pars = Parser_create(text);
		char* test = malloc(sizeof(char));
		//test[0] = malloc(256);
		while(Parser_getNextWord(pars,test,6) !=0){
			printf("%s\n", test);
		}

	*/


/*
 * 	char* a = "A";
	char* b = "B";
	char* y = "Y";
	char* o = "O";
	char* aber ="ABER";
	char* ab = "ABER";
	char* the = "THE";
	char* project = "PROJECT";
	char* gutenberg = "GUTENBERG";
	char* absolut = "ABSOLUT";
//	char* clown = 'CLOWN';
//	char* abend = 'ABEND';

	Dictionary* dict = Dictionary_create();
	Dictionary_insert(dict, the);
	Dictionary_insert(dict, project);
	Dictionary_insert(dict, gutenberg);
	Dictionary_insert(dict, o);
	Dictionary_insert(dict, aber);
	Dictionary_insert(dict, absolut);
	Dictionary_print(dict);
	int i = Dictionary_isIn(dict, ab);
	printf("main i : %i \n", i);*/


	Dictionary* dict_ = Dictionary_create();
	LinkedList* reference_ = LinkedList_create();
	reference_ = read_text_file("/home/felix/git/spp/SPP_Uebungen/source_parktikum_c/text2.txt",16000);
//	LinkedList* testtext_= LinkedList_create();
	//testtext_ = read_text_file("/home/klara/TUD/16_17WS/SPP/Uebung/Lab1/text2.txt",16000);

	char* parserpointer1_ = malloc(sizeof(char));
	char* parserpointer2_ = malloc(sizeof(char));

	Parser* parser1_ ;
	Parser* parser2_ ;


	LinkedListNode* acctualnode1_ = LinkedList_getFirst(reference_);
	//printf("%s",LinkedList_getData(acctualnode1_));
	while(acctualnode1_!= NULL){
		parser1_ = Parser_create(LinkedList_getData(acctualnode1_));
		while(Parser_getNextWord(parser1_,parserpointer1_,20)!=0){
			printf("%s\n",parserpointer1_);
			Dictionary_insert(dict_,parserpointer1_);
			//parserpointer1_ = malloc(sizeof(char)*12);

		}
		//printf("here1 \n");
		acctualnode1_ = LinkedList_getNext(acctualnode1_);
	}
	//Dictionary_print(dict_);
	int counter_=0 ;

	/*LinkedListNode* acctualnode2_ = LinkedList_getFirst(testtext_);
	while(LinkedList_getNext(acctualnode2_) != NULL){
		parser2_ = Parser_create(LinkedList_getData(acctualnode2_));
		while(Parser_getNextWord(parser2_,parserpointer2_,12)!=0){
				if(Dictionary_isIn(dict_, parserpointer2_)){
					Dictionary_insert(dict_,parserpointer1_);
					counter_++;
				}


		}
	} */

	printf("%i words are found exclusively in the second text!", counter_);
	return 0;

};




