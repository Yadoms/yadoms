#pragma once

namespace ioInterfaces {

   //--------------------------------------------------------------
   /// \brief	Single read-only data io accessor interface
   //--------------------------------------------------------------
   template<typename T>
   class ISingleReadOnlyData
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~ISingleReadOnlyData() {}

      //--------------------------------------------------------------
      /// \brief	Read value
      /// \return Voltage value (V)
      //--------------------------------------------------------------
      virtual T read() const = 0;
   };

} // namespace ioInterfaces