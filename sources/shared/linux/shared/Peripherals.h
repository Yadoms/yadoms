//
// Peripherals.h
//
// Platforms peripherals management (serial ports...)
//
#pragma once

//--------------------------------------------------------------
/// \brief	this class is used to manage platforms peripherals
//--------------------------------------------------------------
class CPeripherals
{
public:
   //--------------------------------------------------------------
   /// \brief			Constructor
   //--------------------------------------------------------------
   CPeripherals();

   //--------------------------------------------------------------
   /// \brief			Destructor
   //--------------------------------------------------------------
   virtual ~CPeripherals();

   //--------------------------------------------------------------
   /// \brief			List machine serial ports (all)
   /// \return       The serial ports names
   //--------------------------------------------------------------
   static const boost::shared_ptr<std::vector<std::string> > getSerialPorts();

   //--------------------------------------------------------------
   /// \brief			List unused serial ports
   /// \return       The serial ports names
   //--------------------------------------------------------------
   static const boost::shared_ptr<std::vector<std::string> > getUnusedSerialPorts();

   //--------------------------------------------------------------
   /// \brief			List used serial ports
   /// \return       The serial ports names
   //--------------------------------------------------------------
   static const boost::shared_ptr<std::vector<std::string> > getUsedSerialPorts();
};
