#pragma once
#include <shared/plugin/information/IYadomsInformation.h>
#include <plugin_IPC/yadomsToPlugin.pb.h>

namespace plugin_cpp_api
{
   class CYadomsInformation :public shared::plugin::information::IYadomsInformation
   {
   public:
      //--------------------------------------------------------------
      /// \brief	      Constructor
      /// \param[in]    buffer : Protobuf buffer
      //--------------------------------------------------------------
      explicit CYadomsInformation(const plugin_IPC::toPlugin::YadomsInformationAnswer& buffer);

      virtual ~CYadomsInformation();

      // shared::plugin::information::IYadomsInformation implementation
      bool developperMode() const override;
      shared::versioning::CVersion version() const override;
      boost::shared_ptr<const shared::ILocation> location() const override;
      // [END] shared::plugin::information::IYadomsInformation implementation

   private:
      const plugin_IPC::toPlugin::YadomsInformationAnswer m_buffer;
   };
} // namespace plugin_cpp_api	


