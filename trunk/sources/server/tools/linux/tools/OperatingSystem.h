#pragma once

namespace tools {

   class COperatingSystem
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                  Get OS name
      /// \return                   OS name
      //--------------------------------------------------------------
      static const std::string & getName();
      
      //--------------------------------------------------------------
      /// \brief	                  Shutdown the system
      /// \param[in] andRestart     Shutdown and restart if true
      /// \return                   true if success
      //--------------------------------------------------------------
      static bool shutdown(bool andRestart = false);//TODO faire pour linux

   private:
      static const std::string m_name;
   };

} //namespace tools