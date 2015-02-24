/*************************************************************************
    > File Name: EventLoop.cpp
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2015年02月24日 星期二 05时10分39秒
 ************************************************************************/

#include<iostream>
using namespace std;

class EventLoop
{
public:
	EventLoop(int port,int svr_fd)
		:port_(port),svr_fd_(svr_fd)
	{
	}
	int loop()
	{
		fd_set all_set_,rset_;
		FD_ZERO(&all_set_);
		FD_SET(svr_fd_,&all_set_);
		int maxfd = svr_fd_;
		int nready = -1;
		while(true)
		{

			nready = seclect(maxfd+1,&rset,NULL,NULL,NULL);
			if (nready < 0)
			{
				printf("select error\n");
				break;
			}
			if (FD_ISSET(svr_fd_,&rset))
			{
				int connfd = accept()
			}
		}
	}
private:
	int port_;
	int svr_fd;

};
