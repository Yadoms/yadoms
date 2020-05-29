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
   void initializeWith(const boost::shared_ptr<shared::CDataContainer> &data) override;
   bool isOwfsMode() const override;
   bool isKernelMode() const override;
   const boost::filesystem::path& getOwfsMountPoint() const override;
   const boost::filesystem::path& getKernelMountPoint() const override;
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
