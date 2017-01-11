#pragma once

namespace shared
{
   //-----------------------------------------------------
   ///\brief The location interface
   //-----------------------------------------------------
   class ILocation
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~ILocation()
      {
      }

      //-----------------------------------------------------
      ///\brief               Get latitude
      ///\return latitude as string
      //-----------------------------------------------------
      virtual double latitude() const = 0;

      //-----------------------------------------------------
      ///\brief               Get longitude
      ///\return longitude as string
      //-----------------------------------------------------
      virtual double longitude() const = 0;

      //-----------------------------------------------------
      ///\brief               Get altitude
      ///\return altitude as string
      //-----------------------------------------------------
      virtual double altitude() const = 0;
   };
} // namespace shared