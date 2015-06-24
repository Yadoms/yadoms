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
   //--------------------------------------------------------------
   CKeywordContainer();
   
   //--------------------------------------------------------------
   /// \brief	      Constructor
   /// \param [in]   deviceId   The device id
   /// \param [in]   keyword    The keyword
   //--------------------------------------------------------------
   CKeywordContainer(const std::string & deviceId, const yApi::historization::IHistorizable & keyword)
      :m_deviceId(deviceId), m_keyword(keyword)
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
   const yApi::historization::IHistorizable & getKeyword() const { return m_keyword; } 
   
   //--------------------------------------------------------------
   /// \brief	      Get the device id
   /// \return       The device id
   //--------------------------------------------------------------   
   const std::string & getDeviceId() const { return m_deviceId; }
   
   
private:
   //--------------------------------------------------------------
   /// \brief	      The device id
   //--------------------------------------------------------------      
   std::string	m_deviceId;

   //--------------------------------------------------------------
   /// \brief	      The keyword
   //--------------------------------------------------------------      
   const yApi::historization::IHistorizable &	m_keyword;
};





