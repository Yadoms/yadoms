#pragma once

#include <shared/plugin/yPluginApi/historization/IHistorizable.h>

//--------------------------------------------------------------
/// \brief     Class used to encapsulate a keyword
///            -> deviceId + iHistorizable
//--------------------------------------------------------------
class CKeywordContainer
{
public:

   //--------------------------------------------------------------
   /// \brief	      Constructor
   /// \param [in]   deviceId   The device id
   /// \param [in]   keyword    The keyword
   //--------------------------------------------------------------
   CKeywordContainer(const std::string& deviceId,
                     boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> keyword,
                     bool isConfigurationKeyword)
      : m_deviceId(deviceId),
        m_keyword(keyword),
        m_isConfigurationKeyword(isConfigurationKeyword)
   {
   }

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------   
   virtual ~CKeywordContainer()
   {
   }

   //--------------------------------------------------------------
   /// \brief	      Get the keyword
   /// \return       The keyword
   //--------------------------------------------------------------   
   boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> getKeyword() const
   {
      return m_keyword;
   }

   //--------------------------------------------------------------
   /// \brief	      Get the device id
   /// \return       The device id
   //--------------------------------------------------------------   
   const std::string& getDeviceId() const
   {
      return m_deviceId;
   }

   //--------------------------------------------------------------
   /// \brief	      Tells if keyword is configuration item
   /// \return       True if keyword is configuration item
   //--------------------------------------------------------------   
   bool isConfigurationKeyword() const
   {
      return m_isConfigurationKeyword;
   }


private:
   //--------------------------------------------------------------
   /// \brief	      The device id
   //--------------------------------------------------------------      
   std::string m_deviceId;

   //--------------------------------------------------------------
   /// \brief	      The keyword
   //--------------------------------------------------------------      
   boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> m_keyword;

   //--------------------------------------------------------------
   /// \brief	      The device details
   //--------------------------------------------------------------      
   bool m_isConfigurationKeyword;
};

