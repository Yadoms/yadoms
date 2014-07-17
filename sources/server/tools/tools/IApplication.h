#pragma once

namespace tools
{
   //-----------------------------
   ///\brief Interface for application
   //-----------------------------
   class IApplication
   {
   public:
      //-----------------------------
      ///\brief Run the application (blocking)
      ///\param [in ] argc    The number of arguments
      ///\param [in ] argv    The arguments
      ///\return The application return code
      //-----------------------------   
      virtual int run(int argc, char ** argv) = 0;

      //-----------------------------
      ///\brief Stop the application
      ///\param [in ] callbackAfterStopped    The callback the application should call as last operation
      //-----------------------------
      virtual void stop(boost::function<void()> & callbackAfterStopped) = 0;
   };

} // namespace tools
