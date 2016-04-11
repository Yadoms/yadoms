#pragma once

namespace shared
{
   namespace process
   {
      //--------------------------------------------------------------
      /// \brief	End of process listener interface
      //--------------------------------------------------------------
      class IEndOfProcessObserver
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~IEndOfProcessObserver()
         {
         }

         //--------------------------------------------------------------
         /// \brief	               Called when the process exists
         /// \param[in] returnCode  The process return code
         /// \param[in] error       The error output content (can be empty)
         //--------------------------------------------------------------
         virtual void onEndOfProcess(int returnCode, const std::string& error) = 0;
      };
   }
} // namespace shared::process

