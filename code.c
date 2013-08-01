#include<stdio.h>
#include<stdlib.h>

#define BUFFER 1000000

char* text;
size_t text_len;

struct object
{
	int pos;	
	char* str;
	int ascii_sum;
	int ascii_prod;
	struct object* next;
}*DS[30];	
	
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
	text_len = fread(text,sizeof(char),BUFFER,fp);
	if(text_len == 0)
		error();
	else
		text[text_len-1]='\0';
	fclose(fp);
}

int str_copy(char* a, char* b)
{
	int i=0;
	while(b[i]!='\0')
	{
		a[i]=b[i];
		i++;
	}
	return i-1;
}

void print(char* array)
{
	int i=0;
	while(array[i]!='\0')
	{
		printf("%c",array[i]);
		i++;
	}
	//printf("\n");
	
}

size_t str_len(char* array)		//returns sizeof string (type : size_t)
{
	size_t i=0;
	while(array[i]!='\0')
		i++;
	return i;
}

void text_process()	//Ensures all letters are converted to lowercase letters and other characters replaced by $ 
{
	int i=0;
	while(text[i]!='\0')
	{
		int ascii = (int)text[i];		
		if(!(ascii >=97 && ascii <=122))
			if(ascii>=65 && ascii <=90)
				text[i]+=32;
			else
				text[i]='$';
		i++;
	} 
}

void append_DS(int length, char* string, int pos)
{
	struct object* a = malloc(sizeof(struct object));
	a->pos=pos;
	a->str=string;
	if(DS[length]!=NULL)
	{
		
		a->next=DS[length];
		DS[length]=a;
	}
	else
	{
		DS[length]=a;
		DS[length]->next=NULL;
	}
	//free(a);	
}

void construct_DS()
{
	int i=0;
	while(text[i]!='\0')
	{		
		if(text[i]!='$')
		{			
			char *temp_string;
			temp_string=(char *)malloc(30*sizeof(char));
			int pos=i;			
			while(text[i]!='$')
			{				
				temp_string[i-pos]=text[i];
				i++;
			}
			temp_string[i-pos]='\0';
			//printf("Len :%d",i-pos);
			append_DS(i-pos,temp_string,pos);
		}
		else 
			i++;
	}
}


void print_DS()
{
	int i;
	for(i=1;i<30;i++)
	{		
		struct object* temp = DS[i];
		(temp!=NULL)?printf("\n"):printf("");
		while(temp!=NULL)
		{
			print(temp->str);
			printf(" ");
			temp=temp->next;
		}
	}
}			
		

int main()
{
	text_scan();
	printf("\n After initial processing :\n");	
	text_process();
	print(text);
	printf("\n Classified by length :\n");
	construct_DS();
	print_DS();
	printf("\n");
	return 1;
}

