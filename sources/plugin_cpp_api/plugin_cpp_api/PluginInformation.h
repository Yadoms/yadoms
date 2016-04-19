#pragma once

#include <shared/plugin/information/IInformation.h>
#include <pluginInformation.pb.h>

class CPluginInformation :public shared::plugin::information::IInformation
{
public:
   //--------------------------------------------------------------
   /// \brief	      Constructor
   /// \param[in]    buffer : Protobuf buffer
   //--------------------------------------------------------------
   explicit CPluginInformation(boost::shared_ptr<const pbPluginInformation::Information> buffer);

   virtual ~CPluginInformation();

   // shared::plugin::IInformation implementation
   const std::string& getType() const override;
   const std::string& getVersion() const override;
   shared::versioning::EReleaseType getReleaseType() const override;
   const std::string& getAuthor() const override;
   const std::string& getUrl() const override;
   std::string getIdentity() const override;
   std::string toString() const override;
   bool isSupportedOnThisPlatform() const override;
   bool getSupportManuallyCreatedDevice() const override;
   shared::CDataContainer getPackage() const override;
   const boost::filesystem::path& getPath() const override;
   // [END] shared::plugin::IInformation implementation

private:
   boost::shared_ptr<const pbPluginInformation::Information> m_buffer;
};
