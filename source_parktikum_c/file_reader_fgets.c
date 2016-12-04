#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header_praktikum_c/list.h"


LinkedList* read_text_file_fgets( const char* filename, int blockSize ) {

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
	char temp[blocksize];
	char* b;
	b =(char*) malloc(blocksize);
	*b = '\0';
	LinkedList* list = LinkedList_create();
	int i = blocksize;
	int j = 0;
	while(fgets(temp,blocksize,text) != NULL){ 	//While the end of text is not reached, save one line of text in temp
		i = blocksize - strlen(b);
		if(i >= strlen(temp)){		//if there is enough place left in b, attach string temp to b
			strcat(b, temp);
		}
		/*if string b has reached blocksize or something near,
		 * allocate memory and copy string b.
		 * Then append the char pointer to LinkedList
		 */
		else if(strlen(temp) > i && strlen(temp) < blocksize){

		}
		else{
			a[j] = (char*) malloc(sizeof(char)*strlen(b)+1);
			if(a[j] == NULL){printf("\nKein Speicher vorhanden\n");};
			*a[j] = '\0';
			LinkedList_append(list, strcpy(a[j], b));
			*b = '\0';
			j++;
		} 
	}
	//save last string in b in LinkedList
	a[j] = (char*) malloc(sizeof(char)*strlen(b));
	if(a[j] == NULL){printf("\nKein Speicher vorhanden\n");};
	*a[j] = '\0';
	strcpy(a[j], b);
	LinkedList_append(list, a[j]);

	fclose(text);
	//printf("%s \n",a[s]);
	free(b);
	return list;
};

/*int main(){
	read_text_file_fgets("/home/klara/TUD/16_17WS/SPP/Uebung/Lab1/text1.txt", 16000);
	return 0;
}*/

