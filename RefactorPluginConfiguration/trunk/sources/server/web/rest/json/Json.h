#pragma once

#include <shared/serialization/PTreeToJsonSerializer.h>

namespace web { namespace rest { namespace json {

   //----------------------------------------
   ///\brief   Define the typename used for Json object
   //----------------------------------------
   typedef boost::property_tree::ptree CJson;

   //----------------------------------------
   ///\brief   Define the typename used for Json serializer
   //----------------------------------------
   typedef shared::serialization::CPtreeToJsonSerializer CJsonSerializer;


} //namespace json
} //namespace rest
} //namespace web 

