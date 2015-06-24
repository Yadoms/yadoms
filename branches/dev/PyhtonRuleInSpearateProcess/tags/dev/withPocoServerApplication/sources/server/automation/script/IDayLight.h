#pragma once

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The day light interface
   //-----------------------------------------------------
   class IDayLight
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IDayLight() {}

      //-----------------------------------------------------
      ///\brief               Compute sunrise time
      ///\return Sunrise hour
      //-----------------------------------------------------
      virtual double sunriseTime() const = 0;

      //-----------------------------------------------------
      ///\brief               Compute sunset time
      ///\return Sunset hour
      //-----------------------------------------------------
      virtual double sunsetTime() const = 0;
   };

} } // namespace automation::script

