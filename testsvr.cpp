/*************************************************************************
    > File Name: testsvr.cpp
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2015年02月23日 星期一 20时16分46秒
 ************************************************************************/

//多个客户端与服务端的通讯
 
////1 可循环发送信息  2 服务器把接受到的信息转发回给客户端。//
////3 可利用c4_display作为显示窗
///4 可接受多个客户端的连接
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "Poller.h"
#define MAXCON 100
int main()
{
 	struct sockaddr_in addrSrv,addrClt; 
 	char buf[1024],buf2[1024]; 
 	
 	int socketSrv=socket(AF_INET,SOCK_STREAM,0);   
 	//初始化
 	addrSrv.sin_family=AF_INET;
 	addrSrv.sin_port=htons(12345);
 	addrSrv.sin_addr.s_addr=htonl(INADDR_ANY); 
 	memset(addrSrv.sin_zero,0,8); 
 	//绑定服务器端的地址和ID
 	bind(socketSrv,(struct sockaddr*)&addrSrv,sizeof(addrSrv));
 	
 	listen(socketSrv,MAXCON);
 	
 	int len=sizeof(addrClt); 
 	
 	printf("listening....\n");
 	

	vector<int> init_fds;
	init_fds.push_back(socketSrv);
	Poller poller_;
	int iRet = poller_.init(init_fds);
	if (iRet != 0)
	{
		printf("poller init error\n");
		return 0;
	}
	vector<int> active_fds;
	while(1)
 	{
		vector<int> tmpfds = poller_.all_fds();
		for (size_t i = 0;i < tmpfds.size();i++)
		{
			printf("%d ",tmpfds[i]);
		}
		printf("\n");
		int nready = poller_.poller(active_fds);
 	 	if (nready < 0)   //nready代表有几个文件描述已经准备好
		{
			printf("poller error\n");
			break;
		}
		printf("nready = %d\n",nready);
		for (size_t i = 0;i < active_fds.size();i++)
		{
			if (active_fds[i] == socketSrv)
			{
				int  connfd= accept(socketSrv,(struct sockaddr*)&addrClt,(socklen_t*)(&len));
				poller_.add_fd(connfd);
				printf("accept new connection fd = %d\n",connfd);
			}
			else
			{
				memset(buf,0,1024);   
 	 	 	 	memset(buf2,0,1024);  
 	 	 	 	int n = recv(active_fds[i],buf,1024,0);    
 	 	 	 	if ((n == 0)||(strcmp(buf,"end")==0))   //n=0代表正常关闭
 	 	 	 	{  
					poller_.remove_fd(active_fds[i]);
					close(active_fds[i]);     
					printf("close connection fd = %d\n",active_fds[i]);
 	 	 	 	}
 	 	 	 	else
 	 	 	 	{ 
					printf("service receive:%s\n",buf);    
					sprintf(buf2,"server dispatcmsg:%s",buf);    
					vector<int> all_fds = poller_.all_fds();
					for (size_t j = 0;j < all_fds.size();j++)
					{    
						if (all_fds[j] != socketSrv && all_fds[j]!=active_fds[i])
						{
							if(send(all_fds[j],buf2,strlen(buf2)+1,0)<0) perror("send");  //群发
								printf("send to client %d\n",i);
						}
					}  
 	 	 	 	}
 	 	 	}
 	 	}
 	}
	close(socketSrv); 
	return 0;
}




