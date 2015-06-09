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
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer &data) = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the OWFS mount point
   //--------------------------------------------------------------
   virtual const boost::filesystem::path& getOwfsMountPoint () const = 0;
};