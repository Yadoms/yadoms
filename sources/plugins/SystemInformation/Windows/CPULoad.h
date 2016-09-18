#pragma once

#include "../ILoad.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <windows.h>

//--------------------------------------------------------------
/// \brief	CPU Load of the Windows System
/// \note   return the CPU load under Windows Operating System
//--------------------------------------------------------------
class CCPULoad : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	   Constructor
   /// \param[in] keywordName The keyword name
   //--------------------------------------------------------------
   explicit CCPULoad(const std::string& keywordName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CCPULoad();

   // ILoad Implementation
   void read() override;

   boost::shared_ptr<const yApi::historization::IHistorizable> historizable() const override
   {
      return m_keyword;
   }

   // [END] ILoad Implementation

protected:

   float getCpuUsage( void );
   void readSystemValues(ULARGE_INTEGER *sysIdle, ULARGE_INTEGER *sysKernel, ULARGE_INTEGER *sysUser);

private:

   //--------------------------------------------------------------
   /// \brief	    Old references
   //--------------------------------------------------------------
   ULARGE_INTEGER  ul_sys_idle_old;
   ULARGE_INTEGER  ul_sys_kernel_old;
   ULARGE_INTEGER  ul_sys_user_old;

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CLoad> m_keyword;
};