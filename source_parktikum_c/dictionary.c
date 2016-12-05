#include "../header_praktikum_c/dictionary.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>


typedef struct{
	char* prefix;
	int isword; //isWord == false -> isWord = 0;
	struct node* child[5];
}node;

struct Dictionary{
	node* root;
};

Dictionary* Dictionary_create(){
	Dictionary* dict = malloc(sizeof(Dictionary));
	(*dict).root = NULL;
	return dict;
};

void Dictionary_delete( Dictionary* dict ){
	free(dict);
};

void Dictionary_insert( Dictionary* dict, const char* word ){
	/*
	node* newNode = malloc(sizeof(struct node));
	(*newNode).prefix = word;
	(*newNode).isword = 1;

	if((*dict).root == NULL ){
		(*dict).root = newNode;
	}
	//else{
		node* current = (*dict).root;
		node* parent = NULL;
		node* child;
		int j = 1;
		int k;
		while(1){
			for(int i = 0; (*current).child[i] != NULL ;i++){
				child = (*current).child[i];
				int comp = strncmp((*child).prefix, word, j);
				if(comp < 0) {i++;}
				else if (comp == 0){
					while(strncmp((*child).prefix, word, j) == 0){
						j++;
					};
					if((j-1) == strlen((*child).prefix) && strlen(word) > j-1){ //child.prefix is a real prefix of word
						j = j-1;
						parent = current;
						current = child;
						i = 0;
					}
					else if(strcmp((*child).prefix, word) == 0){ //prefix == word
						(*child).isword = 1;
						free(newNode);
						return;
					}
					/*prefix is not a real prefix of word
					*insert a new prefix node as child of current at i. prefix sind die ersten j-1 stellen von word.
					*kinder des neuen prefix sind das alte child[i] und word

					else{
						insert_word(current, word, i);
						return;
					};
				}
				else if(comp > 0){
					insert_word_left(current, word, i-1);
					return;
				}
				k = i;
			}
			if(k <= sizeof(child)/sizeof(node) - 1){
				child = newNode;
			}
			//attach newNode to (*current).child by coping (*current).child in a larger array
			else {
				node* c[k];
				for(int z = 0; z < k; z++){
					c[z] = (*current).child[z];
				};
				c[k] = newNode;
				(*current).child = c;
			};
		}
	//} */
};

void insert_word(node* parent, const char* word, int index){

};

int Dictionary_isIn( const Dictionary* dict, const char* word ){

	return 0;
};

void Dictionary_print( const Dictionary* dict ){

};

void Dictionary_merge( Dictionary* destination, const Dictionary* source){

};





