#pragma once
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class IConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IConfiguration() {}

   //--------------------------------------------------------------
   /// \brief		 Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const boost::shared_ptr<shared::CDataContainer> &data) = 0;

   //--------------------------------------------------------------
   /// \brief	    Check if OWFs mode is selected
   /// \return     true if OWFs mode selected
   //--------------------------------------------------------------
   virtual bool isOwfsMode() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Check if Kernel mode is selected
   /// \return     true if Kernel mode selected
   //--------------------------------------------------------------
   virtual bool isKernelMode() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the OWFS mount point
   //--------------------------------------------------------------
   virtual const boost::filesystem::path& getOwfsMountPoint() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the kernel mode mount point
   //--------------------------------------------------------------
   virtual const boost::filesystem::path& getKernelMountPoint() const = 0;
};
