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
      virtual boost::posix_time::ptime sunriseTime();
      virtual boost::posix_time::ptime sunsetTime();
      // [END] IDayLight Implementation

      //-----------------------------------------------------
      ///\brief               Sun event computation adapter
      ///\param[in] sunrise   true to compute sunrise, false to compute sunset
      ///\return sunrise/sunset hour in the day
      //-----------------------------------------------------
      boost::posix_time::ptime sunEventTime(bool sunrise);

      //-----------------------------------------------------
      ///\brief               Convert hours number to local time
      ///\param[in] date      The date
      ///\param[in] hours     Number of hours, as double
      ///\return local time composed by provided date and hour
      //-----------------------------------------------------
      static boost::posix_time::ptime hoursToLocalTime(const boost::gregorian::date& date, double hours);

   private:
      //-----------------------------------------------------
      ///\brief               Location helper
      //-----------------------------------------------------
      boost::shared_ptr<ILocation> m_location;

      //-----------------------------------------------------
      ///\brief               Last calculation date (to compute only once a day)
      //-----------------------------------------------------
      boost::posix_time::ptime m_lastCalculationDate;

      //-----------------------------------------------------
      ///\brief               Pre-calculed sun event times
      //-----------------------------------------------------
      boost::posix_time::ptime m_rise, m_set;
   };

} } // namespace automation::script
	
	