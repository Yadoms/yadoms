#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>

namespace shared
{
   //--------------------------------------------------------------
   /// \brief	Base class for threads
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CHttpMethods
   {
   public:
      static CDataContainer SendGetRequest(const std::string& url);
   };
} // namespace shared
