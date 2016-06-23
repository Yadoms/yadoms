#pragma once

namespace ioInterfaces
{
   //--------------------------------------------------------------
   /// \brief	Multi counters io accessor interface
   //--------------------------------------------------------------
   class IMultiCounter
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IMultiCounter()
      {
      }

      //--------------------------------------------------------------
      /// \brief	Read value
      /// \param[in] unit Unit number (0..n)
      /// \return Counter value
      //--------------------------------------------------------------
      virtual int read(unsigned char unit) const = 0;
   };

} // namespace ioInterfaces