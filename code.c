#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUFFER 1000000

char* text;
size_t text_len;


struct object
{
	int pos;	
	char* str;
	int ascii_sum;
	struct object* next;
}*DS[30],*filt_DS[30];	
	
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

void append_object(struct object** DS, int length, char* string, int pos, int ascii_sum)
{
	struct object* a = malloc(sizeof(struct object));
	a->pos = pos;
	a->str = string;
	a->ascii_sum = ascii_sum;
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
			int ascii_sum=0,ascii_prod=1,pos=i;			
			while(text[i]!='$')
			{				
				temp_string[i-pos]=text[i];
				ascii_sum+=(int)text[i];
				i++;
			}
			temp_string[i-pos]='\0';
			//printf("Len :%d",i-pos);
			append_object(DS,i-pos,temp_string,pos,ascii_sum);
		}
		else 
			i++;
	}
}

void print_DS(struct object ** DS)
{
	int i;
	for(i=1;i<30;i++)
	{		
		struct object* temp = DS[i];
		(temp!=NULL)?printf("\n"):i;
		while(temp!=NULL)
		{
			print(temp->str);
			printf("(%d),",temp->ascii_sum);
			printf(" ");
			temp=temp->next;
		}
	}
}
			
	
void print_array(int* temp_array, int temp_used)
{	int i=0;
	while(i<temp_used){
		printf("A%d ",temp_array[i]);
	i++;}

}
	
void process_DS()
{
	int i;
	for(i=2;i<30;i++)
	{
		int* temp_array=(int *) malloc(sizeof(int)*1000);
		int temp_used=0;		
		struct object* obj = DS[i];
		while(obj!=NULL)
		{
			int k=0,f=-1;
			while(k<temp_used && f!=1)
			{
				if(temp_array[k]==obj->ascii_sum)
					f=1;
				k++;
			}
			if(f!=1)
			{
				temp_array[temp_used]=obj->ascii_sum;
				append_object(filt_DS,str_len(obj->str),obj->str,obj->pos,obj->ascii_sum);
				temp_used++;
			}
			obj=obj->next;
		}
		//print_array(temp_array,temp_used);
		/*obj = DS[i];
		while(obj!=NULL)
		{
			int j;
			for(j=0;j<temp_used;j++)
				if(temp_array[j]==obj->ascii_sum)
					
			obj=obj->next;
		}*/		
	}
}
		
int main()
{
	text_scan();
	printf("\n--After initial processing :\n");	
	text_process();
	print(text);
	construct_DS();
	printf("\n--Classified by length :");
	print_DS(DS);
	process_DS();
	printf("\n--Classified by ASCII sum :");
	print_DS(filt_DS);
	printf("\n");
	return 1;
}

