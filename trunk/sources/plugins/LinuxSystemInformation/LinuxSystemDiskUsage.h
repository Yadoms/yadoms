#pragma once

#include "ILoad.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	Disk Usage for a drive for the Linux System
/// \note   return the disk usage of a disk for the Linux Operating System
//--------------------------------------------------------------
class CLinuxSystemDiskUsage : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   /// \param[in] deviceId    The drive name ex: C:
   //--------------------------------------------------------------
   CLinuxSystemDiskUsage(const std::string & deviceId, const std::string & driveName, const std::string & Keyword);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CLinuxSystemDiskUsage();

   //--------------------------------------------------------------
   /// \brief	    Returns the sensor device ID
   /// \return     Device ID
   //--------------------------------------------------------------
   const std::string& getDeviceId() const;

   const std::string& getDriveName() const;

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

   //--------------------------------------------------------------
   /// \brief	    Returns the Capacity String
   /// \return     Device ID
   //--------------------------------------------------------------
   const std::string& getCapacity() const;

   //--------------------------------------------------------------
   /// \brief	    Returns the Keyword String
   /// \return     Device ID
   //--------------------------------------------------------------
   const std::string& getKeyword() const;

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

   //--------------------------------------------------------------
   /// \brief	    Capacity string
   //--------------------------------------------------------------
   const std::string m_Capacity;

   //--------------------------------------------------------------
   /// \brief	    Keyword string
   //--------------------------------------------------------------
   const std::string m_Keyword;

   //FIXME : Ecriture à modifier ??
   std::vector<std::string> ExecuteCommandAndReturn(const std::string &szCommand);
};

