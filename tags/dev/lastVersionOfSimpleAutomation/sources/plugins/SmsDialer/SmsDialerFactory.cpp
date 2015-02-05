#include "stdafx.h"
#include "SmsDialerFactory.h"
#include "GammuPhone.h"

CSmsDialerFactory::~CSmsDialerFactory()
{
}


boost::shared_ptr<IPhone> CSmsDialerFactory::constructPhone(const ISmsDialerConfiguration& configuration)
{
   boost::shared_ptr<IPhone> phone(new CGammuPhone(configuration));
   return phone;
}