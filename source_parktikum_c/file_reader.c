#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header_praktikum_c/list.h"
#include <math.h>

int main() {

	FILE* text;
	int  blocksize = 500;
	text = fopen("text1.txt", "r");
	if(text == NULL){
		perror("Error opening file");
		return(-1);
	}
	fseek(text, 0, SEEK_END);
	long size = ftell(text);
	fseek(text, 0, SEEK_SET);
	int s = (int) round(size/blocksize + 0.5);
	char* a[s];
	char temp[500];
	char* b;
	b =(char*) malloc(blocksize);
	*b = '\0';
	LinkedList* list = LinkedList_create();
	int i = blocksize;
	int j = 0;
	while(fgets(temp,i,text) != NULL){
		i = blocksize - strlen(b);
		if(i >= strlen(temp)){
			strcat(b, temp);
		}
		else{
			a[j] = (char*) malloc(sizeof(char)*strlen(b));
			strcpy(a[j], b);
			LinkedList_append(list, a[j]);
			*b = '\0';
		} 
		printf("%s \n",a[j]);
		/*printf("%i \n",(int) strlen(b));
		fgets(temp,i,text);
		strcat(b, temp);*/
		j++;
	}
	fclose(text);
	//printf("%s \n",b);
	free(b);
	return 0;
}
