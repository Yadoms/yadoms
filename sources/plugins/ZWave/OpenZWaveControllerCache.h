#pragma once

#include "KeywordContainer.h"
#include <shared/Datacontainer.h>
#include <shared/plugin/yPluginApi/historization/DeviceState.h>

//--------------------------------------------------------------
/// \brief	    OpenZWaveController cache
///             The aim is to cache every data until all network has been discovered
///             This allow a better device/keyword declaration (don't matter of OZW notification order) and when all node are queried, all information are known
//--------------------------------------------------------------
class COpenZWaveControllerCache
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveControllerCache();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COpenZWaveControllerCache();

   //--------------------------------------------------------------
   /// \brief	    Add a kewyrod value to cache
   /// \param [in] homeId  The home ID
   /// \param [in] nodeId  The node ID
   /// \param [in] info    The keyword container
   //--------------------------------------------------------------
   void AddKeywordValue(const std::string & device, boost::shared_ptr<CKeywordContainer> & info);

   //--------------------------------------------------------------
   /// \brief	    Add a device info
   /// \param [in] homeId  The home ID
   /// \param [in] nodeId  The node ID
   /// \param [in] info    The device info container
   //--------------------------------------------------------------
   void AddDeviceInfo(const std::string & device, shared::CDataContainer & info);
   void UpdateDeviceState(const std::string & device, shared::plugin::yPluginApi::historization::EDeviceState & state);

public:
   typedef std::vector<boost::shared_ptr<CKeywordContainer>> KeywordList;
   typedef std::map<std::string, KeywordList > KeywordCache;
   typedef std::pair<shared::CDataContainer, shared::plugin::yPluginApi::historization::EDeviceState> DeviceInfoAndState;
   typedef std::map<std::string, DeviceInfoAndState > DeviceCache;
   

   DeviceCache & getDeviceCache();

   DeviceInfoAndState & getDeviceInfo(const std::string & deviceName);

   KeywordList & getKeywordsForDevice(const std::string & deviceName);

   void clearKeywordsForDevice(const std::string & deviceName);
private:
   //--------------------------------------------------------------
   /// \brief	    The keyword cache
   //--------------------------------------------------------------
   KeywordCache m_keywordCache;

   //--------------------------------------------------------------
   /// \brief	    The device cache
   //--------------------------------------------------------------
   DeviceCache m_deviceCache;
};

