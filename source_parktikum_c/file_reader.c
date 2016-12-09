#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header_praktikum_c/file_reader.h"
#include <ctype.h>


LinkedList* read_text_file( const char* filename, int blockSize ) {

	FILE* text;
	text = fopen(filename, "r");
	int blocksize = blockSize;
	if(text == NULL){
		perror("Error opening file");
		exit(EXIT_FAILURE);
		//return -1;
	}
	fseek(text, 0, SEEK_END);
	long size = ftell(text);
	fseek(text, 0, SEEK_SET);
	int s = (int)(size/blocksize + 0.5);
	char *a[s];
	int temp;
	char t[50];
	t[0] = '\0';
	char* b;
	b =(char*) malloc(blocksize);
	*b = '\0';
	LinkedList* list = LinkedList_create();
	int j = 0;
	while((temp = fgetc(text)) != EOF){ 	//While the end of text is not reached, save one character of text in temp
		char c[2] = {(char)temp, '\0'}; //Save character in string (char array)
		strcat(t, c); //attach string to t until t is a whole word
		if(!isalpha(temp)){
			if((blocksize-strlen(b)) > strlen(t)){		//if there is enough place left in b, attach string t to b
				strcat(b, t);
				*t = '\0';
			}
			/*if string b has reached blocksize or something near,
			 * allocate memory and copy string b.
			 * Then append the char pointer to LinkedList
			 */
			else{
				a[j] = (char*) malloc(sizeof(char)*strlen(b)+1);
				if(a[j] == NULL){printf("\nKein Speicher vorhanden\n");};
				*a[j] = '\0';
				LinkedList_append(list, strcpy(a[j], b));;
				strcpy(b,t);
				j++;
			}
		}
	}
	if(feof(text)){
		printf("end reched successfully\n");
	}

	//save last string in b in LinkedList
	a[j] = (char*) malloc(sizeof(char)*strlen(b));
	if(a[j] == NULL){printf("\nKein Speicher vorhanden\n");};
	*a[j] = '\0';
	strcpy(a[j], b);
	LinkedList_append(list, a[j]);


	fclose(text);
	free(b);
	return list;
};

