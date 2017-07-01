#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <rpc/xdr.h>

#define MAX_LINE 1024





double add(int x,float y)
{
	char recvBuff[MAX_LINE];
	int sfd;
	int port = 8000;
	char *str = "test string";
	char *serverIP = "127.0.0.1";

	int sockfd=0, n=0;
	char recvline[4096],sendline[4096];
	struct sockaddr_in serv_addr;

	/*if (argc!=2){
		printf("usage:./client <ipaddress>\n");
		exit(0);
	}*/

	

	memset(recvBuff,'0',sizeof(recvBuff));
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0) )<0){
		printf("creat socket error:%s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//connect to the server 
	if (connect(sockfd,(struct sockaddr * )&serv_addr, sizeof(serv_addr))<0) 
	{
		printf("\n Error : Connect Failed\n");
	}
     

	//xdr encode the information
	XDR xdrs;
	char buf[MAX_LINE];
	xdrmem_create(&xdrs,buf,MAX_LINE,XDR_ENCODE);
    
    char req[]="request";
    char funcname[]="add";
    
    char *p1=req;
    char *p2=funcname; 

	xdr_string(&xdrs,&p1,sizeof(p1));
	xdr_string(&xdrs,&p2,sizeof(p2));
	
    printf("%d\n", x);
    printf("%f\n", y);
	xdr_int(&xdrs,&x);
	xdr_float(&xdrs,&y);
     


    //send the message to server
    int status  = send(sockfd,buf,MAX_LINE,0);
    printf("%s\n", "send the message");
    //xdr_destroy(&xdrs);
     
    char bufferR[MAX_LINE];


    /*if (connect(sockfd,(struct sockaddr * )&serv_addr, sizeof(serv_addr))<0) 
	{
		printf("\n Error : Connect Failed\n");
	}*/

	char buf_xdr[120];
    int numberbytes;
    if((numberbytes=recv(sockfd,buf_xdr,MAX_LINE-1,0))==-1)
    {
    	printf("%s\n", "recv");
    }
    else if(numberbytes==0)
    {
    	printf("%s\n", "Remote server has shutdown!");
    }
   
    char buff[100];
    xdrmem_create(&xdrs,buf_xdr,MAX_LINE,XDR_DECODE);
    

 	double z;
  	xdr_double(&xdrs,&z);

  	printf("%d+%f=%lf\n", x,y,z);

    //xdr_string(&xdrs,&result,120);
    
    //recv(sockfd,buf,MAX_LINE,0);
    //XDR xdrs;
    /*xdrmem_create(&xdrs,buf,sizeof(buf),XDR_DECODE);
      
    char name1[20];
    char *result=name1;
    xdr_string(&xdrs,&result,sizeof(result));*/
    

    //printf("call_result:%s\n",result);
    xdr_destroy(&xdrs);
    return 0;

}




int  sayHello(char * myName,int myAge)
{
	char recvBuff[MAX_LINE];
	int sfd;
	int port = 8000;
	char *str = "test string";
	char *serverIP = "127.0.0.1";

	int sockfd=0, n=0;
	char recvline[4096],sendline[4096];
	struct sockaddr_in serv_addr;

	/*if (argc!=2){
		printf("usage:./client <ipaddress>\n");
		exit(0);
	}*/

	

	memset(recvBuff,'0',sizeof(recvBuff));
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0) )<0){
		printf("creat socket error:%s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//connect to the server 
	if (connect(sockfd,(struct sockaddr * )&serv_addr, sizeof(serv_addr))<0) 
	{
		printf("\n Error : Connect Failed\n");
	}
     

	//xdr encode the information
	XDR xdrs;
	char buf[120];
	char *p=myName;
	xdrmem_create(&xdrs,buf,120,XDR_ENCODE);
    
    char req[]="request";
    char funcname[]="sayHello";
    
    char *p1=req;
    char *p2=funcname; 

	xdr_string(&xdrs,&p1,sizeof(p1));
	xdr_string(&xdrs,&p2,sizeof(p2));
	xdr_string(&xdrs,&p,10);
	xdr_int(&xdrs,&myAge);
     


    //send the message to server
    int status  = send(sockfd,buf,MAX_LINE,0);
    printf("%s\n", "send the message");
    //xdr_destroy(&xdrs);
     
    char bufferR[MAX_LINE];


    /*if (connect(sockfd,(struct sockaddr * )&serv_addr, sizeof(serv_addr))<0) 
	{
		printf("\n Error : Connect Failed\n");
	}*/

	char buf_xdr[120];
    int numberbytes;
    if((numberbytes=recv(sockfd,buf_xdr,MAX_LINE-1,0))==-1)
    {
    	printf("%s\n", "recv");
    }
    else if(numberbytes==0)
    {
    	printf("%s\n", "Remote server has shutdown!");
    }
   
    char buff[100];
    xdrmem_create(&xdrs,buf_xdr,MAX_LINE,XDR_DECODE);
    char str1[100];
 	char *result = str1;
  	xdr_string(&xdrs,&result,MAX_LINE);

  	//printf("%d\n", sizeof(result));

    //xdr_string(&xdrs,&result,120);
    
    //recv(sockfd,buf,MAX_LINE,0);
    //XDR xdrs;
    /*xdrmem_create(&xdrs,buf,sizeof(buf),XDR_DECODE);
      
    char name1[20];
    char *result=name1;
    xdr_string(&xdrs,&result,sizeof(result));*/
    

    printf("call_result:%s\n\n",result);
    xdr_destroy(&xdrs);
    return 0;

}


int main(void)
{
	/*char recvBuff[MAX_LINE];
	int sfd;
	int port = 8000;
	char *str = "test string";
	char *serverIP = "127.0.0.1";

	int sockfd=0, n=0;
	char recvline[4096],sendline[4096];
	struct sockaddr_in serv_addr;

	memset(recvBuff,'0',sizeof(recvBuff));
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0) )<0){
		printf("creat socket error:%s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(sockfd,(struct sockaddr * )&serv_addr, sizeof(serv_addr))<0) 
	{
		printf("\n Error : Connect Failed\n");
		return 1;
	}
	while((n= read(sockfd, recvBuff,sizeof(recvBuff)-1))>0)
	{

		recvBuff[n]=0;
		if(fputs(recvBuff,stdout)==EOF)
		{
			printf("\n Error :Fputs error");
		}
	}
	if(n<0)
	{
		printf("\n Read Error");
	}*/
    while(1)
    {
    	printf("call sayHello funtcion, press 1.\n\n");
    	printf("call add funtcion, press 2.\n\n");
    	int a;
    	scanf("%d",&a);
    	switch(a)
    	{
    		case 1:
    		{
    			printf("Please input the name and age!\n");
    			int age;
    			char name[20];
    			char *p=name;
    			scanf("%s%d",name,&age);
    			sayHello(name,age);
    			break;
    		}
    		case 2: 
    		{
    			printf("Please input the int x and float y!\n");
    			int x;
    			float y;
    			scanf("%d%f",&x,&y);
    			add(x,y);
				break;
			}
			default:printf("error\n");

    	}
    } 
    /*int age=30;
    char *name="chenfei";

    int x=10;
    float y=3.4;
	sayHello(name,age);
	add(x,y);
	return 0;*/
}
