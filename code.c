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
}*hash_tab[30],*refined_hash_tab[30];
	
void error()
{
	printf("Error!");
	exit(0);
}

int str_copy(char* a, char* b)
{
	int i = 0;
	while(b[i]!='\0')
	{
		a[i] = b[i];
		i++;
	}
	return i-1;
}

size_t str_len(char* array)		
{
	size_t i = 0;
	while(array[i]!='\0')
		i++;
	return i;
}

int str_compare(char* a, char*b)
{
	int i = 0;
	while(a[i]!='\0' && b[i]!='\0')
	{		
		if(a[i]!=b[i]) 
			return 0;
		else if(a[i+1]=='\0' && b[i+1]=='\0')
			return 1;
		i++;			
	}
	return 0;
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

void text_scan()
{
	FILE *fp;
	fp = fopen("sample.txt","r");
	text = malloc((BUFFER+1)*sizeof(char));
	if(fp==NULL)
		error();
	text_len = fread(text,sizeof(char),BUFFER,fp);
	if(text_len == 0)
		error();
	else
		text[text_len-1] = '\0';
	fclose(fp);
}

void text_process()	
{
	int i = 0;
	while(text[i]!='\0')
	{
		int ascii = (int)text[i];		
		if(!(ascii >=97 && ascii <=122))
			if(ascii>=65 && ascii <=90)
				text[i]+=32;
			else if(ascii>=48 && ascii <=57)
				{}
			else
				text[i]='$';
		i++;
	} 
}

void append_object(struct object** hash_tab, int length, char* string, int pos, int ascii_sum, int flag)
{
	struct object* a = malloc(sizeof(struct object));
	a->pos = pos;
	a->str = string;
	a->ascii_sum = ascii_sum;
	a->flag = flag;
	a->next = NULL;
	if(hash_tab[length]!=NULL)
	{
		
		a->next = hash_tab[length];
		hash_tab[length] = a;
	}
	else
	{
		hash_tab[length] = a;
		hash_tab[length]->next = NULL;
	}
}

void construct_hash_tab()
{
	int i = 0;
	while(text[i]!='\0')
	{		
		if(text[i]!='$')
		{			
			char *temp_string;
			temp_string = (char *)malloc(30*sizeof(char));
			int ascii_sum = 0,ascii_prod = 1,pos = i;			
			while(text[i]!='$')
			{				
				temp_string[i-pos] = text[i];
				ascii_sum+=(int)text[i];
				i++;
			}
			temp_string[i-pos] = '\0';
			append_object(hash_tab,i-pos,temp_string,pos,ascii_sum,0);
		}
		else 
			i++;
	}
}

void print_hash_tab(struct object ** hash_tab)
{
	int i;
	for(i=2;i<30;i++)
	{		
				
		struct object* temp = hash_tab[i];
		(temp!=NULL)?printf("\n[%d]",i):i;
		while(temp!=NULL)
		{
			printf(temp->str);
			printf("(%d){%d},",temp->ascii_sum,temp->pos);
			printf(" ");
			temp=temp->next;
		}
	}
}
	
int isadded_str(struct object* a,char* string,int ascii_sum)		
{
	struct object* b = a;
	int ret_val = 0;
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

struct object* rip_sword_hash_tab(struct object *a)
{
	struct object *b = (struct object *)malloc(sizeof(struct object)), *n;
	b = a;
	while(b!=NULL)
	{
		if(b->flag==-1)
			if(b==a)
			{ 	a = b->next;
				n = b;
			}
			else
				n->next = b->next; 
			
		else
			n = b;
		b = b->next;
 	}
	return a;
}
	
void construct_refined_hash_tab()
{
	int i;
	for(i=2;i<30;i++)
	{
		int* temp_array  = (int *) malloc(sizeof(int)*10000);
		int temp_used = 0;		
		struct object* obj = hash_tab[i];
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
				append_object(refined_hash_tab,str_len(obj->str),obj->str,obj->pos,obj->ascii_sum,f);
				temp_used++;
			}
			else
				if(!isadded_str(refined_hash_tab[str_len(obj->str)],obj->str,obj->ascii_sum))
					append_object(refined_hash_tab,str_len(obj->str),obj->str,obj->pos,obj->ascii_sum,f);
			obj=obj->next;
		}
		refined_hash_tab[i] = rip_sword_hash_tab(refined_hash_tab[i]);
	}
}

int is_anagram(char *a, char *b)
{
	int x[26]={0},y[26]={0},i;
	for(i=0;a[i]!='\0';i++)
	{
		int ascii = a[i];
		x[ascii-97]++;
	}
	for(i=0;b[i]!='\0';i++)
	{
		int ascii = b[i];
		y[ascii-97]++;
	}
	for(i=0;i<26;i++)
		if(x[i]!=y[i])
			return 0;
	return 1;
}

void anagram_scan()
{
	int i;
	for(i=2;refined_hash_tab[i]!=NULL && i<30;i++)
	{
		
		while(refined_hash_tab[i]!=NULL)
		{
					
			struct object *pre = refined_hash_tab[i],*obj = refined_hash_tab[i]->next;
			while(obj!=NULL)
			{
								
				if(is_anagram(pre->str,obj->str))
				{
					if(pre->flag!=2)
					{
						printf(" %s",pre->str);
						pre->flag = 2;
					}					
					if(obj->flag!=2)
					{
						printf(" %s",obj->str);
						obj->flag = 2;
					}
				}
				obj = obj->next;
			}
			
			refined_hash_tab[i] = refined_hash_tab[i]->next;
		}
	printf("\n");
		
	}				
				
}				

void bubble_sort(struct object **array,int n)
{
	struct object *swap;
	int c,d;
	for(c=0;c<(n-1);c++)
	{
    		for(d=0;d<n-c-1;d++)
    		{
			if(array[d]!=NULL && array[d+1]!=NULL)
	      			if(array[d]->pos > array[d+1]->pos)
	      			{
					swap = array[d];
					array[d] = array[d+1];
					array[d+1] = swap;
	      			}
    		}
  	}
}
		
int main()
{
	text_scan();
	//printf("\n--After initial processing :\n");	
	text_process();
	//print(text);
	construct_hash_tab();
	//printf("\n--Classified by length :");
	//print_hash_tab(hash_tab);
	construct_refined_hash_tab();
	//printf("\n--Classified by ASCII sum :");
	bubble_sort(refined_hash_tab,30);
	//print_hash_tab(refined_hash_tab);
	//printf("\n--Anagram scanner results :");
	anagram_scan();
	return 1;
}
