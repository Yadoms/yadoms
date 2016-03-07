#pragma once
#include "ioInterfaces/IQuadAdConverter.h"

namespace owfs { namespace io {

   //--------------------------------------------------------------
   /// \brief	OWFS quad AD converter device read/write class
   //--------------------------------------------------------------
   class CQuadAdConverter : public ioInterfaces::IQuadAdConverter
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	devicePath Device path
      //--------------------------------------------------------------
      explicit CQuadAdConverter(const boost::filesystem::path& devicePath);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CQuadAdConverter();

   protected:
      // ioInterfaces::IQuadAdConverter implementation
      virtual double read(unsigned char unit) const;
      // [END] ioInterfaces::IQuadAdConverter implementation

   private:
      //--------------------------------------------------------------
      /// \brief	OWFS device path
      //--------------------------------------------------------------
      const boost::filesystem::path m_devicePath;
   };

} } // namespace owfs::io