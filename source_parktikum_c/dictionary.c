#include "../header_praktikum_c/dictionary.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

typedef struct node node;

struct node{
	char* prefix;
	int isword;
	node* child[26];
};

struct Dictionary{
	node* root;
};

Dictionary* Dictionary_create(){
	Dictionary* dict = malloc(sizeof(Dictionary));
	dict->root = malloc(sizeof(node));
	node root;
	root.isword = 0;
	root.prefix = NULL;
	memset(root.child, 0, 26*8);
	*((*dict).root) = root;
	return dict;
};

void free_current_dict(node* current);

void Dictionary_delete( Dictionary* dict ){
	node* current = (*dict).root;
	free_current_dict(current);
};

//recursive helper to free all nodes in dictionary
void free_current_dict(node* current){
	int i;
	for(i = 0; i < 26; i++){
		if((*current).child[i] != NULL){
			free_current_dict((*current).child[i]);
		};
	};
	free((*current).prefix);
	free(current);
};

void insert_word_and_prefix(node* parent, node newNode, int index, int prefixLen);
void insert_word_as_sibbling(node* parent, node newNode, int index);

void Dictionary_insert( Dictionary* dict, const char* word ){
	//printf("here %s \n", word);
	node newNode;// = malloc(sizeof(node));
	newNode.prefix = word;
	newNode.isword = 1;
	memset(newNode.child,0,26*8);

	int j = 1;
	int i = 0;
	int insert_word = 0; //boolean: Is word already inserted

	node* current = (*dict).root;
	//node* parent;
	while(current->child[i] != NULL){
	//	printf("while \n");
		//child = current->child[i];
	//	printf("str1: %s str2: %s comp: %i j: %i \n",current->child[i]->prefix,word, strncmp(current->child[i]->prefix, word, j), j);
		int comp = strncmp(current->child[i]->prefix, word, j);
		if(comp < 0) {i++;}
		else if (comp == 0){
			while(j <= strlen(current->child[i]->prefix) && strncmp(current->child[i]->prefix, word, j) == 0){
				j++;
			};
			if((j-1) == strlen(current->child[i]->prefix) && strlen(word) > j-1){ //child.prefix is a real prefix of word
				j = j-1;
				//*parent = *current;
				*current = *(current->child[i]);
				i = 0;
			}
			else if(strcmp(current->child[i]->prefix, word) == 0){ //prefix == word
				//printf("already in \n");
				if(!current->child[i]->isword) current->child[i]->isword = 1;
				insert_word = 1;
				//free(newNode);
				return;
				}
			/* prefix is not a real prefix of word
			 * insert a new prefix node as child of current at i. prefix sind die ersten j-1 stellen von word.
			 * kinder des neuen prefix sind das alte child[i] und word
			*/
			else{
	//			printf("current: %s i: %i j: %i word: %s \n", (*current).prefix, i, j, word);
				insert_word_and_prefix(current, newNode, i, j-1);
				insert_word = 1;
				return;
			};
		}
		else if(comp > 0){
		//	printf("comp > 0 %s %s %i \n", current->prefix, newNode.prefix, i);
			insert_word_as_sibbling(current, newNode, i);
			insert_word = 1;
			return;
		};
	}
	if(i < 26 && !insert_word){
		(*current).child[i] = (node*) malloc(sizeof(node));
		*((*current).child[i]) = newNode;
		//printf("word: %s \n", (*current).child[i]->prefix);
		//printf("i: %i %s \n", i, current->child[i]->prefix);
		insert_word=1;
		return;
	};
return;
};

/*If a word is found with the same prefix as (*newNode).prefix, insert a new prefix node as child of current
*Insert the found word and new word as childs of the new prefix
*/
void insert_word_and_prefix(node* parent, node newNode, int index, int prefixLen){
	node newPrefix;
	newPrefix.isword = 0;
	newPrefix.prefix = (char*) malloc(prefixLen+1);
	*(newPrefix.prefix) = '\0';
	memset(newPrefix.child, 0, 26*8);

	strncpy(newPrefix.prefix, newNode.prefix, prefixLen);
	node cur_child = (*(*parent).child[index]);

	newPrefix.child[0] = (node*) malloc(sizeof(node));
	newPrefix.child[1] = (node*) malloc(sizeof(node));

	if(strcmp(cur_child.prefix, newNode.prefix) < 0){
		*(newPrefix.child[0]) = cur_child;
		*(newPrefix.child[1]) = newNode;
	}
	else{
		*(newPrefix.child[1]) = cur_child;
		*(newPrefix.child[0]) = newNode;
	}
	(*(*parent).child[index]) = newPrefix;

};

void insert_word_as_sibbling(node* parent, node newNode, int index){
	int i;
	int last = 1; //boolean: Is this the last child?
	for(i = 25; i >= -1; i = i-1){
	//	printf("for i: %i \n", i);
		if(i != -1 && (*parent).child[i] == NULL){}
		else if (last){
			(*parent).child[i+1] = malloc(sizeof(node));
			(*(*parent).child[i+1]) = (*(*parent).child[i]);
			last = 0;
		//	printf("last: %i child %i + 1: %s \n",last, i, (*parent).child[i+1]->prefix);
		}
		else if(!last && i >= index){
			(*(*parent).child[i+1]) = (*(*parent).child[i]);
		}
		else if(i == index-1){
			(*(*parent).child[i+1]) = newNode;
		//	printf("child: %s sibbling: %s \n", (*parent).child[i+1]->prefix, (*parent).child[i+2]->prefix);
		};
	};

};
int isIn_child(const node* current,const char* word, int isIn);

//returns 1 if word is in Dictionary* dict
int Dictionary_isIn( const Dictionary* dict, const char* word ){

	node*current = (*dict).root;
	int i = isIn_child(current, word, 0);
	return i;
};

//recursive helper or Dictionary_isIn()
int isIn_child(const node* current,const char* word, int isIn){

	int is = isIn;
	if ((*current).prefix != NULL && (strcmp((*current).prefix, word) == 0)){
		is = 1;
	}
	else{
		int i;
		for(i = 0; i < 26 && !is; i++){
			if((*current).child[i] != NULL){
				int j = isIn_child((*current).child[i], word, is);
				if(j) is = 1;
			}
		};
	};
	return is;
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
		if((*current).child[i] == NULL){break;}
		else{
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

