#include "stdafx.h"

#include "ConfigurationFactory.h"
#include "../../Serialization/PTreeToJsonSerializer.h"


boost::shared_ptr<IPtreeToStringSerializer> CConfigurationFactory::createSerializer()
{
   boost::shared_ptr<IPtreeToStringSerializer> serializer(new PtreeToJsonSerializer);
   return serializer;
}
