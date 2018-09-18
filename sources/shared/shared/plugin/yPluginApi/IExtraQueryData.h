#pragma once

#include <shared/DataContainer.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //-----------------------------------------------------
         ///\brief The API used for extra query data
         //-----------------------------------------------------
         class IExtraQueryData
         {
         public:
            //-----------------------------------------------------
            ///\brief               Destructor
            //-----------------------------------------------------
            virtual ~IExtraQueryData()
            {
            }

            //-----------------------------------------------------
            ///\brief               Get the query
            ///\return              The query
            //-----------------------------------------------------
            virtual const std::string& query() const = 0;

            //-----------------------------------------------------
            ///\brief               Get the query data
            ///\return              The query data
            //-----------------------------------------------------
            virtual const CDataContainer& data() const = 0;
         };
      }
   }
} // namespace shared::plugin::yPluginApi	


