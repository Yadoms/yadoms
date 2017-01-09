#pragma once

#include "IWUConfiguration.h"
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CWUConfiguration : public IWUConfiguration
{
public:

   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CWUConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWUConfiguration();

   // IWUConfiguration implementation
   void initializeWith(const shared::CDataContainer& data) override;
   std::string getAPIKey() const override;
   boost::shared_ptr<const shared::ILocation> getLocation() const override;
   bool pluginLocationEnabled() const override;
   bool isConditionsIndividualKeywordsEnabled() const override;
   bool isLiveConditionsEnabled() const override;
   bool isRainIndividualKeywordsEnabled() const override;
   bool isTempIndividualKeywordsEnabled() const override;
   bool isAstronomyEnabled() const override;
   bool isForecast10DaysEnabled() const override;
   // [END] IWUConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};

