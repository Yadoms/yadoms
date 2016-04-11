#pragma once

namespace shared
{
   namespace process
   {
      //--------------------------------------------------------------
      /// \brief	Process manager interface
      //--------------------------------------------------------------
      class IProcess
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~IProcess()
         {
         }

         //--------------------------------------------------------------
         /// \brief	Ask for module stop
         /// \throw CPythonException if error
         //--------------------------------------------------------------
         virtual void kill() = 0;

         //--------------------------------------------------------------
         /// \brief	Wait for module stop (blocking)
         /// \return Process return code
         //--------------------------------------------------------------
         virtual int waitForStop() = 0;

         //--------------------------------------------------------------
         /// \brief	Get the process error
         /// \return Error (empty if none)
         //--------------------------------------------------------------
         virtual std::string getError() const = 0;
      };
   }
} // namespace shared::process

