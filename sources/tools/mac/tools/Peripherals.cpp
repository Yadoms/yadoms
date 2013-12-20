#include "stdafx.h"

#include "tools/Exceptions/NotImplementedException.hpp"
#include "tools/Log.h"
#include "Peripherals.h"


CPeripherals::CPeripherals()
{
}

CPeripherals::~CPeripherals()
{
}

const boost::shared_ptr<std::vector<std::string> > CPeripherals::getSerialPorts()
{
   //TODO Voir http://stackoverflow.com/questions/8632558/macos-programmatically-finding-serial-ports
   NOT_IMPLEMENTED;
}

const boost::shared_ptr<std::vector<std::string> > CPeripherals::getUnusedSerialPorts()
{
   //TODO Voir http://stackoverflow.com/questions/8632558/macos-programmatically-finding-serial-ports
   // Utiliser "throw CNotSupportedException();" si une de ces fonctions ne peut �tre support�e par la plateforme
   NOT_IMPLEMENTED;
}

const boost::shared_ptr<std::vector<std::string> > CPeripherals::getUsedSerialPorts()
{
   //TODO Voir http://stackoverflow.com/questions/8632558/macos-programmatically-finding-serial-ports
   // Utiliser "throw CNotSupportedException();" si une de ces fonctions ne peut �tre support�e par la plateforme
   NOT_IMPLEMENTED;
}
