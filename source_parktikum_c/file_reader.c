#include "../header_praktikum_c/file_reader.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

int main(){
	char b = read_text_file("text1.txt", 0);
	printf("This is block: %s \n", b);
}

char read_text_file(const char* filename, int blockSize){
	FILE *text;
	char block[16000];
	char* p = block;
	text = fopen(filename, "r");
	if(text == NULL){
      		perror("Error opening file");
      		return(-1);
   	}
	strcat(block, fgets(block, 500, (FILE*)text));
	fclose(text);
	return block;
}
