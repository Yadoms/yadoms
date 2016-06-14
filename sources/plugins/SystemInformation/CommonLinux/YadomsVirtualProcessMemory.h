#pragma once
#include "../ILoad.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "../specificHistorizers/KByte.h"


// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Virtual Memory of Yadoms
/// \note   return the virtual memory used by Yadoms
//--------------------------------------------------------------
class CYadomsVirtualProcessMemory : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] keywordName The keyword name
   //--------------------------------------------------------------
   explicit CYadomsVirtualProcessMemory(const std::string& keywordName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CYadomsVirtualProcessMemory();

   // ILoad Implementation
   void read() override;

   boost::shared_ptr<const yApi::historization::IHistorizable> historizable() const override
   {
      return m_keyword;
   }

   // [END] ILoad Implementation
   
protected:
   int parseLine(char* line) const;

private:
   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CKByte> m_keyword;
};

