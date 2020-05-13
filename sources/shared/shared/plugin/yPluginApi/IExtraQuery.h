#pragma once
#include "IExtraQueryData.h"
#include <shared/DataContainer.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //-----------------------------------------------------
         ///\brief The API used for the commands on plugin
         //-----------------------------------------------------
         class IExtraQuery
         {
         public:
            //-----------------------------------------------------
            ///\brief               Destructor
            //-----------------------------------------------------
            virtual ~IExtraQuery()
            {
            }

            //-----------------------------------------------------
            ///\brief               Get the extra query data
            ///\return              The extra query data
            //-----------------------------------------------------
            virtual boost::shared_ptr<IExtraQueryData> getData() const = 0;

            //-----------------------------------------------------
            ///\brief Answer the request with success, providing query answer data
            ///\param [in] data      The answer data
            //-----------------------------------------------------
            virtual void sendSuccess(const CDataContainerSharedPtr& data) = 0;

            //-----------------------------------------------------
            ///\brief Answer the request with error
            ///\param [in] errorMessage  The error message
            //-----------------------------------------------------
            virtual void sendError(const std::string& errorMessage) = 0;

            //-----------------------------------------------------
            ///\brief Report the extra query progression
            ///\param [in] progression  The progression (0->100)
            ///\param [in] message The associated message (can be i18n string)
            //-----------------------------------------------------
            virtual void reportProgress(const float progression, const std::string& message) = 0;
         };
      }
   }
} // namespace shared::plugin::yPluginApi	


