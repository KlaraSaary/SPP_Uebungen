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


	int i=0;
	const char* text = "Alle.meine.  Entchen\0";
	Parser* a = Parser_create(text);
	char* test[]={"eins","zwei","drei","vier"};
	char str[18];
	//test[0] = malloc(256);

	Parser_getNextWord(a,&test[0],256);
	printf("%s\n", test[0]);
	Parser_getNextWord(a,&test[3],256);
	printf("%s\n", test[3]);


	//printf("%i", Parser_getNextWord(a,cash[0],8));
	//printf("hopefully a char: %s", cash[0]);




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
	char* a = "A";
	char* b = "B";
	char* y = "Y";
	char* o = "O";
	char* aber ="ABER";
	char* absolut = "ABSOLUT";
//	char* clown = 'CLOWN';
//	char* abend = 'ABEND';

	Dictionary* dict = Dictionary_create();
	Dictionary_insert(dict, a);
	Dictionary_insert(dict, b);
	Dictionary_insert(dict, y);
	Dictionary_insert(dict, o);
	Dictionary_insert(dict, aber);
	Dictionary_insert(dict, absolut);
	Dictionary_print(dict);
	int i = Dictionary_isIn(dict, aber);
	printf("main i : %i \n", i);
};




