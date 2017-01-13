#pragma once

namespace automation
{
   //-----------------------------------------------------
   ///\brief The rule logger interface
   //-----------------------------------------------------
   class IRuleLogger//TODO virer ?
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IRuleLogger()
      {
      }

      //-----------------------------------------------------
      ///\brief               Log a line
      ///\param[in] line      Line to log
      //-----------------------------------------------------
      virtual void information(const std::string& line) = 0;
      virtual void error(const std::string& line) = 0;
   };
} // namespace automation


