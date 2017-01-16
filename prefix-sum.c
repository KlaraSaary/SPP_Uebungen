
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int main(){

int list[8]={0,1,2,3,4,5,6,7};
int* list1 = malloc(8*sizeof(int));
int* list2 = malloc(8*sizeof(int));
int* sol= malloc(8*sizeof(int));

int i=0;
int t =0;	
	
while(i<4){
	list1[i]=list[t]+list[t+1];
	i++;
	t+=2;
}

list2[0]=list1[0]+list1[1];
i=1;

while(i<3){
	list2[i]=list2[i-1]+list1[i+1];
	i++;
}
	
for(i=0;i<8;i++){
	if(i==0){
		
		sol[0]=list[0];
		printf("number0 %d \n",sol[0]);
	}
	else if(i==1){
		
		sol[1]=list1[0];
		printf("number1 %d \n",sol[1]);
	}
	else if(i==2){
		
		sol[2]=list1[0]+list[2];
		printf("number2 %d \n",sol[2]);
	}else{
		printf("%d ",i);
		if(i%2!=0){
			sol[i]=list2[(i-3)/2];
		}else{
			sol[i]=list2[(i-4)/2]+list[i];
		}
	}
}
	printf("\n list: ");
for(i=0;i<8;i++){
	
	printf("%d ",list[i]);
}
	printf("\n list1: ");
for(i=0;i<4;i++){
	
	printf("%d ",list1[i]);
}
	printf("\n list2: ");
for(i=0;i<3;i++){
	
	printf("%d ",list2[i]);
}
	printf("\n sol: ");
for(i=0;i<8;i++){
	
	
	
	
	printf("%d ",sol[i]);
}




return 0;
}