#include "stdafx.h"

#include "Factory.h"
#include "../../serialization/PTreeToJsonSerializer.h"


namespace shared { namespace plugin { namespace configuration
{

boost::shared_ptr<serialization::IPtreeToStringSerializer> CFactory::createSerializer()
{
   boost::shared_ptr<serialization::IPtreeToStringSerializer> serializer(new serialization::CPtreeToJsonSerializer);
   return serializer;
}

} } } // namespace shared::plugin::configuration
