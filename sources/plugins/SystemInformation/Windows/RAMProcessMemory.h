#pragma once

#include "../ILoad.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "../specificHistorizers/KByte.h"


// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Memory of the current process in RAM
/// \note   return the memory used in RAM by the current process
//--------------------------------------------------------------
class CRAMProcessMemory : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] keywordName The keyword name
   //--------------------------------------------------------------
   explicit CRAMProcessMemory(const std::string& keywordName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CRAMProcessMemory();

   // ILoad Implementation
   void read() override;

   boost::shared_ptr<const yApi::historization::IHistorizable> historizable() const override
   {
      return m_keyword;
   }

   // [END] ILoad Implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CKByte> m_keyword;
};

