#pragma once

//--------------------------------------------------------------
/// \brief	Python executable
//--------------------------------------------------------------
class IScriptProcess
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IScriptProcess() {}

   //--------------------------------------------------------------
   /// \brief	Ask for module stop
   /// \throw CPythonException if error
   //--------------------------------------------------------------
   virtual void interrupt() = 0;

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



