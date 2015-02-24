/*************************************************************************
    > File Name: EventLoop.h
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2015年02月24日 星期二 07时47分44秒
 ************************************************************************/

#include<iostream>
using namespace std;
class Poller;
class TcpConnection;
class EventLoop
{
public:
	EventLoop(int fd,int * onConnection(),int * onMessage())
		:local_server_fd_(fd),onConnection_(onConnection),onMessage_(onMessage)
	{

	}
	int loop();
private:
	int local_server_fd_;
	int * onConnection_();
	int * onMessage_(TcpConnection *,string msg);
	Poller poller_;
	map<int,TcpConnection *> Fd_Conn_Map;
};

int EventLoop::loop()	
{
	vector<int> all_fds,active_fds;
	all_fds.push_back(local_server_fd_);
	poller_ = Poller(all_fds);
	struct sockaddr_in addrCli; 
	int len = sizeof(addrCli);
	int iRet = 0;
	while(true)
	{
		iRet = poller_.poll(active_fds);
		if (iRet != 0)break;
		for(size_t i = 0;i < active_fds.size();i++)
		{
			if (FD_ISSET(active_fds[i]))
			{
				if (active_fds[i] == local_server_fd_)
				{
					int  new_conn_fd= accept(local_server_fd_,(struct sockaddr*)&addrCli,(socklen_t*)(&len));
					poller_.add_fd(new_conn_fd);
					Fd_Conn_Map[new_conn_fd] = new TcpConnection(fd);
				}
				else
				{
					int buf[1024];
					memset(buf,0,1024);
					int n = recv(active_fds[i],buf,1024,0);
					if (n == 0)
					{
						TcpConnection * tcpconn_ = Fd_Conn_Map.[active_fds[i]];
						Fd_Conn_Map.remove(active_fds[i]);
						delete tcpconn_;
						poller_.remove(active_fds[i]);
					}
					else
					{
						int iRet = onMessage_(Fd_Conn_Map[active_fds[i]],string(buf));
					}
				}
			}
		}
	}
	return -1;
}
