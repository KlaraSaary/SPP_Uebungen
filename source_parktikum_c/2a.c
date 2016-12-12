#include "../header_praktikum_c/dictionary.h"
#include "../header_praktikum_c/parser.h"
#include "../header_praktikum_c/list.h"
#include "../header_praktikum_c/file_reader.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <omp.h>
#include <time.h>


int main(int arg_count,char* arg_value[]){

	Dictionary* dict_ = Dictionary_create();
	LinkedList* reference_ = LinkedList_create();
	reference_ = read_text_file(arg_value[1],16000);
	LinkedList* testtext_= LinkedList_create();
	testtext_ = read_text_file(arg_value[2],16000);

	char* parserpointer1_ = malloc(sizeof(char));


	Parser* parser1_ ;
	Parser* parser2_ ;


	LinkedListNode* acctualnode1_ = LinkedList_getFirst(reference_);
	while(acctualnode1_!= NULL){
		parser1_ = Parser_create(LinkedList_getData(acctualnode1_));
		while(Parser_getNextWord(parser1_,parserpointer1_,20)!=0){
			Dictionary_insert(dict_,parserpointer1_);
			parserpointer1_ = malloc(sizeof(char)*20);

		}
		acctualnode1_ = LinkedList_getNext(acctualnode1_);
	}



	LinkedListNode* acctualnode2_ = LinkedList_getFirst(testtext_);

	int counter_=0 ;
 	int i =0;
 	int counter2=0;
 	double time = omp_get_wtime( );
	#pragma omp parallel for private(i) reduction(+: counter_) 


	for(i=0;i<LinkedList_getSize(testtext_);i++){

		//int tid = omp_get_thread_num();
		//printf("Hello World from thread = %d\n", tid);
		//printf("%d\n", omp_get_thread_num());
		parser2_ = Parser_create(LinkedList_getDataAt(testtext_,i));
		char* parserpointer2_ = malloc(sizeof(char));
				//#pragma omp parallel reduction(+:counter_)

				while(Parser_getNextWord(parser2_,parserpointer2_,20)!=0){

						if(!Dictionary_isIn(dict_, parserpointer2_)){
							//printf("%s\n",parserpointer2_);
							Dictionary_insert(dict_,parserpointer2_);
							counter_++;
							//

						}

				}

	};


	printf("Time: \t %f \n", omp_get_wtime()-time);
	//Dictionary_print(dict_);
	printf("%i words are found exclusively in the second text! sould be 3808 \n", counter_);
	return 0;


};



