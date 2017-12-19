// This code comes from Paul Schlyter, thanks to him
// See http://stjarnhimlen.se/comp/sunriset.c

#include "stdafx.h"
#include "DayLightProvider.h"
#include <shared/currentTime/Provider.h>
#include <shared/exception/InvalidParameter.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

namespace automation
{
   namespace script
   {
      /* A macro to compute the number of days elapsed since 2000 Jan 0.0 */
      /* (which is equal to 1999 Dec 31, 0h UT)                           */

      #define days_since_2000_Jan_0(y,m,d) \
         (367L*(y)-((7*((y)+(((m)+9)/12)))/4)+((275*(m))/9)+(d)-730530L)

      /* Some conversion factors between radians and degrees */

      #ifndef PI
      #define PI        boost::math::constants::pi<double>()
      #endif

      #define RADEG     ( 180.0 / PI )
      #define DEGRAD    ( PI / 180.0 )

      /* The trigonometric functions in degrees */

      #define sind(x)  sin((x)*DEGRAD)
      #define cosd(x)  cos((x)*DEGRAD)
      #define tand(x)  tan((x)*DEGRAD)

      #define atand(x)    (RADEG*atan(x))
      #define asind(x)    (RADEG*asin(x))
      #define acosd(x)    (RADEG*acos(x))
      #define atan2d(y,x) (RADEG*atan2(y,x))


      /* This macro computes times for sunrise/sunset.                      */
      /* Sunrise/set is considered to occur when the Sun's upper limb is    */
      /* 35 arc minutes below the horizon (this accounts for the refraction */
      /* of the Earth's atmosphere).                                        */
      #define sun_rise_set(year,month,day,lon,lat,rise,set)  \
         __sunriset__( year, month, day, lon, lat, -35.0/60.0, 1, rise, set )

      /* This macro computes the start and end times of civil twilight.       */
      /* Civil twilight starts/ends when the Sun's center is 6 degrees below  */
      /* the horizon.                                                         */
      #define civil_twilight(year,month,day,lon,lat,start,end)  \
         __sunriset__( year, month, day, lon, lat, -6.0, 0, start, end )

      /* This macro computes the start and end times of nautical twilight.    */
      /* Nautical twilight starts/ends when the Sun's center is 12 degrees    */
      /* below the horizon.                                                   */
      #define nautical_twilight(year,month,day,lon,lat,start,end)  \
         __sunriset__( year, month, day, lon, lat, -12.0, 0, start, end )

      /* This macro computes the start and end times of astronomical twilight.   */
      /* Astronomical twilight starts/ends when the Sun's center is 18 degrees   */
      /* below the horizon.                                                      */
      #define astronomical_twilight(year,month,day,lon,lat,start,end)  \
         __sunriset__( year, month, day, lon, lat, -18.0, 0, start, end )


      /* Function prototypes */

      int __sunriset__(int year, int month, int day, double lon, double lat,
                       double altit, int upper_limb, double* rise, double* set);

      void sunpos(double d, double* lon, double* r);

      void sun_RA_dec(double d, double* RA, double* dec, double* r);

      double revolution(double x);

      double rev180(double x);

      double GMST0(double d);

      /* The "workhorse" function for sun rise/set times */

      int __sunriset__(int year, int month, int day, double lon, double lat,
                       double altit, int upper_limb, double* trise, double* tset)
      /***************************************************************************/
      /* Note: year,month,date = calendar date, 1801-2099 only.             */
      /*       Eastern longitude positive, Western longitude negative       */
      /*       Northern latitude positive, Southern latitude negative       */
      /*       The longitude value IS critical in this function!            */
      /*       altit = the altitude which the Sun should cross              */
      /*               Set to -35/60 degrees for rise/set, -6 degrees       */
      /*               for civil, -12 degrees for nautical and -18          */
      /*               degrees for astronomical twilight.                   */
      /*         upper_limb: non-zero -> upper limb, zero -> center         */
      /*               Set to non-zero (e.g. 1) when computing rise/set     */
      /*               times, and to zero when computing start/end of       */
      /*               twilight.                                            */
      /*        *rise = where to store the rise time                        */
      /*        *set  = where to store the set  time                        */
      /*                Both times are relative to the specified altitude,  */
      /*                and thus this function can be used to compute       */
      /*                various twilight times, as well as rise/set times   */
      /* Return value:  0 = sun rises/sets this day, times stored at        */
      /*                    *trise and *tset.                               */
      /*               +1 = sun above the specified "horizon" 24 hours.     */
      /*                    *trise set to time when the sun is at south,    */
      /*                    minus 12 hours while *tset is set to the south  */
      /*                    time plus 12 hours. "Day" length = 24 hours     */
      /*               -1 = sun is below the specified "horizon" 24 hours   */
      /*                    "Day" length = 0 hours, *trise and *tset are    */
      /*                    both set to the time when the sun is at south.  */
      /*                                                                    */
      /**********************************************************************/
      {
         double d, /* Days since 2000 Jan 0.0 (negative before) */
                sr, /* Solar distance, astronomical units */
                sRA, /* Sun's Right Ascension */
                sdec, /* Sun's declination */
                sradius, /* Sun's apparent radius */
                t, /* Diurnal arc */
                tsouth, /* Time when Sun is at south */
                sidtime; /* Local sidereal time */

         int rc = 0; /* Return cde from function - usually 0 */

         /* Compute d of 12h local mean solar time */
         d = days_since_2000_Jan_0(year,month,day) + 0.5 - lon / 360.0;

         /* Compute the local sidereal time of this moment */
         sidtime = revolution(GMST0(d) + 180.0 + lon);

         /* Compute Sun's RA, Decl and distance at this moment */
         sun_RA_dec(d, &sRA, &sdec, &sr);

         /* Compute time when Sun is at south - in hours UT */
         tsouth = 12.0 - rev180(sidtime - sRA) / 15.0;

         /* Compute the Sun's apparent radius in degrees */
         sradius = 0.2666 / sr;

         /* Do correction to upper limb, if necessary */
         if (upper_limb)
            altit -= sradius;

         /* Compute the diurnal arc that the Sun traverses to reach */
         /* the specified altitude altit: */
         {
            double cost;
            cost = (sind(altit) - sind(lat) * sind(sdec)) /
               (cosd(lat) * cosd(sdec));
            if (cost >= 1.0)
               rc = -1 , t = 0.0; /* Sun always below altit */
            else if (cost <= -1.0)
               rc = +1 , t = 12.0; /* Sun always above altit */
            else
               t = acosd(cost) / 15.0; /* The diurnal arc, hours */
         }

         /* Store rise and set times - in hours UT */
         *trise = tsouth - t;
         *tset = tsouth + t;

         return rc;
      } /* __sunriset__ */


      /* This function computes the Sun's position at any instant */

      void sunpos(double d, double* lon, double* r)
      /******************************************************/
      /* Computes the Sun's ecliptic longitude and distance */
      /* at an instant given in d, number of days since     */
      /* 2000 Jan 0.0.  The Sun's ecliptic latitude is not  */
      /* computed, since it's always very near 0.           */
      /******************************************************/
      {
         double M, /* Mean anomaly of the Sun */
                w, /* Mean longitude of perihelion */
                /* Note: Sun's mean longitude = M + w */
                e, /* Eccentricity of Earth's orbit */
                E, /* Eccentric anomaly */
                x, y, /* x, y coordinates in orbit */
                v; /* True anomaly */

         /* Compute mean elements */
         M = revolution(356.0470 + 0.9856002585 * d);
         w = 282.9404 + 4.70935E-5 * d;
         e = 0.016709 - 1.151E-9 * d;

         /* Compute true longitude and radius vector */
         E = M + e * RADEG * sind(M) * (1.0 + e * cosd(M));
         x = cosd(E) - e;
         y = sqrt(1.0 - e * e) * sind(E);
         *r = sqrt(x * x + y * y); /* Solar distance */
         v = atan2d( y, x ); /* True anomaly */
         *lon = v + w; /* True solar longitude */
         if (*lon >= 360.0)
            *lon -= 360.0; /* Make it 0..360 degrees */
      }

      void sun_RA_dec(double d, double* RA, double* dec, double* r)
      /******************************************************/
      /* Computes the Sun's equatorial coordinates RA, Decl */
      /* and also its distance, at an instant given in d,   */
      /* the number of days since 2000 Jan 0.0.             */
      /******************************************************/
      {
         double lon, obl_ecl, x, y, z;

         /* Compute Sun's ecliptical coordinates */
         sunpos(d, &lon, r);

         /* Compute ecliptic rectangular coordinates (z=0) */
         x = *r * cosd(lon);
         y = *r * sind(lon);

         /* Compute obliquity of ecliptic (inclination of Earth's axis) */
         obl_ecl = 23.4393 - 3.563E-7 * d;

         /* Convert to equatorial rectangular coordinates - x is unchanged */
         z = y * sind(obl_ecl);
         y = y * cosd(obl_ecl);

         /* Convert to spherical coordinates */
         *RA = atan2d( y, x );
         *dec = atan2d( z, sqrt(x*x + y*y) );
      } /* sun_RA_dec */


      /******************************************************************/
      /* This function reduces any angle to within the first revolution */
      /* by subtracting or adding even multiples of 360.0 until the     */
      /* result is >= 0.0 and < 360.0                                   */
      /******************************************************************/

      #define INV360    ( 1.0 / 360.0 )

      double revolution(double x)
      /*****************************************/
      /* Reduce angle to within 0..360 degrees */
      /*****************************************/
      {
         return (x - 360.0 * floor(x * INV360));
      } /* revolution */

      double rev180(double x)
      /*********************************************/
      /* Reduce angle to within +180..+180 degrees */
      /*********************************************/
      {
         return (x - 360.0 * floor(x * INV360 + 0.5));
      } /* revolution */


      /*******************************************************************/
      /* This function computes GMST0, the Greenwich Mean Sidereal Time  */
      /* at 0h UT (i.e. the sidereal time at the Greenwhich meridian at  */
      /* 0h UT).  GMST is then the sidereal time at Greenwich at any     */
      /* time of the day.  I've generalized GMST0 as well, and define it */
      /* as:  GMST0 = GMST - UT  --  this allows GMST0 to be computed at */
      /* other times than 0h UT as well.  While this sounds somewhat     */
      /* contradictory, it is very practical:  instead of computing      */
      /* GMST like:                                                      */
      /*                                                                 */
      /*  GMST = (GMST0) + UT * (366.2422/365.2422)                      */
      /*                                                                 */
      /* where (GMST0) is the GMST last time UT was 0 hours, one simply  */
      /* computes:                                                       */
      /*                                                                 */
      /*  GMST = GMST0 + UT                                              */
      /*                                                                 */
      /* where GMST0 is the GMST "at 0h UT" but at the current moment!   */
      /* Defined in this way, GMST0 will increase with about 4 min a     */
      /* day.  It also happens that GMST0 (in degrees, 1 hr = 15 degr)   */
      /* is equal to the Sun's mean longitude plus/minus 180 degrees!    */
      /* (if we neglect aberration, which amounts to 20 seconds of arc   */
      /* or 1.33 seconds of time)                                        */
      /*                                                                 */
      /*******************************************************************/

      double GMST0(double d)
      {
         double sidtim0;
         /* Sidtime at 0h UT = L (Sun's mean longitude) + 180.0 degr  */
         /* L = M + w, as defined in sunpos().  Since I'm too lazy to */
         /* add these numbers, I'll let the C compiler do it for me.  */
         /* Any decent C compiler will add the constants at compile   */
         /* time, imposing no runtime or code overhead.               */
         sidtim0 = revolution((180.0 + 356.0470 + 282.9404) +
            (0.9856002585 + 4.70935E-5) * d);
         return sidtim0;
      } /* GMST0 */


      CDayLightProvider::CDayLightProvider(boost::shared_ptr<shared::ILocation> location)
         : m_location(location)
      {
      }

      CDayLightProvider::~CDayLightProvider()
      {
      }

      boost::posix_time::ptime CDayLightProvider::sunEventTime(bool sunrise)
      {
         auto now = shared::currentTime::Provider().now();
         if (m_lastCalculationDate.date() != now.date())
         {
            m_lastCalculationDate = now;

            // Compute sun events
            double rise, set;
            if (sun_rise_set(now.date().year(), now.date().month(), now.date().day(), m_location->longitude(), m_location->latitude(), &rise, &set) != 0)
               throw shared::exception::CInvalidParameter("Unable to compute sunrise time");

            m_rise = hoursToLocalTime(now.date(), rise);
            m_set = hoursToLocalTime(now.date(), set);
         }

         return sunrise ? m_rise : m_set;
      }

      boost::posix_time::ptime CDayLightProvider::hoursToLocalTime(const boost::gregorian::date& date,
                                                                   double hours)
      {
         // Use the Os settings to adjust to local time
         typedef boost::date_time::c_local_adjustor<boost::posix_time::ptime> local_adj;
         return local_adj::utc_to_local(boost::posix_time::ptime(date,
                                                                 boost::posix_time::hours(static_cast<long>(floor(hours))) + boost::posix_time::minutes(lround(hours * 60) % 60)));
      }


      boost::posix_time::ptime CDayLightProvider::sunriseTime()
      {
         return sunEventTime(true);
      }

      boost::posix_time::ptime CDayLightProvider::sunsetTime()
      {
         return sunEventTime(false);
      }

      std::string CDayLightProvider::formatSunEventTime(const boost::posix_time::ptime& sunEventTime)
      {
         std::stringstream stream;
         auto facet(new boost::posix_time::time_facet());
         facet->format("%H:%M");
         stream.imbue(std::locale(std::locale::classic(), facet));
         stream << sunEventTime;
         return stream.str();
      }
   }
} // namespace automation::script
