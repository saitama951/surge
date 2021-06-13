#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
int main()
{
int fd,return_len,count;
char chbuf[128];
char buff[100]="my name is sanjam!!!!"; //block of memory allocated
unsigned long i=(unsigned long)buff;//pointing to the block memory

	fd=open("/dev/phy_read",O_RDWR); //opening the device file
	if(fd < 0){
		printf("\nUnable to open file\n");
		return 0;
	}
	else
	{
		printf("\nfile is opened successfully \n"); 
		sprintf(chbuf,"%lx",i);//converting the address to string
		return_len = write(fd,&chbuf,sizeof(chbuf));//writing it into the device file
		printf("\n Successfully write %d %lx %lx %s\n",return_len,buff,i,chbuf); //checking if all addresses are same
		//count=read(fd,&chbuf,sizeof(chbuf));
		//printf("successfully read %d %s\n",count,chbuf);
		sleep(2);
		//read(fd,&buff,sizeof(buff));
		close(fd);
		return 0;
	}
}



