#pragma once
#include "communication/callback/ISynchronousCallback.h"
#include "communication/callback/ICallbackRequest.h"
#include <shared/plugin/yPluginApi/IExtraQuery.h>
#include <shared/plugin/yPluginApi/IExtraQueryData.h>


namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IExtraQuery implementation
   //-----------------------------------------------------
   class CExtraQuery : public shared::plugin::yPluginApi::IExtraQuery
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] query        The extra query
      ///\param[in] data         The query data
      ///\param [in]  callback   The callback to call when request returns
      //-----------------------------------------------------
      explicit CExtraQuery(const shared::plugin::yPluginApi::IExtraQueryData& data,
                           communication::callback::ISynchronousCallback<shared::CDataContainer>& callback);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CExtraQuery();

      // IExtraQuery implementation
      const shared::plugin::yPluginApi::IExtraQueryData& getData() const override;
      void sendSuccess(const shared::CDataContainer& data) override;
      void sendError(const std::string& errorMessage) override;
      // [END] IExtraQuery implementation

   private:
      //-----------------------------------------------------
      ///\brief Internal data
      //-----------------------------------------------------
      boost::shared_ptr<communication::callback::ICallbackRequest<shared::plugin::yPluginApi::IExtraQueryData, shared::CDataContainer> > m_requestPtr;
   };
} // namespace pluginSystem	


