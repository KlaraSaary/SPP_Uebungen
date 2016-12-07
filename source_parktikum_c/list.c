#include "../header_praktikum_c/list.h"
#include "../header_praktikum_c/parser.h"
#include "../header_praktikum_c/dictionary.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>


int changechar(char** value){
	value = malloc(256);
	strcpy(*value,"wer");
	return 0;
}


<<<<<<< HEAD
=======

int main() {
/*
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

	int i=0;
	const char* text = "Alle.meine. ich hab ein groses problem mit diesem Parser  Entchen\0";
	Parser* a = Parser_create(text);
	char* test = malloc(sizeof(char));
	//test[0] = malloc(256);

	while(Parser_getNextWord(a,test,5) != 0){
		printf("%s \n", test);
		//Parser_getNextWord(a,test,10);
	}




	//printf("%i", Parser_getNextWord(a,cash[0],8));
	//printf("hopefully a char: %s", cash[0]);
/*
	Dictionary* testobject = Dictionary_create();
	Dictionary_insert(testobject, "whale" );
	Dictionary_insert(testobject, "word" );
	Dictionary_insert(testobject, "wood" );
	Dictionary_print(testobject);
*/
	return 0;

}




>>>>>>> master
struct LinkedListNode {

	//previous element
	LinkedListNode* predecessor;
	//successiv element
	LinkedListNode* successor;
	//sting with content
	char* content;
};

struct LinkedList {
	//first element
	LinkedListNode* first;
	//last element
	LinkedListNode* last;
};

LinkedList* LinkedList_create() {

	//create new List
	LinkedList* newlist_ = malloc(sizeof(LinkedList));

	//fill fields with NULL
	(*newlist_).first = NULL;
	(*newlist_).last = NULL;

	return newlist_;
}

void LinkedList_append(LinkedList* list, char* data) {

	//create new Node
	LinkedListNode* a = malloc(sizeof(LinkedListNode)); //Sollte es nicht lauten: a = (LinkedListNode*)malloc(sizeof(LinkedListNode)), da der return typ von malloc void* ist 
	//printf("node created: %s", " success\n");

	if (list->first == NULL) {
		(*a).predecessor = NULL;
		list->first = a;
		(*a).successor = NULL;
		(*a).content = data;
		list->last = list->first;
	} else {
		//fill fields
		(*a).predecessor = list->last;
		(*a).predecessor->successor = a;
		list->last = a;
		(*a).successor = NULL;
		(*a).content = data;
		//printf("content: %s", (*a).predecessor->content );
	}
}

void LinkedList_delete(LinkedList* list) {

	while (list->last->predecessor != NULL) {
		LinkedListNode* dummy_ = list->last;
		dummy_->successor = NULL;
		list->last = dummy_->predecessor;
		free(dummy_->content);
		free(dummy_);

	}
}

LinkedListNode* LinkedList_getFirst(LinkedList* list) {
	//if(list->first = NULL) return NULL;
	return list->first;
}

LinkedListNode* LinkedList_getLast(LinkedList* list) {
	//if(list->last = NULL) return NULL;
	return list->last;
}

LinkedListNode* LinkedList_getPrevious(LinkedListNode* node) {
	//if(node->predecessor = NULL) return NULL;
	return node->predecessor;
}

char* LinkedList_getData(LinkedListNode* node) {
	//if(node->content = NULL) return NULL;
	return node->content;
}

unsigned int LinkedList_getSize(LinkedList* list) {

	int size = 0;
	if (list->first == NULL)
		return size;
	LinkedListNode* a = list->first;
	size++;
	while ((a = a->successor) != NULL)
		size++;

	return size;
}

char* LinkedList_getDataAt(LinkedList* list, unsigned int index) {

	int position = 0;
	LinkedListNode* a = list->first;
	//int gleich = position == index;
	while (position != index) {
		a = a->successor;
		position++;
	}

	return LinkedList_getData(a);
}












