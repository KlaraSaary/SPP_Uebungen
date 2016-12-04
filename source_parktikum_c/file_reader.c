#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header_praktikum_c/list.h"
#include <math.h>


LinkedList* read_text_file( const char* filename, int blockSize ) {

	FILE* text;
	text = fopen(filename, "r");
	int blocksize = blockSize;
	if(text == NULL){
		perror("Error opening file");
		return -1;
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
		//printf("i: %i strlen(b): %i strlen(temp): %i %s \n", i, strlen(b), strlen(temp), temp);
		if(i >= strlen(temp)){		//if there is enough place left in b, attach string temp to b
			strcat(b, temp);
		}
		/*if string b has reached blocksize or something near,
		 * allocate memory and copy string b.
		 * Then append the char pointer to LinkedList
		 */
		else{
			a[j] = (char*) malloc(sizeof(char)*strlen(b)+1);
			if(a[j] == NULL){printf("\nKein Speicher vorhanden\n");};
			*a[j] = '\0';
			//printf("B: %s \n", b);
			strcpy(a[j], b);
			LinkedList_append(list, a[j]);
			//printf("%s \n",LinkedList_getData(LinkedList_getLast(list)));
			*b = '\0';
			//printf("%i : %s \n",j, a[j]);
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
	printf("%s \n",a[s]);
	free(b);
	return list;
};

int main(){
	read_text_file("/home/klara/TUD/16_17WS/SPP/Uebung/Lab1/text1.txt", 16000);
	return 0;
}
