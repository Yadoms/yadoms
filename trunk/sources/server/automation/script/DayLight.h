#pragma once
#include "IDayLight.h"
#include "ILocation.h"

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The day light helper
   //-----------------------------------------------------
   class CDayLight : public IDayLight
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] location  The current location
      //-----------------------------------------------------
      CDayLight(boost::shared_ptr<ILocation> location);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CDayLight();

   protected:
      // IDayLight Implementation
      virtual std::string sunriseTime() const;
      virtual std::string sunsetTime() const;
      // [END] IDayLight Implementation

      //-----------------------------------------------------
      ///\brief               Convert time to Iso representation
      ///\param[in] utcTime   UTC Time (in hours)
      ///\return Time ISO representation
      //-----------------------------------------------------
      std::string toIso(double utcTime) const;

      //-----------------------------------------------------
      ///\brief               Sun event computation adapter
      ///\param[in] sunrise   true to compute sunrise, false to compute sunset
      ///\return sunrise/sunset hour in the day
      //-----------------------------------------------------
      std::string sunEventTime(bool sunrise) const;

   private:
      //-----------------------------------------------------
      ///\brief               Location helper
      //-----------------------------------------------------
      boost::shared_ptr<ILocation> m_location;
   };

} } // namespace automation::script
	
	