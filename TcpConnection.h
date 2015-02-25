/*************************************************************************
    > File Name: TcpConnection.h
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2015年02月25日 星期三 06时28分46秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
using namespace std;

class TcpConnection
{
public:
	TcpConnection(int fd)
		:fd_(fd)
	{
	}
	~TcpConnection()
	{
		close(fd_);
	}
	int Send(const string msg);
private:
	int fd_;
};

int TcpConnection::Send(const string msg)
{
	return send(fd_,msg.c_str(),msg.size(),0);
}
