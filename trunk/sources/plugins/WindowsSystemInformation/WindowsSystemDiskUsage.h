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
   /// \param[in] device      The device
   /// \param[in] keywordName The keyword name
   /// \param[in] driveName   The drive name ex: C:\
   //--------------------------------------------------------------
   CWindowsSystemDiskUsage(const std::string & device, const std::string & keywordName, const std::string & driveName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWindowsSystemDiskUsage();

   // ILoad Implementation
   virtual void declareKeywords(boost::shared_ptr<yApi::IYadomsApi> context);
   virtual void read();
   virtual void historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const;
   // [END] ILoad Implementation

private:

   //--------------------------------------------------------------
   /// \brief	    Device
   //--------------------------------------------------------------
   const std::string m_device;

   //--------------------------------------------------------------
   /// \brief	    Disk Name
   //--------------------------------------------------------------
   const std::string m_driveName;

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   yApi::historization::CLoad m_keyword;
};

