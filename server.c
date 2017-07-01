#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <rpc/xdr.h>



#define MAX_LINE 1024

//define the type 
typedef char* (*Pre_ProcName) (char*);



//define the APM
typedef struct APM
{
  char *ProcName;        //sayHello;
  Pre_ProcName _ptr;     //pre_sayHello;
};
 

typedef struct APM ProcThalbe;

//define the func_tabel


int Regist_Proc(ProcThalbe *pt, char *Pro_name,Pre_ProcName rProc)
{
   

  char str[20];
  char *p=str;
  //strcpy(p,Pro_name);
  pt->ProcName  =(char*)malloc(10);
  strcpy(pt->ProcName,Pro_name);
  //pt->ProcName =p;
  pt->_ptr=rProc;
 //pt->sex[10]="man";

  //printf("%s\n", pt->ProcName);
  //printf("%s\n", "Registe the proc_function ok!");
  return 0; 
}


double add(int x,float y)
{
    //printf("%d\n", x);
    //printf("%f\n", y);
    double z;
    z=x+y;
    printf("call the add  function\n");
    return z;
}

double pre_add(char *request)
{
  XDR xdrs;
  char *buf=request;
  //char *p=buf;
  //strcpy(p,request);

  xdrmem_create(&xdrs,buf,MAX_LINE,XDR_DECODE);
   

  char str1[10];
  char *p1 = str1;
  xdr_string(&xdrs,&p1,sizeof(p1));

  char str2[10];
  char *p2 = str2;
  xdr_string(&xdrs,&p2,sizeof(p2));

  int x;
  xdr_int(&xdrs,&x);

  float y;
  xdr_float(&xdrs,&y);
  
  //printf("%d\n", x);
  //printf("%f\n", y);
  double z;
  z=add(x,y);

  printf("%lf\n", z);
  return z;
  
}


char* sayHello(char *myName, int myAge)
{
    char string1[20]="Hello,";
    char *p1=string1;


    char buf[20];
    memset(buf,0x00,sizeof(buf));
    sprintf(buf,"%d",myAge);
    char *p2=buf;
    strcat(p1,p2);

    char string2[20]=" years old "; 
    char *p3=string2;
    strcat(p1,p3);


    //itoa(myAge,str,10);
    //printf("%s+%s\n",&string1,&string2);
    //cout<<"Hello"+myName+"!";
    
    char bufname[20];
    char *p4=bufname;
    strcpy(p4,myName);
    strcat(p1,p4); 

    char string3[]="!";
    char *p5 = string3;
    strcat(p1,p5);
    return p1;
}

char* pre_sayHello(char* request)
{

  //printf("%s\n", "call the sayhello ");
  //decode the params
  XDR xdrs;
  char *buf = request;

  xdrmem_create(&xdrs,buf,120,XDR_DECODE);
   
  char str1[10];
  char *p1 = str1;
  xdr_string(&xdrs,&p1,sizeof(p1));

  char str2[10];
  char *p2 = str2;
  xdr_string(&xdrs,&p2,sizeof(p2));

  char myName[10];
  char *p3=myName;

  xdr_string(&xdrs,&p3,sizeof(p3));

  int myAge;
  xdr_int(&xdrs,&myAge); 
  

  //printf("myname:---%s\n", myName);
  
  //char call_buf[100];
  char result[100];
  //result = sayHello(myName,myAge);
  char *p=result;
  strcpy(p,sayHello(myName,myAge));

  //printf("pre_sayhello: %s\n",result);
  return  p;
}


int main()
{
  int listenfd = 0,connfd = 0;
  struct sockaddr_in serv_addr;

  char sendBuff[1025];  
  int numrv;  
  
  int received;

  //register the sayHello function


  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  printf("socket retrieve success\n");

  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;    
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  serv_addr.sin_port = htons(5000);    

  bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

  if(listen(listenfd, 10) == -1){
      printf("Failed to listen\n");
      return -1;
  }     

  while(1)
    {      
      connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request
      
      char buff[MAX_LINE];
      if ((received = recv(connfd,buff,MAX_LINE,0))<0)
      {
         printf("Failed to receive from client");
      }
       
      
      printf("%s\n", "receive the mesage");



      ProcThalbe pt[2];
      Pre_ProcName rProc;
      rProc = pre_sayHello;

      Regist_Proc(&pt[0],"sayHello",rProc);
      
      rProc = pre_add;
      Regist_Proc(&pt[1],"add",rProc);



      XDR xdrs;
      char buf[120];

      xdrmem_create(&xdrs,buff,120,XDR_DECODE);
      
      char name1[20];
      char *req=name1;
      xdr_string(&xdrs,&req,MAX_LINE);
      
      printf("%s\n", req); 
      
      char name2[20];
      char *proname=name2;
      xdr_string(&xdrs,&proname,MAX_LINE); 

     
      char *rpc_result = (char*)malloc(100);
      double z;
       if (strcmp(pt[0].ProcName,proname)==0)
       {
          //char *result_buffer = (char*)malloc(100);
        //printf("%s\n", "match");
          rpc_result=pre_sayHello(buff);
          //p= pre_sayHello(buff); 
          //p = (*pt._ptr)(buff);


          //printf("final_rpc_result:%s\n", rpc_result);
      //send(connfd,rpc_result,MAX_LINE,0);
     

      //XDR xdrs;
          char rpc_result_buf[120];
      
          char xdr_buf[120];
          xdrmem_create(&xdrs,xdr_buf,120,XDR_ENCODE);
          char name[120];
          char *p=name;
          strcpy(p,rpc_result);
       

       
          xdr_string(&xdrs,&p,MAX_LINE);
       
          int status  = send(connfd,xdr_buf,MAX_LINE,0);
          printf("%s\n", "send the message");
          
       }
       else if(strcmp(pt[1].ProcName,proname)==0)
       {
          z=pre_add(buff);

          //printf("final_rpc_result:%lf\n", z);
      //send(connfd,rpc_result,MAX_LINE,0);
     

      //XDR xdrs;
          char rpc_result_buf[120];
      
          char xdr_buf[120];
          xdrmem_create(&xdrs,xdr_buf,120,XDR_ENCODE);
          
          xdr_double(&xdrs,&z);
          int status  = send(connfd,xdr_buf,MAX_LINE,0);
          printf("%s\n", "send the message");
       }
       else
       {
         char str_tmp[]="there is no registed function";
         //printf("%s\n", str_tmp);
       }
    } 

  return 0;
}