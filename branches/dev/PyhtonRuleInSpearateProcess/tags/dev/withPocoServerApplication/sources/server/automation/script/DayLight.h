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
      virtual double sunriseTime() const;
      virtual double sunsetTime() const;
      // [END] IDayLight Implementation

      //-----------------------------------------------------
      ///\brief               Convert utcTime to localTime
      ///\param[in] utcTime   UTC Time (in hours)
      ///\return local time (in hours)
      //-----------------------------------------------------
      double toLocalTime(double utcTime) const;

      //-----------------------------------------------------
      ///\brief               Sun event computation adapter
      ///\param[in] sunrise   true to compute sunrise, false to compute sunset
      ///\return sunrise/sunset hour in the day
      //-----------------------------------------------------
      double sunEventTime(bool sunrise) const;

   private:
      //-----------------------------------------------------
      ///\brief               Location helper
      //-----------------------------------------------------
      boost::shared_ptr<ILocation> m_location;
   };

} } // namespace automation::script
	
	