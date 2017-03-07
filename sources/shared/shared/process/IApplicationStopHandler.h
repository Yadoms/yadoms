#pragma once
#include <shared/Export.h>

namespace shared
{
   namespace process
   {
      //-----------------------------------------------------------------------------
      /// \class              Application stop request
      //-----------------------------------------------------------------------------
      class YADOMS_SHARED_EXPORT IApplicationStopHandler
      {
      public:
         //--------------------------------------------------------------
         /// \brief	                        Destructor
         //--------------------------------------------------------------
         virtual ~IApplicationStopHandler()
         {
         }

         //-----------------------------------------------------------------------------
         /// \brief		                     Called when application stop was requested
         /// \param[in] onStopRequestedFct   Function called when application stop is requested
         ///                                 This function must return only when application is fully stopped
         ///                                 This function must return true if application was gracefully stop, else false
         ///                                 To ignore stop requests (like CTRL-C), set this argument to boost::function<bool()>()
         //-----------------------------------------------------------------------------
         virtual void setApplicationStopHandler(boost::function<bool()> onStopRequestedFct) = 0;
      };
   }
} // namespace shared::process


