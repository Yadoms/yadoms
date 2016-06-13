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
   /// \param[in] keywordName The keyword name
   //--------------------------------------------------------------
   explicit CMemoryLoad(const std::string& keywordName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CMemoryLoad();

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
   boost::shared_ptr<yApi::historization::CLoad> m_keyword;
};

