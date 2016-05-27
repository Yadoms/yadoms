#pragma once
//TODO virer
namespace shared
{
   namespace process
   {
      //-----------------------------------------------------------------------------
      /// \class              Application stop request
      //-----------------------------------------------------------------------------
      class IApplicationStopRequestObserver
      {
      public:
         //--------------------------------------------------------------
         /// \brief	                        Destructor
         //--------------------------------------------------------------
         virtual ~IApplicationStopRequestObserver()
         {
         }

         //-----------------------------------------------------------------------------
         /// \brief		                     Called when application stop was requested
         //-----------------------------------------------------------------------------
         virtual void onApplicationStopRequested() = 0;
      };
   }
} // namespace shared::process


