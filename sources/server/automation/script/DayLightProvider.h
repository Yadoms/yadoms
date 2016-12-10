#pragma once
#include "IDayLightProvider.h"
#include <shared/ILocation.h>

namespace automation
{
   namespace script
   {
      //-----------------------------------------------------
      ///\brief The day light helper
      //-----------------------------------------------------
      class CDayLightProvider : public IDayLightProvider
      {
      public:
         //-----------------------------------------------------
         ///\brief               Constructor
         ///\param[in] location  The current location
         //-----------------------------------------------------
         explicit CDayLightProvider(boost::shared_ptr<shared::ILocation> location);

         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~CDayLightProvider();

         //-----------------------------------------------------
         ///\brief               Format a sun event time as "HH:MM"
         ///\param[in] sunEventTime The sun event time to format
         ///\return Formated time
         //-----------------------------------------------------
         static std::string formatSunEventTime(const boost::posix_time::ptime& sunEventTime);

      protected:
         // IDayLightProvider Implementation
         boost::posix_time::ptime sunriseTime() override;
         boost::posix_time::ptime sunsetTime() override;
         // [END] IDayLightProvider Implementation

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
         boost::shared_ptr<shared::ILocation> m_location;

         //-----------------------------------------------------
         ///\brief               Last calculation date (to compute only once a day)
         //-----------------------------------------------------
         boost::posix_time::ptime m_lastCalculationDate;

         //-----------------------------------------------------
         ///\brief               Pre-calculed sun event times
         //-----------------------------------------------------
         boost::posix_time::ptime m_rise, m_set;
      };
   }
} // namespace automation::script


