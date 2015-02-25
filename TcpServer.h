/*************************************************************************
    > File Name: TcpServer.h
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2015年02月24日 星期二 07时21分46秒
 ************************************************************************/

#include <iostream>
#include "EventLoop.h"
using namespace std;

class TcpServer
{
public:
	TcpServer(int port)
		:port_(port),local_server_fd_(-1)
	{
	}
	~TcpServer()
	{
		close(local_server_fd_);
	}
	int init();
	int loop();
private:
	int port_;
	int local_server_fd_;
	EventLoop * eventloop_;
};


int TcpServer::init()
{
 	struct sockaddr_in addrSrv; 
 	int local_server_fd_=socket(AF_INET,SOCK_STREAM,0);   
	if (local_server_fd_ < 0)return -1;
 	addrSrv.sin_family=AF_INET;
 	addrSrv.sin_port=htons(port_);
 	addrSrv.sin_addr.s_addr=htonl(INADDR_ANY); 
 	memset(addrSrv.sin_zero,0,8); 
 	int iRet = bind(local_server_fd_,(struct sockaddr*)&addrSrv,sizeof(addrSrv));
 	if (iRet != 0)return -2;
	iRet = listen(local_server_fd_,101);
	if (iRet != 0)return -3;
	eventloop_ = new EventLoop(local_server_fd_);
	return 0;
}

int TcpServer::loop()
{
	eventloop_->loop();	
}


