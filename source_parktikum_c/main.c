#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header_praktikum_c/list.h"

int main() {

	FILE* text;
	text = fopen("text1.txt", "r");
	if(text == NULL){
		perror("Error opening file");
		return(-1);
	}
	int  blocksize = 500;
	char temp[500];
	char* b;
	b =(char*) malloc(blocksize);
	*b = '\0';
	//LinkedList* list = LinkedList_create();
	int i = blocksize;
	int j = 1;
	while(i > 0){
		int i = 500 - strlen(b);
		fgets(temp,i,text);
		strcat(b, temp);
		j++;
	}
	fclose(text);
	printf("%s \n",b);
	free(b);
	return 0;
}
