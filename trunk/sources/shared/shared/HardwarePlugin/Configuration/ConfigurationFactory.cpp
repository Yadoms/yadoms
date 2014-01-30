#include "stdafx.h"

#include "ConfigurationFactory.h"
#include "JsonSerializer.h"


boost::shared_ptr<IConfigurationSerializer> CConfigurationFactory::createSerializer()
{
   boost::shared_ptr<IConfigurationSerializer> serializer(new CJsonSerializer);
   return serializer;
}
