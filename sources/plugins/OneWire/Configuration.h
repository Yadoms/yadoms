#pragma once

#include "IConfiguration.h"
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CConfiguration : public IConfiguration
{
public:

   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CConfiguration();

   // IConfiguration implementation
   virtual void initializeWith(const shared::CDataContainer &data);
   virtual bool isOwfsMode() const;
   virtual bool isKernelMode() const;
   virtual const boost::filesystem::path& getOwfsMountPoint() const;
   virtual const boost::filesystem::path& getKernelMountPoint() const;
   // [END] IConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    OWFS mode
   //--------------------------------------------------------------
   bool m_isOwfsMode;

   //--------------------------------------------------------------
   /// \brief	    Kernel mode
   //--------------------------------------------------------------
   bool m_isKernelMode;

   //--------------------------------------------------------------
   /// \brief	    OWFS mount point
   //--------------------------------------------------------------
   boost::filesystem::path m_owfsMountPoint;

   //--------------------------------------------------------------
   /// \brief	    Kernel mount point
   //--------------------------------------------------------------
   boost::filesystem::path m_kernelMountPoint;
};
