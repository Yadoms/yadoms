#include "stdafx.h"
#if defined(MAC)   // TODO : vérifier la condition, j'ai mis au pif (JMD... ;-) )

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
   throw CNotImplementedException();
}

const boost::shared_ptr<std::vector<std::string> > CPeripherals::getUnusedSerialPorts()
{
   //TODO Voir http://stackoverflow.com/questions/8632558/macos-programmatically-finding-serial-ports
   // Utiliser "throw CNotSupportedException();" si une de ces fonctions ne peut être supportée par la plateforme
   throw CNotImplementedException();
}

const boost::shared_ptr<std::vector<std::string> > CPeripherals::getUsedSerialPorts()
{
   //TODO Voir http://stackoverflow.com/questions/8632558/macos-programmatically-finding-serial-ports
   // Utiliser "throw CNotSupportedException();" si une de ces fonctions ne peut être supportée par la plateforme
   throw CNotImplementedException();
}

#endif