#pragma once

namespace ioInterfaces {

   //--------------------------------------------------------------
   /// \brief	Single switch io accessor interface
   //--------------------------------------------------------------
   class ISingleSwitch
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~ISingleSwitch() {}

      //--------------------------------------------------------------
      /// \brief	Read value
      /// \return Switch state
      //--------------------------------------------------------------
      virtual bool read() const = 0;

      //--------------------------------------------------------------
      /// \brief	Write value
      /// \param[in] state Switch state
      //--------------------------------------------------------------
      virtual void write(bool state) const = 0;
   };

} // namespace ioInterfaces