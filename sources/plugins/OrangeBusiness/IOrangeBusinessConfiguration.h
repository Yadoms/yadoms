#pragma once
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class IOrangeBusinessConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IOrangeBusinessConfiguration()
   {
   }

   //--------------------------------------------------------------
   /// \brief	    Get the API Key from the configuration
   /// \return     The API Key String
   //--------------------------------------------------------------
   virtual std::string getAPIKey() const = 0;

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer& data) = 0;
};