#pragma once
#include "../../ioInterfaces/IMultiSwitch.h"

namespace owfs { namespace io {

   //--------------------------------------------------------------
   /// \brief	OWFS multi switch device read/write class
   //--------------------------------------------------------------
   class CMultiSwitch : public ioInterfaces::IMultiSwitch
   {
   public:
      //--------------------------------------------------------------
      /// \brief	The unit representation type
      //--------------------------------------------------------------
      enum EUnitRepresentation
      {
         kUnitIsZeroBasedNumber,
         kUnitIsUppercaseLetter
      };

   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	devicePath Device path
      /// \param[in]	unitRepresentation Unit representation type
      //--------------------------------------------------------------
      CMultiSwitch(const boost::filesystem::path& devicePath, EUnitRepresentation unitRepresentation);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CMultiSwitch();

   protected:
      // ioInterfaces::IMultiSwitch implementation
      virtual bool read(unsigned char unit) const;
      virtual void write(unsigned char unit, bool state) const;
      // [END] ioInterfaces::IMultiSwitch implementation

   private:
      //--------------------------------------------------------------
      /// \brief	OWFS device path
      //--------------------------------------------------------------
      const boost::filesystem::path m_devicePath;

      //--------------------------------------------------------------
      /// \brief	Unit representation type
      //--------------------------------------------------------------
      const EUnitRepresentation m_unitRepresentation;
   };

} } // namespace owfs::io