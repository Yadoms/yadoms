#pragma once

namespace ioInterfaces {

   //--------------------------------------------------------------
   /// \brief	Multi AD converter io accessor interface
   //--------------------------------------------------------------
   class IQuadAdConverter
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IQuadAdConverter() {}

      //--------------------------------------------------------------
      /// \brief	Read value
      /// \param[in] unit Unit number (0..n)
      /// \return AD value (V)
      //--------------------------------------------------------------
      virtual double read(unsigned char unit) const = 0;
   };

} // namespace ioInterfaces