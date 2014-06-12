#pragma once
#include "IDeviceList.h"
#include <shared/plugin/yadomsApi/IYadomsApi.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	List of devices implementation
//--------------------------------------------------------------
class CDeviceList : public IDeviceList
{  
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   CDeviceList(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CDeviceList();

   // IDeviceList Implementation
   virtual void declareDevice(const std::string& device, const std::string& model, const std::string& parameters);
   virtual const std::string& getDeviceParameters(const std::string& device);
   // [END] IDeviceList Implementation


private:
   //--------------------------------------------------------------
   /// \brief	YadomsApi context
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::IYadomsApi> m_context;

   //--------------------------------------------------------------
   /// \brief	The device list. the key is the device name, the value is the device parameters
   //--------------------------------------------------------------
   typedef std::map<std::string, std::string> DeviceList;
   DeviceList m_devices;
};



