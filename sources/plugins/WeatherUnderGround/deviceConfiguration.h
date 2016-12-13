#pragma once

#include "IdeviceConfiguration.h"
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Configuration of the device
//--------------------------------------------------------------
class CdeviceConfiguration : public IdeviceConfiguration
{
public:

   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   explicit CdeviceConfiguration(const shared::CDataContainer& data);

   explicit CdeviceConfiguration(CdeviceConfiguration& newConfiguration);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CdeviceConfiguration();

   // IWUConfiguration implementation
   void initializeWith(const shared::CDataContainer& data) override;
   bool isConditionsIndividualKeywordsEnabled() const override;
   bool isLiveConditionsEnabled() const override;
   bool isRainIndividualKeywordsEnabled() const override;
   bool isAstronomyEnabled() const override;
   bool isForecast10DaysEnabled() const override;
   // [END] IWUConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};