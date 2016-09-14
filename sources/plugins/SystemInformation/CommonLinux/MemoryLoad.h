#pragma once

#include "../ILoad.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Memory Load of the Linux System
/// \note   return the memory load for Linux Operating System
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

   void ReadFromFile(unsigned long long *dmemTotal,
                     unsigned long long *dmemFree,
                     unsigned long long *dbuffer,
                     unsigned long long *dcached);

private:

   //--------------------------------------------------------------
   /// \brief	tab indexes used for needed values
   //--------------------------------------------------------------
   enum EEnumType
   {
      kmemTotal = 0,
      kmemFree  = 1,
      kbuffer   = 2,
      kcached   = 3
   };

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CLoad> m_keyword;
};