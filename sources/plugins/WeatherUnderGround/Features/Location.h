#pragma once
#include <shared/ILocation.h>

namespace location
{
   //-----------------------------------------------------
   ///\brief The location helper
   //-----------------------------------------------------
   class CLocation : public shared::ILocation
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] longitude 
      ///\param[in] latitude 
      ///\param[in] altitude 
      /// \throw shared::exception::CEmptyResult if location is undefined
      //-----------------------------------------------------
      CLocation(double longitude,
                double latitude,
                double altitude);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CLocation();

   protected:
      // ILocation Implementation
      double latitude() const override;
      double longitude() const override;
      double altitude() const override;
      // [END] ILocation Implementation

   private:

      double m_longitude;
      double m_latitude;
      double m_altitude;
   };
} // namespace location