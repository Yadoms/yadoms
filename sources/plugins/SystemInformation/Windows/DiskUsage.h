#pragma once

#include "../ILoad.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Disk Usage for a drive for the Windows System
/// \note   return the disk usage of a disk for the Windows Operating System
//--------------------------------------------------------------
class CDiskUsage : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] device      The device
   /// \param[in] keywordName The keyword name
   /// \param[in] driveName   The drive name ex: C:\
   //--------------------------------------------------------------
   CDiskUsage(const std::string & device, const std::string & keywordName, const std::string & driveName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CDiskUsage();

   // ILoad Implementation
   void declareKeywords(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details) override;
   void read() override;
   void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const override;
   boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const override;
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
   boost::shared_ptr<yApi::historization::CLoad> m_keyword;
};

