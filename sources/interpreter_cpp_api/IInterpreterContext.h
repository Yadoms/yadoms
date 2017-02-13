#pragma once

namespace interpreter_cpp_api
{
   //--------------------------------------------------------------
   /// \class The interpreter context
   //--------------------------------------------------------------
   class IInterpreterContext
   {
   public:
      enum EProcessReturnCode
      {
         kOk = 0,
         kStartError,         // Interpreter failed to start
         kRuntimeError,       // Interpreter crashed at runtime
         kUnexpectedStop      // Interpreter has stopped without stop requested
      };

      virtual ~IInterpreterContext() { }

      //--------------------------------------------------------------
      /// \brief                 The main run method, where the interpreter code is executed
      //--------------------------------------------------------------
      virtual void run() = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the return code, after run() returned
      /// \return                The return code
      //--------------------------------------------------------------
      virtual EProcessReturnCode getReturnCode() const = 0;
   };

} // namespace interpreter_cpp_api
