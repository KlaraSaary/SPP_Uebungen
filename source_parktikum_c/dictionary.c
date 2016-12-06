#include "../header_praktikum_c/dictionary.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

typedef struct node node;

struct node{
	char* prefix;
	int isword; //isWord == false -> isWord = 0;
	node* child[26];
};

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
void insert_word_and_prefix(node* parent, node* newNode, int index, int prefixLen);
void insert_word_as_sibbling(node* parent, node* newNode, int index);

void Dictionary_insert( Dictionary* dict, const char* word ){

	node* newNode = malloc(sizeof(node));
	*((*newNode).prefix) = *word;
	(*newNode).isword = 1;

	node* current = (*dict).root;
	node* parent;
	node* child;
	int j = 1;
	int i;
	while(1){
		for(i = 0; (*current).child[i] != NULL ;i++){
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

				/* prefix is not a real prefix of word
				 * insert a new prefix node as child of current at i. prefix sind die ersten j-1 stellen von word.
				 * kinder des neuen prefix sind das alte child[i] und word
				*/
				else{
					insert_word_and_prefix(current, newNode, i, j-1);
					return;
				};
			}
			else if(comp > 0){
				insert_word_as_sibbling(current, newNode, i);
				return;
			};
		};
		if(i <= 26){
			(*current).child[i] = newNode;
			return;
		};
		/*attach newNode to (*current).child by coping (*current).child in a new array
		else {
			node* c[];
			for(int z = 0; z < k; z++){
				c[z] = (*current).child[z];
			};
			c[k] = newNode;
			(*current).child = c;
		}*/
	return;
	};
};

/*If a word is found with the same prefix as (*newNode).prefix, insert a new prefix node as child of current
*Insert the found word and new word as childs of the new prefix
*/
void insert_word_and_prefix(node* parent, node* newNode, int index, int prefixLen){
	node* newPrefix = malloc(sizeof(node));
	(*newPrefix).prefix = malloc(prefixLen);
	strncpy((*newPrefix).prefix, (*newNode).prefix, prefixLen);
	(*newPrefix).isword = 0;
	node* cur_child = (*parent).child[index];
	(*parent).child[index] = newPrefix;
	if(strcmp((*cur_child).prefix, (*newNode).prefix) < 0){
		(*newPrefix).child[0] = cur_child;
		(*newPrefix).child[1] = newNode;
	}
	else{
		(*newPrefix).child[1] = cur_child;
		(*newPrefix).child[0] = newNode;
	}
};

void insert_word_as_sibbling(node* parent, node* newNode, int index){
	node* temp_childs[26-index];
	int i;
	int j ;
	for(i = index; i < 26; i++){
		temp_childs[i-index] = (*parent).child[i];
	};
	(*parent).child[index] = newNode;
	for(j = index+1; j < 26; j++){
		(*parent).child[j] = temp_childs[j-(index+1)];
	};
};
int isIn_child(const node* current,const char* word);

//returns 1 if word is in Dictionary* dict
int Dictionary_isIn( const Dictionary* dict, const char* word ){

	node*current = (*dict).root;
	isIn_child(current, word);
	return 0;
};

//recursive helper or Dictionary_isIn()
int isIn_child(const node* current,const char* word ){

	if (strcmp((*current).prefix, word) == 0) {return 1;}
	else{
		int i;
		for(i = 0; i < 26; i++){
			if((*current).child[i] != NULL){
				isIn_child((*current).child[i], word);
			}
		};
	};
	return 0;
};

void print_word(const node* current);

void Dictionary_print( const Dictionary* dict ){
	node* current = (*dict).root;
	print_word(current);
};

//recursive helper for Dictionary_print()
void print_word(const node* current){

	if((*current).isword && (*current).prefix != NULL){printf("%s \n",(*current).prefix);};
	int i;
	for(i = 0; i < 26; i++){
		if((*current).child[i] != NULL){
			print_word((*current).child[i]);
		};
	};
};

void merge_dict_insert_source(Dictionary* dest, const node* current);

void Dictionary_merge( Dictionary* destination, const Dictionary* source){
	node* current = (*source).root;
	merge_dict_insert_source(destination, current);
};

void merge_dict_insert_source(Dictionary* dest, const node* current){
	if((*current).isword && (*current).prefix != NULL) {Dictionary_insert(dest, (*current).prefix);};
	int i;
	for(i = 0; i < 26; i++){
		if((*current).child[i] != NULL){
			merge_dict_insert_source(dest, (*current).child[i]);
		};
	};
};



