#pragma once
#include <plugin_IPC/yadomsToPlugin.pb.h>
#include <shared/plugin/yPluginApi/IExtraQuery.h>



namespace plugin_cpp_api
{
   //-----------------------------------------------------
   ///\brief The IExtraQuery implementation
   //-----------------------------------------------------
   class CExtraQuery : public shared::plugin::yPluginApi::IExtraQuery
   {
   public:
      explicit CExtraQuery(const plugin_IPC::toPlugin::ExtraQuery& msg,
                           boost::function1<void, const shared::CDataContainer&> sucessCallback,
                           boost::function1<void, const std::string&> errorCallback,
                           boost::function2<void, const float, const std::string&> progressionCallback);
      virtual ~CExtraQuery();

      // IExtraQuery implementation
      boost::shared_ptr<shared::plugin::yPluginApi::IExtraQueryData> getData() const override;
      void sendSuccess(const shared::CDataContainer& result) override;
      void sendError(const std::string& errorMessage) override;
      void reportProgress(const float progression, const std::string& message) override;
      // [END] IExtraQuery implementation

   private:
      boost::shared_ptr<shared::plugin::yPluginApi::IExtraQueryData> m_data;
      boost::function1<void, const shared::CDataContainer&> m_sucessCallback;
      boost::function1<void, const std::string&> m_errorCallback;
      boost::function2<void, const float, const std::string&> m_progressionCallback;
   };
} // namespace plugin_cpp_api	


