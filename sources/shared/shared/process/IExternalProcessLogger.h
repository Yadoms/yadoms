#pragma once
#include <shared/Export.h>

namespace shared
{
   namespace process
   {
      //-----------------------------------------------------
      ///\brief The process logger interface
      //-----------------------------------------------------
      class YADOMS_SHARED_EXPORT IExternalProcessLogger //TODO utile ?
      {
      public:
         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~IExternalProcessLogger()
         {
         }

         //-----------------------------------------------------
         ///\brief               Init the logger
         ///\note Must be called in the thread where log function will be called
         //-----------------------------------------------------
         virtual void init() = 0;

         //-----------------------------------------------------
         ///\brief               Log a line
         ///\param[in] line      Line to log
         //-----------------------------------------------------
         virtual void information(const std::string& line) = 0;
         virtual void error(const std::string& line) = 0;
      };
   }
} // namespace shared::process

