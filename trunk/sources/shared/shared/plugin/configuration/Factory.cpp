#include "stdafx.h"

#include "Factory.h"
#include "../../Serialization/PTreeToJsonSerializer.h"


namespace shared { namespace plugin { namespace configuration
{

boost::shared_ptr<IPtreeToStringSerializer> CFactory::createSerializer()
{
   boost::shared_ptr<IPtreeToStringSerializer> serializer(new CPtreeToJsonSerializer);
   return serializer;
}

} } } // namespace shared::plugin::configuration
