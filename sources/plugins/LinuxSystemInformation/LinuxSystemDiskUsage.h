#pragma once

#include "ILoad.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Disk Usage for a drive for the Linux System
/// \note   return the disk usage of a disk for the Linux Operating System
//--------------------------------------------------------------
class CLinuxSystemDiskUsage : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] device    The device
   /// \param[in] device    The drive name ex: C:
   /// \param[in] keywordName The keyword name
   //--------------------------------------------------------------
   CLinuxSystemDiskUsage(const std::string & deviceId, const std::string & driveName, const std::string & Keyword);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CLinuxSystemDiskUsage();

   // ILoad Implementation
   virtual void declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context);
   virtual void read();
   virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const;
   // [END] ILoad Implementation

private:

   //--------------------------------------------------------------
   /// \brief	    Device ID
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

   //FIXME : Ecriture à modifier ??
   std::vector<std::string> ExecuteCommandAndReturn(const std::string &szCommand);
};

