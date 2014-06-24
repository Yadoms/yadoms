#pragma once

#include "ILoad.h"
#include <shared/plugin/yadomsApi/IYadomsApi.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	Disk Usage for a drive for the Windows System
/// \note   return the disk usage of a disk for the Windows Operating System
//--------------------------------------------------------------
class CWindowsSystemDiskUsage : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   /// \param[in] deviceId    The drive name ex: C:\
   //--------------------------------------------------------------
   CWindowsSystemDiskUsage(const std::string & deviceId, const std::string & driveName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWindowsSystemDiskUsage();

   //--------------------------------------------------------------
   /// \brief	    Returns the sensor device ID
   /// \return     Device ID
   //--------------------------------------------------------------
   virtual const std::string& getDeviceId() const;

   //--------------------------------------------------------------
   /// \brief	            Declare the device and its associated keywords
   /// \param[in] context  YadomsApi context to which declare the device
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief	            Send all sensor data to Yadoms
   /// \param[in] context  YadomsApi context to which historize data
   //--------------------------------------------------------------
   void historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const;

   //--------------------------------------------------------------
   /// \brief	    Returns read (computed) Disk usage
   /// \return     Disk Usage in %
   //--------------------------------------------------------------
   virtual double getValue();

private:

   //--------------------------------------------------------------
   /// \brief	    Device ID
   //--------------------------------------------------------------
   const std::string m_deviceId;

   //--------------------------------------------------------------
   /// \brief	    Disk Name
   //--------------------------------------------------------------
   const std::string m_driveName;

   //--------------------------------------------------------------
   /// \brief	    Value of the Disk Usage in %
   //--------------------------------------------------------------
   double m_diskUsage;
};

