#pragma once

// Include directly CPP yInterpreterApi interface
#include <shared/script/yInterpreterApi/IYInterpreterApi.h>


namespace interpreter_cpp_api
{
   //--------------------------------------------------------------
   /// \class General interpreter interface
   //--------------------------------------------------------------
   class IInterpreter
   {
   public:
      virtual ~IInterpreter() {}

      //--------------------------------------------------------------
      /// \brief              Main interpreter function
      /// \param [in] api     Interpreter execution context (Yadoms API)
      /// \note               Do the work. This function runs in specific-thread context (independent from Yadoms main-thread).
      //                      Implementation should loop until end is asked by Yadoms.
      //                      Be careful to put some sleeps to prevent using too much system resources.
      //                      Use boost::this_thread::sleep as a watch stop point (a point where thread is able to stop).
      //--------------------------------------------------------------
      virtual void doWork(boost::shared_ptr<shared::script::yInterpreterApi::IYInterpreterApi> api) = 0;
   };

} // namespace interpreter_cpp_api
