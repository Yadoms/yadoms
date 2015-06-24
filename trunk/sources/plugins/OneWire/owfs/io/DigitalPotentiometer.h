#pragma once
#include "ioInterfaces/IDigitalPotentiometer.h"

namespace owfs { namespace io {

   //--------------------------------------------------------------
   /// \brief	OWFS digital potentiometer device read/write class
   //--------------------------------------------------------------
   class CDigitalPotentiometer : public ioInterfaces::IDigitalPotentiometer
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	devicePath Device path
      //--------------------------------------------------------------
      CDigitalPotentiometer(const boost::filesystem::path& devicePath);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CDigitalPotentiometer();

   protected:
      // ioInterfaces::IDigitalPotentiometer implementation
      virtual bool readPotentiometerMode() const;
      virtual void writePotentiometerMode(bool potentiometerMode) const;
      virtual int readDim() const;
      virtual void writeDim(int dim) const;
      // [END] ioInterfaces::IDigitalPotentiometer implementation

   private:
      //--------------------------------------------------------------
      /// \brief	OWFS device paths
      //--------------------------------------------------------------
      const boost::filesystem::path m_modePath;
      const boost::filesystem::path m_dimPath;
   };

} } // namespace owfs::io