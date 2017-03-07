#pragma once

namespace shared
{
   namespace process
   {
      //--------------------------------------------------------------
      /// \brief	Process life observer interface
      //--------------------------------------------------------------
      class IProcessObserver
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~IProcessObserver()
         {
         }

         //--------------------------------------------------------------
         /// \brief	               Called when the process starts
         //--------------------------------------------------------------
         virtual void onStart() = 0;

         //--------------------------------------------------------------
         /// \brief	               Called when the process finish
         /// \param[in] returnCode  The process return code
         /// \param[in] error       The error output content (can be empty)
         //--------------------------------------------------------------
         virtual void onFinish(int returnCode,
                               const std::string& error) = 0;
      };
   }
} // namespace shared::process


