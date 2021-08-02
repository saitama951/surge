#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "malloc.h"
#define module 6

void generate_random_text(char *text,int msize)
{
	int i=0,randint=0;	
	
	for(i=0;i<msize-1;i++)
	{
		randint = (rand()%26)+97;
	
		text[i]=(char)randint;
	}
	text[i]='\0';

}

int main()
{
	
	#if module==1
		char *text=(char*)malloc(512);
		generate_random_text(text,512);
		printf("%llx %d %d",text,getpid(),512);
		//free(text);
	#elif module==2
		char *text=(char *)malloc(1024);
		generate_random_text(text,1024);
		printf("%llx %d %d",text,getpid(),1024);
		//free(text);
	#elif module==3
		char *text=(char *)malloc(1024*1024);
		generate_random_text(text,1024*1024);
		printf("%llx %d %d",text,getpid(),1024*1024);
		free(text);
	#elif module==4
		char *text=(char *)malloc(1204*1024*1024);
		generate_random_text(text,1024*1024*1024);
		printf("%llx %d %d",text,getpid(),1024*1024*1024);
		free(text);
	#elif module==5
		char **text;
		int i;
		text=(char**)malloc(10);
		for(i=0;i<10;i++)
		{
			text[i]=(char*)malloc(512);
			generate_random_text(text[i],512);
		}
		for(i=0;i<10;i++)
		{
			printf("%llx ",text[i]);
		}
		printf("%d %d",getpid(),512);
	#elif module==6
		char **text;
		int i;
		text=(char**)malloc(30);
		for(i=0;i<30;i++)
		{
			text[i]=(char*)malloc(512);
			generate_random_text(text[i],512);
		}
		for(i=0;i<30;i++)
		{
			printf("%llx ",text[i]);
		}
		printf("%d %d",getpid(),512);
	#endif

	//printf("%llx %d %d",text,getpid(),sizeof(text));
	fflush(stdout);
	sleep(1000);

	return 0;
}
