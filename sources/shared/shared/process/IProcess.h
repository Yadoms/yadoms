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
         /// \brief	Ask for process stop
         /// \throw CPythonException if error
         //--------------------------------------------------------------
         virtual void kill() = 0;

         //--------------------------------------------------------------
         /// \brief	Get the process return code
         /// \return Return code
         //--------------------------------------------------------------
         virtual int getReturnCode() const = 0;

         //--------------------------------------------------------------
         /// \brief	Get the process error
         /// \return Error (empty if none)
         //--------------------------------------------------------------
         virtual std::string getError() const = 0;
      };
   }
} // namespace shared::process

