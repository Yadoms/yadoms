#pragma once

#include "../../Serialization/IPtreeToStringSerializer.h"


namespace shared { namespace plugin { namespace configuration
{

   //--------------------------------------------------------------
   /// \class Hardware plugin configuration factory
   //--------------------------------------------------------------
   class CFactory
   {
      // This class can only be used by the configuration
      friend class CConfiguration;

   private:
      //--------------------------------------------------------------
      /// \brief	   Serializer creation
      /// \return    A new serializer instance
      //--------------------------------------------------------------
      static boost::shared_ptr<IPtreeToStringSerializer> createSerializer();
   };

} } } // namespace shared::plugin::configuration
