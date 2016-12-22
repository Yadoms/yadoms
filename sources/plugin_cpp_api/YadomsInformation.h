#pragma once
#include <shared/plugin/information/IYadomsInformation.h>
#include <toPlugin.pb.h>

namespace plugin_cpp_api
{
   class CYadomsInformation :public shared::plugin::information::IYadomsInformation
   {
   public:
      //--------------------------------------------------------------
      /// \brief	      Constructor
      /// \param[in]    buffer : Protobuf buffer
      //--------------------------------------------------------------
      explicit CYadomsInformation(const toPlugin::YadomsInformationAnswer& buffer);

      virtual ~CYadomsInformation();

      // shared::plugin::information::IYadomsInformation implementation
      bool developperMode() const override;
      shared::versioning::CVersion version() const override;
      boost::shared_ptr<const shared::ILocation> location() const override;
      // [END] shared::plugin::information::IYadomsInformation implementation

   private:
      const toPlugin::YadomsInformationAnswer m_buffer;
   };
} // namespace plugin_cpp_api	


