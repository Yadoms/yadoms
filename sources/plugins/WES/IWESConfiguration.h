#pragma once
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class IWESConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IWESConfiguration()
   {}

   //--------------------------------------------------------------
   /// \brief      Load configuration data
   /// \param [in] data The new WES configuration
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer& data) = 0;

   //--------------------------------------------------------------
   /// \brief      return true, if we would like to retrieve all names from the WES equipment
   /// \return     true if we retrieve names
   //--------------------------------------------------------------
   virtual bool isRetrieveNamesFromdevice() const = 0;
};