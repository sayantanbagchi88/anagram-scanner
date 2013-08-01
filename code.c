#include<stdio.h>
#include<stdlib.h>

#define BUFFER 1000000
char* text;
void error()
{
	printf("Error!");
	exit(0);
}

void text_scan()
{
	FILE *fp;
	fp=fopen("sample.txt","r");
	text = malloc((BUFFER+1)*sizeof(char));
	if(fp==NULL)
		error();
	size_t text_len = fread(text,sizeof(char),BUFFER,fp);
	if(text_len == 0)
		error();
	else
		text[text_len-1]='\0';
	fclose(fp);
}

void print(char* array)
{
	int i=0;
	while(array[i]!='\0')
	{
		printf("%c",text[i]);
		i++;
	}
	printf("\n");
	
}

size_t str_len(char* array)		//returns sizeof string (type : size_t)
{
	size_t i=0;
	while(array[i]!='\0')
		i++;
	return i;
}

void remove_bull_in_text(char* array)	//Ensures all letters are converted to lowercase letters and other characters replaced by $ 
{
	int i=0;
	while(array[i]!='\0')
	{
		int ascii = (int)array[i];		
		if(!(ascii >=97 && ascii <=122))
			if(ascii>=65 && ascii <=90)
				array[i]+=32;
			else
				array[i]='$';
		i++;
	} 
}

int main()
{
	text_scan();
	remove_bull_in_text(text);
	print(text);
	return 1;
}
