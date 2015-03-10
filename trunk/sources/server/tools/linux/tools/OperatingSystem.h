#pragma once

namespace tools {

   class COperatingSystem
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                  Shutdown the system
      /// \param[in] andRestart     Shutdown and restart if true
      /// \return                   true if success
      //--------------------------------------------------------------
      static bool shutdown(bool andRestart = false);//TODO faire pour linux
   };

} //namespace tools