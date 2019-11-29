#pragma once

namespace shared
{
   //-----------------------------------------------------
   ///\brief The location interface
   //-----------------------------------------------------
   class ILocation
   {
   public:
      virtual ~ILocation() = default;

      //-----------------------------------------------------
      ///\brief               Get latitude
      ///\return latitude
      //-----------------------------------------------------
      virtual double latitude() const = 0;

      //-----------------------------------------------------
      ///\brief               Get longitude
      ///\return longitude
      //-----------------------------------------------------
      virtual double longitude() const = 0;

      //-----------------------------------------------------
      ///\brief               Get altitude
      ///\return altitude
      //-----------------------------------------------------
      virtual double altitude() const = 0;
   };
} // namespace shared