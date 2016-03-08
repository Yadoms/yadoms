#pragma once

#include "../ILoad.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "../specificHistorizers/KByte.h"


// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Virtual Memory of the current process
/// \note   return the virtual memory used by the current process
//--------------------------------------------------------------
class CVirtualProcessMemory : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] device    The device
   //--------------------------------------------------------------
   explicit CVirtualProcessMemory(const std::string & device);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CVirtualProcessMemory();

   // ILoad Implementation
   virtual void declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context);
   virtual void read();
   virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const;
   virtual boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const;
   // [END] ILoad Implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Device name
   //--------------------------------------------------------------
   const std::string m_device;

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CKByte> m_keyword;
};

