#include<stdio.h>
#include<stdlib.h>

#define BUFFER 10000000

char* text;
size_t text_len;


struct object
{
	int pos;	
	char* str;
	int ascii_sum;
	int flag;
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
	int i = 0;
	while(b[i]!='\0')
	{
		a[i]=b[i];
		i++;
	}
	return i-1;
}

void print(char* array)
{
	int i = 0;
	while(array[i]!='\0')
	{
		printf("%c",array[i]);
		i++;
	}
	
}

size_t str_len(char* array)		
{
	size_t i=0;
	while(array[i]!='\0')
		i++;
	return i;
}

int str_compare(char* a, char*b)
{
	int i=0;
	while(a[i]!='\0' && b[i]!='\0')
	{		
		if(a[i]!=b[i])
			return 0;
		else 
			i++;
	}
	return 1;
}

void text_process()	
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

void append_object(struct object** DS, int length, char* string, int pos, int ascii_sum, int flag)
{
	struct object* a = malloc(sizeof(struct object));
	a->pos = pos;
	a->str = string;
	a->ascii_sum = ascii_sum;
	a->flag = flag;
	a->next=NULL;
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
			append_object(DS,i-pos,temp_string,pos,ascii_sum,0);
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
			printf(temp->str);
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

int isadded_str(struct object* a,char* string,int ascii_sum)		//Checks for a word's existence & updates flags of possible anagrams
{
	struct object* b = a;
	int ret_val=0;
	while(b!=NULL)
	{
		if(str_compare(b->str,string)==1)
			ret_val = 1;
		if(b->ascii_sum==ascii_sum)
			b->flag = 1;
		b = b->next;
	}
	return ret_val;
}

int rip_single_words(struct object *a)
{
	struct object *b = (struct object *)malloc(sizeof(struct object)), *n;
	b=a;
	while(b!=NULL)
		if(b->flag==-1)
		{
			if(b==a)
				return -1;
			else
				n->next=b->next;
			b=b->next;
		}
		else
		{
			n = b;
			b = b->next;
		}
	return 0;
}
	
void construct_filt_DS()
{
	int i=2;
	for(i=2;i<30;i++)
	{
		int* temp_array  = (int *) malloc(sizeof(int)*10000);
		int temp_used = 0;		
		struct object* obj = DS[i];
		while(obj!=NULL)
		{
			int k=0,f = -1;
			while(k<temp_used && f!=1)
			{
				if(temp_array[k]==obj->ascii_sum)
					f=1;
				k++;
			}
			if(f!=1)
			{
				temp_array[temp_used] = obj->ascii_sum;
				append_object(filt_DS,str_len(obj->str),obj->str,obj->pos,obj->ascii_sum,f);
				temp_used++;
			}
			else
				if(!isadded_str(filt_DS[str_len(obj->str)],obj->str,obj->ascii_sum))
					append_object(filt_DS,str_len(obj->str),obj->str,obj->pos,obj->ascii_sum,f);
			obj=obj->next;
		}
		rip_single_words(filt_DS[i]);
	}
}

int is_anagram(char *a, char *b)
{
	int x[26]={0},y[26]={0},i=0;
	while(a[i]!='\0')
	{
		int ascii = a[i];
		x[ascii-97]++;
		i++;
	}
	i=0;
	while(b[i]!='\0')
	{
		int ascii = b[i];
		y[ascii-97]++;
		i++;
	}
	i=0;
	while(i<26)
	{
		if(x[i]!=y[i])
			return 0;
		i++;
	}
	return 1;
}

void anagram_scan()
{
	int i;
	for(i=2;i<30;i++)
	{
		
		while(filt_DS[i]!=NULL)
		{
					
			struct object *pre = filt_DS[i],*obj = filt_DS[i]->next;
			while(obj!=NULL)
			{
				if(is_anagram(pre->str,obj->str))
					printf(" %s-%s \n",pre->str,obj->str);
				obj=obj->next;
			}
			filt_DS[i]=filt_DS[i]->next;
		}
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
	construct_filt_DS();
	printf("\n--Classified by ASCII sum :");
	print_DS(filt_DS);
	printf("\n");
	printf("\n--Anagram scanner results :");
	anagram_scan();
	return 1;
}

