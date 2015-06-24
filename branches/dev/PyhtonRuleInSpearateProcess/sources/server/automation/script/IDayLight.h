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
      ///\return Sunrise hour (ISO 8601)
      //-----------------------------------------------------
      virtual std::string sunriseTime() const = 0;

      //-----------------------------------------------------
      ///\brief               Compute sunset time
      ///\return Sunset hour (ISO 8601)
      //-----------------------------------------------------
      virtual std::string sunsetTime() const = 0;
   };

} } // namespace automation::script

