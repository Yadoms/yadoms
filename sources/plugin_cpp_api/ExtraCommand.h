#pragma once
#include <toPlugin.pb.h>
#include <shared/plugin/yPluginApi/IExtraCommand.h>


namespace plugin_cpp_api
{
   //-----------------------------------------------------
   ///\brief The IExtraCommand implementation
   //-----------------------------------------------------
   class CExtraCommand : public shared::plugin::yPluginApi::IExtraCommand
   {
   public:
      explicit CExtraCommand(const toPlugin::ExtraCommand& msg);
      virtual ~CExtraCommand();

      // IExtraCommand implementation
      const std::string& getCommand() const override;
      const shared::CDataContainer& getData() const override;
      // [END] IExtraCommand implementation

   private:
      std::string m_command;
      shared::CDataContainer m_data;
   };
} // namespace plugin_cpp_api	


