#pragma once

#include "../ILoad.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "../specificHistorizers/KByte.h"


// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Physical Memory used in RAM
/// \note   return the Physical memory used in RAM
//--------------------------------------------------------------
class CYadomsRAMProcessMemory : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] device    The device
   //--------------------------------------------------------------
   explicit CYadomsRAMProcessMemory(const std::string & device);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CYadomsRAMProcessMemory();

   // ILoad Implementation
   virtual void declareKeywords(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details);
   virtual void read();
   virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const;
   virtual boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const;
   // [END] ILoad Implementation

private:

   int parseLine(char* line);

   //--------------------------------------------------------------
   /// \brief	    Device name
   //--------------------------------------------------------------
   const std::string m_device;

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CKByte> m_keyword;
};

