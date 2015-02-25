/*************************************************************************
    > File Name: EventLoop.h
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2015年02月24日 星期二 07时47分44秒
 ************************************************************************/
#include <vector>
#include "TcpConnection.h"
#include "Poller.h"
#include <map>
using namespace std;

class EventLoop
{
public:
	EventLoop(int fd)
		:local_server_fd_(fd)
	{

	}
	int loop();
private:
	int local_server_fd_;
	Poller * poller_;
	map<int,TcpConnection *> Fd_Conn_Map;
	int onMessage_(TcpConnection * conn,string msg);
};

int EventLoop::loop()	
{
	vector<int> init_fds;
	init_fds.push_back(local_server_fd_);
	poller_ = new Poller();
	vector<int> active_fds;
	if (poller_->init(init_fds) != 0)
	{
		printf("poller init error\n");
		return -1;
	}
 	struct sockaddr_in addrCli; 
	int cli_len = sizeof(addrCli);
	while(true)
	{
		int nready = poller_->poller(active_fds);
		if (nready < 0)
		{
			printf("poller error\n");
			break;
		}
		for (size_t i = 0;i < active_fds.size();i++)
		{
			if (active_fds[i] == local_server_fd_)
			{
				int  new_conn_fd= accept(local_server_fd_,(struct sockaddr*)&addrCli,(socklen_t*)(&cli_len));
				poller_->add_fd(new_conn_fd);
				TcpConnection * tcpconnection = new TcpConnection(new_conn_fd);
				Fd_Conn_Map[new_conn_fd] = tcpconnection; 
			}
			else
			{
				char buf[1024];
				memset(buf,0,1024);
				int n = recv(active_fds[i],buf,1024,0);
				if (n < 0)
				{
					printf("recv error fd = %d\n",active_fds[i]);
					continue;
				}
				if (n == 0)
				{
					poller_->remove_fd(active_fds[i]);
					TcpConnection * tcpconnection = Fd_Conn_Map[active_fds[i]];
					delete tcpconnection;
				}
				else
				{
					onMessage_(Fd_Conn_Map[active_fds[i]],string(buf));
				}
			}
		}
	}
}

int EventLoop::onMessage_(TcpConnection * conn,string msg)
{
	sleep(10);
	conn->Send(msg);
}
