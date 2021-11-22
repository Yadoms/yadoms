#pragma once
#include <plugin_IPC/yadomsToPlugin.pb.h>
#include <shared/plugin/yPluginApi/IExtraQuery.h>


namespace plugin_cpp_api
{
   //-----------------------------------------------------
   ///\brief The IExtraQuery implementation
   //-----------------------------------------------------
   class CExtraQuery final : public shared::plugin::yPluginApi::IExtraQuery
   {
   public:
      explicit CExtraQuery(const plugin_IPC::toPlugin::ExtraQuery& msg,
                           boost::function1<void, const boost::shared_ptr<shared::CDataContainer>&> sucessCallback,
                           boost::function1<void, const std::string&> errorCallback,
                           boost::function2<void, float, const std::string&> progressionCallback);
      ~CExtraQuery() override = default;

      // IExtraQuery implementation
      boost::shared_ptr<shared::plugin::yPluginApi::IExtraQueryData> getData() const override;
      void sendSuccess(const boost::shared_ptr<shared::CDataContainer>& result) override;
      void sendError(const std::string& errorMessage) override;
      void reportProgress(float progression,
                          const std::string& message) override;
      // [END] IExtraQuery implementation

   private:
      boost::shared_ptr<shared::plugin::yPluginApi::IExtraQueryData> m_data;
      boost::function1<void, const boost::shared_ptr<shared::CDataContainer>&> m_sucessCallback;
      boost::function1<void, const std::string&> m_errorCallback;
      boost::function2<void, float, const std::string&> m_progressionCallback;
   };
} // namespace plugin_cpp_api	
