#pragma once

#include "../ILoad.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>


// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Memory Load of the Windows System
/// \note   return the memory load for Windows Operating System
//--------------------------------------------------------------
class CMemoryLoad : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] device    The device
   //--------------------------------------------------------------
   explicit CMemoryLoad(const std::string & device);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CMemoryLoad();

   // ILoad Implementation
   void declareKeywords(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details) override;
   void read() override;
   void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const override;
   boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const override;
   // [END] ILoad Implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Device name
   //--------------------------------------------------------------
   const std::string m_device;

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CLoad> m_keyword;
};

