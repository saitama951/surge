#define _GNU_SOURCE
#include "stdio.h"
#include "sys/uio.h"
#include "stdlib.h"
#include "errno.h"
#include "sys/ptrace.h"
#define module 6
void main(int argc,char* argv[])
{
	int errcode=0;
	#if module==1
		struct iovec remote[1];
		struct iovec local[1];

		pid_t pid=atoi(argv[2]);
		ssize_t size=atoi(argv[3]);

		char *buf=(char*)malloc(size);

		local[0].iov_base = buf;
		local[0].iov_len = 512;

		remote[0].iov_base = (void *)strtoul(argv[1],NULL,16);
		remote[0].iov_len = size;
		ssize_t nread=process_vm_readv(pid,local,1,remote,1,0);
		printf("%d",nread);
		if(nread<0)
		{
			goto x;
		}
		printf("string:%s\n",local[0].iov_base);
	

	#elif module==2
		struct iovec remote[1];
		struct iovec local[10];

		pid_t pid=atoi(argv[2]);
		ssize_t size=atoi(argv[3]);
		int i=0;
		char **buf;
		buf=(char **)malloc(10);
		for(i=0;i<10;i++)
		{
			buf[i]=(char*)malloc(size/10);
		}

		for(i=0;i<10;i++)
		{
			local[i].iov_base=buf[i];
			local[i].iov_len=size/10;
		}
		
		remote[0].iov_base = (void *)strtoul(argv[1],NULL,16);
		remote[0].iov_len = size;
		
		ssize_t nread=process_vm_readv(pid,local,10,remote,1,0);
		
		if(nread<0)
			goto x;
		for(i=0;i<10;i++)
		{
			printf("string:%s\n",local[i].iov_base);
		}
	

	#elif module==3
		struct iovec remote[1];
		struct iovec local[20];

		pid_t pid=atoi(argv[2]);
		ssize_t size=atoi(argv[3]);
		int i=0;
		char **buf;
		
		buf=(char **)malloc(20);
		
		for(i=0;i<20;i++)
		{
			buf[i]=(char*)malloc(size/20);
		}

		for(i=0;i<20;i++)
		{
			local[i].iov_base=buf[i];
			local[i].iov_len=size/20;
		}
		
		remote[0].iov_base = (void *)strtoul(argv[1],NULL,16);
		remote[0].iov_len = size;
		
		ssize_t nread=process_vm_readv(pid,local,20,remote,1,0);
		
		if(nread<0)
			goto x;
		for(i=0;i<20;i++)
		{
			printf("string:%s\n",local[i].iov_base);
		}

		
	
	#elif module==4
		struct iovec remote[1];
		struct iovec local[50];

		pid_t pid=atoi(argv[2]);
		ssize_t size=atoi(argv[3]);
		int i=0;
		char **buf;
		
		buf=(char **)malloc(50);
		
		for(i=0;i<50;i++)
		{
			buf[i]=(char*)malloc(size/50);
		}

		for(i=0;i<50;i++)
		{
			local[i].iov_base=buf[i];
			local[i].iov_len=size/50;
		}
		
		remote[0].iov_base = (void *)strtoul(argv[1],NULL,16);
		remote[0].iov_len = size;
		
		ssize_t nread=process_vm_readv(pid,local,50,remote,1,0);
		
		if(nread<0)
			goto x;
		for(i=0;i<50;i++)
		{
			printf("string:%s\n",local[i].iov_base);
	    }
	#elif module==5
		struct iovec remote[10];
		struct iovec local[100];

		pid_t pid=atoi(argv[11]);
		ssize_t size=atoi(argv[12]);
		int i=0;
		char **buf;

		buf=(char**)malloc(100);

		for(i=0;i<100;i++)
		{
			buf[i]=(char*)malloc(size/100);
		}
		for(i=0;i<100;i++)
		{
			local[i].iov_base=buf[i];
			local[i].iov_len=size/100;
		}

		for(i=0;i<10;i++)
		{
			remote[i].iov_base = (void*)strtoul(argv[i+1],NULL,16);
			remote[i].iov_len = size;
		}
		
		ssize_t nread=process_vm_readv(pid,local,100,remote,10,0);
		
		if(nread<0)
			goto x;
		for(i=0;i<100;i++)
		{
			printf("string:%s\n",local[i].iov_base);
	    }

	

	#elif module==6

		struct iovec remote[30];
		struct iovec local[100];

		pid_t pid=atoi(argv[31]);
		ssize_t size=atoi(argv[32]);
		int i=0;
		char **buf;

		buf=(char**)malloc(100);

		for(i=0;i<100;i++)
		{
			buf[i]=(char*)malloc(size/100);
		}
		for(i=0;i<100;i++)
		{
			local[i].iov_base=buf[i];
			local[i].iov_len=size/100;
		}

		for(i=0;i<30;i++)
		{
			remote[i].iov_base = (void*)strtoul(argv[i+1],NULL,16);
			remote[i].iov_len = size;
		}
		
		ssize_t nread=process_vm_readv(pid,local,100,remote,30,0);
		
		if(nread<0)
			goto x;
		for(i=0;i<100;i++)
		{
			printf("string:%s\n",local[i].iov_base);
	    }


		
	#endif

	
	x:
        switch (errno) {
            case EINVAL:
              printf("ERROR: INVALID ARGUMENTS.\n");
              break;
            case EFAULT:
              printf("ERROR: UNABLE TO ACCESS TARGET MEMORY ADDRESS.\n");
              break;
            case ENOMEM:
              printf("ERROR: UNABLE TO ALLOCATE MEMORY.\n");
              break;
            case EPERM:
              printf("ERROR: INSUFFICIENT PRIVILEGES TO TARGET PROCESS.\n");
              break;
            case ESRCH:
              printf("ERROR: PROCESS DOES NOT EXIST.\n");
              break;
            default:
              printf("");
   		 }
}
