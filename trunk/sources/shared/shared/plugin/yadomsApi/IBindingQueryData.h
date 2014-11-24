#pragma once

#include <shared/DataContainer.h>

namespace shared { namespace plugin { namespace yadomsApi
{
   //-----------------------------------------------------
   ///\brief The API used for binding query
   //-----------------------------------------------------
   class IBindingQueryData
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IBindingQueryData() {}

      //-----------------------------------------------------
      ///\brief               Get the query
      ///\return              The query
      //-----------------------------------------------------
      virtual const std::string& getQuery() const = 0;
   };
   
   
} } } // namespace shared::plugin::yadomsApi	
	
	