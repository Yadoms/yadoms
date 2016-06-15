#pragma once
#include "../IUnsecuredProtocolFilter.h"

//--------------------------------------------------------------
/// \brief	The unsecured protocols filter
//--------------------------------------------------------------
class CCurrentEnergyFilter : public IUnsecuredProtocolFilter
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CCurrentEnergyFilter()
   {
   }

   // IUnsecuredProtocolFilter Implementation
   bool isValid(const std::string& deviceName) override;
   // [END] IUnsecuredProtocolFilter Implementation

private:
   std::vector<std::pair<boost::posix_time::ptime, std::string> > m_recentlySeenDevices;
};

