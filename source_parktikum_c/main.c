#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

	FILE* text;
	char block[500];
	text = fopen("text1.txt", "r");
	if(text == NULL){
		perror("Error opening file");
		return(-1);
	}
	while(strlen(block) < 500){
		char temp[10];
		int i = 500 - strlen(block);
		fgets(temp,i,text);
		strcat(block, temp);
		strcat(block, " ");
	}
	fclose(text);
	printf("%s \n",block);
	return 0;
}
