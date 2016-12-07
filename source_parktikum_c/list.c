#include "../header_praktikum_c/list.h"
#include "../header_praktikum_c/parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>


int changechar(char** value){
	value = malloc(256);
	strcpy(*value,"wer");
	return 0;
}


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












