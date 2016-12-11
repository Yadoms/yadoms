#pragma once
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class IdeviceConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IdeviceConfiguration()
   {
   }

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer& data) = 0;

   //--------------------------------------------------------------
   /// \brief	    Get options enabled from the configuration
   //--------------------------------------------------------------
   virtual bool isLiveConditionsEnabled(void) const = 0;
   virtual bool isConditionsIndividualKeywordsEnabled(void) const = 0;
   virtual bool isRainIndividualKeywordsEnabled(void) const = 0;
   virtual bool isAstronomyEnabled(void) const = 0;
   virtual bool isForecast10DaysEnabled(void) const = 0;
};