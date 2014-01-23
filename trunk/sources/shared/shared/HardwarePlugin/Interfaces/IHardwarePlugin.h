#pragma once


//--------------------------------------------------------------
/// \class General hardware plugin interface
//--------------------------------------------------------------
class IHardwarePlugin
{
public:
   virtual ~IHardwarePlugin() {}

   //--------------------------------------------------------------
   /// \brief              Main plugin function
   /// \par configurationValues   Plugin instance configuration values from database (Json)
   /// \note               Do the work. This function runs in specific-thread context (yadoms main-thread independent).
   //                      Implementation should loop until end is asked by yadoms.
   //                      Be careful to put some sleeps to prevent using too much system ressources.
   //                      Use boost::this_thread::sleep is a watch stop point (a point where thread is able to stop).
   //--------------------------------------------------------------
   virtual void doWork(const std::string& configurationValues) = 0;

   //--------------------------------------------------------------
   /// \brief              Notify the plugin that its configuration was changed
   /// \par configurationValues   Plugin instance configuration values from database (Json)
   /// \note               Note that this function is asynchronous. It's the plugin
   ///                     responsibility to manage this notification thread-safetely
   //--------------------------------------------------------------
   virtual void updateConfiguration(const std::string& configurationValues) = 0;

};

