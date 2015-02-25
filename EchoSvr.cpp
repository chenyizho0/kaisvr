/*************************************************************************
    > File Name: EchoSvr.cpp
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2015年02月25日 星期三 06时36分56秒
 ************************************************************************/

#include "TcpServer.h"

int main()
{
	TcpServer * echosvr = new TcpServer(12345);
	int iRet = echosvr->init();
	if (iRet != 0)
	{
		printf("Echosvr init error\n");
		return 0;
	}
	echosvr->loop();
	delete echosvr;
}
