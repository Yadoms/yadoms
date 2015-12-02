#pragma once

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The time adapter for script API
   ///\detail This class helps to convert a time expressed as literal.
   /// This string can represent a duration (if begins by a 'f' like "for")
   /// or a time point (if begins by a 'a' like "at")
   //-----------------------------------------------------
   class CTimeAdapter
   {
   public:
      //-----------------------------------------------------
      ///\brief Constructor
      ///\param[in] timeout : "f10:00" = for 10 hours (duration), "a10:00" = at 10 o'clock (time point)
      //-----------------------------------------------------
      CTimeAdapter(const std::string& timeout);
      virtual ~CTimeAdapter();

      bool isDateTime() const;
      boost::posix_time::time_duration duration() const;
      boost::posix_time::ptime dateTime() const;

   private:
      void convert(const std::string& timeout);
   
      bool m_isDateTime;
      boost::posix_time::time_duration m_duration;
      boost::posix_time::ptime m_dateTime;
   };

} } // namespace automation::script
	
	