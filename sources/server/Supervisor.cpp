#include "Supervisor.h"
#include <boost/log/trivial.hpp>


CSupervisor::CSupervisor(void)
	:CThreadBase("Supervisor")
{
}


CSupervisor::~CSupervisor(void)
{
}

void CSupervisor::doWork()
{
	BOOST_LOG_TRIVIAL(info) << "Message from Supervisor ";
}
