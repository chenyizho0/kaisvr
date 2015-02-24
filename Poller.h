/*************************************************************************
    > File Name: Poller.h
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2015年02月24日 星期二 08时24分53秒
 ************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
using namespace std;
class Poller
{
public:
	Poller()
		:max_fd_num_(-1)
	{
	}
	int init(vector<int> init_fds);
	int poller(vector<int> &fds);
	int add_fd(int fd);
	int remove_fd(int fd);
	vector<int> all_fds()
	{
		return all_fds_;
	}
private:
	fd_set allset_;
	int max_fd_num_;
	vector<int> all_fds_;
};

int Poller::init(vector<int> init_fds)
{
	all_fds_ = init_fds;
	FD_ZERO(&allset_);	
	for (size_t i = 0;i < init_fds.size();i++)
	{
		if (init_fds[i] > 0)
			FD_SET(init_fds[i],&allset_);
		if (init_fds[i] > max_fd_num_)
		{
			max_fd_num_ = init_fds[i];
		}
	}
}

int Poller::poller(vector<int> &fds)
{
	fds.clear();
	fd_set rset_ = allset_;
	int nready =  select(max_fd_num_+1,&rset_,NULL,NULL,NULL);
	for (size_t i = 0;i < all_fds_.size();i++)
	{
		if (FD_ISSET(all_fds_[i],&rset_))
		{
			fds.push_back(all_fds_[i]);
		}
	}
	return nready;
}

int Poller::add_fd(int fd)
{
	if (FD_ISSET(fd,&allset_))
	{
		return -1;
	}
	all_fds_.push_back(fd);
	FD_SET(fd,&allset_);
	return 0;
}

int Poller::remove_fd(int fd)
{
	if (!FD_ISSET(fd,&allset_))
	{
		return -1;
	}
	for (int i = 0;i < all_fds_.size();i++)
	{
		if (fd == all_fds_[i])
		{
			all_fds_.erase(all_fds_.begin()+i);
			break;
		}
	}
	FD_CLR(fd,&allset_);
	return 0;
}
