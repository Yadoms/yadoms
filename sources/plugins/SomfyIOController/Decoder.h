#pragma once

#include "IDecoder.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class implement the SomfyIOControler protocol
//--------------------------------------------------------------
class CDecoder : public IDecoder
{
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   //--------------------------------------------------------------
   explicit CDecoder(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CDecoder();

   // IDecoder implementation
   void decodeSomfyIOControlerMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                           const boost::shared_ptr<std::map<std::string, std::vector<std::string>>>& messages) override;
   // [END] IDecoder implementation

private:
   //--------------------------------------------------------------
   /// \brief	                     Create the Device with the counter Id
   //--------------------------------------------------------------
   void createDeviceAndKeywords();
   void createFirstKeywordList();
   void createRunningKeywordList();
   void processMessage(const std::string& key,
                       const std::vector<std::string>& value);

   //--------------------------------------------------------------
   /// \brief  Keywords list
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;

   //--------------------------------------------------------------
   /// \brief  Details for the device
   //--------------------------------------------------------------
   shared::CDataContainer m_DeviceDetails;

   //--------------------------------------------------------------
   /// \brief  Keywords
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CText> m_runningPeriod;
   boost::shared_ptr<yApi::historization::CCounter> m_runningIndex;

   boost::shared_ptr<yApi::IYPluginApi> m_api;
   std::string m_deviceName;
   std::string m_newPeriod;

   bool m_deviceCreated;
   int m_revision;

   static const std::string m_tag_Config;
   static const std::string m_tag_Up;
   static const std::string m_tag_Down;
   static const std::string m_tag_My;
   static const std::string m_tag_Channel;
   static const std::string m_tag_Prog;
   static const std::string m_tag_Read;
   static const std::string m_tag_Write;
   static const std::string m_tag_Status;
   static const std::string m_tag_Error;

   unsigned int m_activeChannel;
};