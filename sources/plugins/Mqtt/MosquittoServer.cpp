#include "stdafx.h"
#include "MosquittoServer.h"
#include "server/mosquitto-main.h"

CMosquittoServer::CMosquittoServer()
   : CThreadBase("MosquittoServer")
{
}

CMosquittoServer::~CMosquittoServer()
{
}

void CMosquittoServer::doWork()
{
   mosquitto_start(0, NULL);
}

void CMosquittoServer::stop()
{
   mosquitto_stop();
}
