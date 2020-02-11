// LibusbTest.cpp : Just a test/demonstration on how to use Libusb++
// Anthony Clay, ZarthCode LLC

// Hopefully, spending a while studying this file and stepping around in debug mode should make it clear how easy
// (AND FAAAAST!) it can be to use LibUSB++ to create intuitive device interfaces.

// BUT...

// You still need to understand the USB protocol!
// http://www.beyondlogic.org/usbnutshell/usb1.shtml is a great place to start.

// Additionally, you'll need a compiler that supports C++11

// Oh, and you'll need a device to connect to. An Arduino Leonardo, PIC 18F4550, ...anything with real USB - and not some dingy ftdi serial interface :-p
// (Maybe I or someone else get an Arduino Leonardo and build a test app with commands/descriptors that correspond to this app)


#include <iostream>
#include <sstream>
#include <string>
#include <ios>
#include <list>
#include <algorithm>
#include <cstring>
#include <wchar.h>

#include <libusbpp.hpp>

// Utility functions

	/// Creates a std::shared_ptr<unsigned char> containing a wide string. (demo)
	std::shared_ptr<unsigned char> allocWString(const wchar_t* pString, size_t& outSize);

	/// Creates a std::shared_ptr<unsigned char> containing a string. (demo)
	std::shared_ptr<unsigned char> allocString(const char* pString, size_t& outSize);


// Test results
	std::list<std::wstring> testNames;
	std::list<bool> testResults;
	std::list<std::wstring> testFailureReasons;


// No using statements - I want it to be clear where everything is located.


int main(int argc, char* argv[])
{

	std::wcout << L"LibUSB++ - Feature Demonstration & Test Suite" << std::endl;
	std::wcout << L"To understand The Force ...YOU MUST READ THE SOURCE! (LibusbTest.cpp)" << std::endl << std::endl;
	std::wcout << L"Press ENTER to begin." << std::endl << std::endl;

	if(std::cin.get() == 27)
	{
		return 0;
	}

	/*
		To run this test suite, you're going to need a device. (Add the parameters below)
	*/

	uint16_t vendorID = 0x04D8;
	uint16_t productID = 0xFB92;

// TEST 1 - The Basics: Device Initialization, CONTROL transfers.
/*



*/
	{
		// wait for user input.
		testNames.push_back(L"Basic Init & Control Transfer");
		std::wcout << L"Test 1 - " << testNames.back() << " - Press any key to begin (ESC to exit)" << std::endl << std::endl;


		bool passed = true;

		try
		{


			std::wcout << L"Performing initial device test & transfer" << std::endl;

			// ALWAYS start with find device! The overloads should allow you to quickly find/filter what you're looking for.
			// The default factory will provide you with a newly-allocated (and initialized) LibUSB::Device object.
			std::list<std::shared_ptr<LibUSB::Device>> deviceList = LibUSB::LibUSB::FindDevice(vendorID, productID);

			if (deviceList.empty())
			{
				throw std::runtime_error("No compatible device found");
			}

			// For this demo, we'll just take the first one.
			std::shared_ptr<LibUSB::Device> pDevice = *deviceList.begin();

			// You will need to Open() the device to begin using it.
			pDevice->Open();

			// This is the fastest way to make transfers to the default control endpoint (endpoint 0).
			std::shared_ptr<LibUSB::ControlTransfer> pTransfer = std::static_pointer_cast<LibUSB::ControlTransfer>(pDevice->getControlEndpoint()->CreateTransfer());

			// This function is required.7
			pTransfer->SetupPacket(0,0,0);

			// No data for this example.

			// Fire the event
			std::wcout << L"Starting a transfer." << std::endl;
			pTransfer->Start();	// This will start a synchronous transfer.

			if(pTransfer->isComplete())	// Indicates that the transfer has been resolved.
			{

				if (pTransfer->isSuccessful())	// Indicates that data was sent (but not necessarily all of it!)
				{
					std::wcout << L"Completed successfully!" << std::endl;
				}
				else
				{
					std::wcout << "Transfer was unsuccessful." << std::endl;
					testFailureReasons.push_back(L"Transfer attempt unsuccessful.");
					passed = false;
				}

			}
		}
		catch (std::exception& e)
		{

			std::wcout << "Exception Thrown:\n\t" << e.what() << std::endl;
			std::wstringstream errStream;
			errStream << L"Exception Thrown:\t" << e.what();
			testFailureReasons.push_back(errStream.str());
			passed = false;

		}

		// wait for user input.
		std::wcout << L"Test 1 - Basic Init & Transfer - " << (passed ? L"PASSED" : L"FAILED") << std::endl << std::endl;
		testResults.push_back(passed);

	}

// TEST 2 - Enumeration and Descriptors
/*
	All descriptors are accessible using the Device class. This is the way to get right down to your config/interface/endpoints, and start making transfers.
*/

	{

		// wait for user input.
		testNames.push_back(L"Enumeration and descriptors");
		std::wcout << L"Test 2 - Enumeration and descriptors" << std::endl << std::endl;

		bool passed = true;

		// Get a list of my devices (Works with any winusb/libusb0.sys/libusbK device)
		std::list<std::shared_ptr<LibUSB::Device>> deviceList = LibUSB::LibUSB::FindDevice(vendorID, productID);


		// Open the device.
		for(std::shared_ptr<LibUSB::Device> pDevice : deviceList)
		{
			// When YOU do something bad, you'll probably get a std::logic_error - especially when it's the library-author's fault. ;-)
			// Be prepared for a std::runtime_error, especially if your device misbehaves, or gets disconnected.
			// There is also a LibUSBException, derived from std::runtime_error - if libusb cranks in an unexpected way... just pay attention to exception::what()
			try
			{

				// descriptors should be available at all times
				std::wcout << L"\nDevice\n\tVendorID: 0x" << std::hex << pDevice->vendorID() << std::endl;
				std::wcout << L"\tProductID: 0x" << std::hex << pDevice->productID() << std::endl;

				pDevice->Open(); // You can't access string descriptors until the device has been opened.

				// Get the manufacturer, product, and serial.
				std::wcout << L"\tManufacturer: " << pDevice->ManufacturerString() << std::endl;
				std::wcout << L"\tProduct: " << pDevice->ProductString() << std::endl;
				std::wcout << L"\tSerial: " << pDevice->SerialString() << std::endl << std::endl;

				// Obtain a configuration descriptor
				std::shared_ptr<LibUSB::Configuration> pActiveConfiguration = pDevice->getActiveConfiguration();

				if (pActiveConfiguration.get() == nullptr)
				{
					std::wcout << std::endl << L"\tDevice is NOT configured." << std::endl;

					// Oops, looks like the device isn't configured! Lets take care of that.
					pActiveConfiguration = pDevice->getConfiguration(1);

					if (pActiveConfiguration.get() == nullptr)
					{

						// Well, that does it...Maybe your device skipped the number ONE?
						throw std::runtime_error("Could not locate a configuration descriptor.");

					}

					pActiveConfiguration->SetAsActive();	// Note that there is currently

				}
				else
				{

					std::wcout << L"\tActive Configuration:" << std::endl;
					std::wcout << L"\t\tValue: " << pActiveConfiguration->Value() << std::endl;
					std::wcout << L"\t\tDescription: " << pActiveConfiguration->DescriptorString() << std::endl;
					std::wcout << L"\t\tMax Power: " << std::dec << pActiveConfiguration->MaxPower() << "mA" << std::endl;
					std::wcout << L"\t\tIs " << (pActiveConfiguration->isSelfPowered() ? "" : "NOT ") << "Self-powered." << std::endl;
					std::wcout << L"\t\tDoes " << (pActiveConfiguration->supportsRemoteWakeup() ? "" : "NOT ") << "support remote wakeup." << std::endl;
					std::wcout << L"\t\tDoes " << (pActiveConfiguration->hasExtraDescriptors() ? "" : "NOT ") << "have extra descriptors." << std::endl;

				}

				// Iterate the Interface descriptors
				for(int i = 0; i < pActiveConfiguration->NumInterfaces(); i++)
				{
					// Iterate the Interfaces (NOTE: If you're looking for a specific interface number, you should use getInterface(int interfaceNumber) instead!)
					std::shared_ptr<LibUSB::Interface> pInterface = pActiveConfiguration->getInterfaceByIndex(i);

					// Interface information
					std::wcout << L"\tInterface number: " << std::dec << pInterface->Number() << std::endl;
					std::wcout << L"\tInterface num alt settings: " << std::dec << pInterface->NumAlternateSettings() << std::endl;

					for (int alt = 0; alt < pInterface->NumAlternateSettings(); alt++)
					{

						// Set the alt setting to demonstrate inspection, without claiming the interface - (although, you can claim() now, if you want)
						pInterface->SetAlternate(alt);

						std::wcout << L"\tAlternate setting: " << std::dec << pInterface->AlternateSetting() << L":" << std::endl;


						std::wcout << L"\t\tInterface class: 0x" << std::hex << pInterface->Class() << std::endl;
						std::wcout << L"\t\tInterface subclass: 0x" << std::hex << pInterface->SubClass() << std::endl;
						std::wcout << L"\t\tInterface protocol: 0x" << std::hex << pInterface->Protocol() << std::endl;
						std::wcout << L"\t\tInterface descriptor:" << pInterface->DescriptorString() << std::endl;
						std::wcout << L"\t\tInterface Number of endpoints: " << std::dec << pInterface->NumEndpoints() << std::endl << std::endl;

						// Iterate over the interface endpoints (other than ep0!!) - by the index to number map
						LibUSB::Interface::EndpointNumbers_t mEndpointNumbers = pInterface->getEndpointNumbers();
						for (const auto &epnum : mEndpointNumbers)
						{

							std::shared_ptr<LibUSB::Endpoint> pEndpoint = pInterface->getEndpoint(epnum.second);

							/// Endpoint information
							std::wcout << L"\t\t\tEndpoint number: " << std::dec << pEndpoint->Number() << std::endl;
							std::wcout << L"\t\t\tMaxPacketSize: " << std::dec << pEndpoint->MaxPacketSize() << std::endl;
							std::wcout << L"\t\t\tPolling Interval: " << std::dec << pEndpoint->PollingInterval() << std::endl;
							std::wcout << L"\t\t\tTransfer Type: ";


							switch (pEndpoint->TransferType())
							{
							case LibUSB::Transfer_t::BULK:
								std::wcout << L"Bulk" << std::endl;
								break;

							case LibUSB::Transfer_t::CONTROL:
								std::wcout << L"Control" << std::endl;
								break;

							case LibUSB::Transfer_t::INTERRUPT:
								std::wcout << L"Interrupt" << std::endl;
								break;
							case LibUSB::Transfer_t::ISOCHRONOUS:

								std::wcout << L"Isochronous" << std::endl;
								std::wcout << L"\t\t\t\tSynchronization Type: ";

								// Isochronous endpoints have some extra info.

								// Synch type
								switch (pEndpoint->SynchronizationType())
								{
								case LibUSB::Synchronization_t::ADAPTIVE:
									std::wcout << L"Adaptive";
								case LibUSB::Synchronization_t::ASYNCRONOUS:
									std::wcout << L"Asynchronous";
								case LibUSB::Synchronization_t::SYNCHRONOUS:
									std::wcout << L"Synchronous";
								case LibUSB::Synchronization_t::NONE:
									std::wcout << L"None";
									break;
								default:
									std::wcout << L"Unrecognized! (" << (int)pEndpoint->SynchronizationType() << L")";
									break;
								}
								std::wcout << std::endl;

								std::wcout << L"\t\t\t\tUsage Type: ";

								// Usage Type
								switch (pEndpoint->UsageType())
								{
								case LibUSB::Usage_t::DATA:
									std::wcout << L"Data";
									break;
								case LibUSB::Usage_t::FEEDBACK:
									std::wcout << L"Feedback";
									break;
								case LibUSB::Usage_t::EXPLICITFEEDBACKDATA:
									std::wcout << L"Explicit Feedback Data";
									break;
								case LibUSB::Usage_t::RESERVED:
									std::wcout << L"Reserved";
									break;
								default:
									std::wcout << L"Unrecognized! (" << (int)pEndpoint->UsageType() << L")";
									break;
								}
								std::wcout << std::endl;
								break;
							default:
								std::wcout << std::endl << L"Unrecognized Transfer Type! (" << (int)pEndpoint->TransferType() << L")";
								break;
							}

							std::wcout << L"\t\t\tDirection: " << ((pEndpoint->Direction() == LibUSB::Direction_t::DIR_OUT) ? L"OUT" : L"IN") << std::endl << std::endl;

						}

					}
				}
			}
			catch (std::exception& e)
			{

				std::wcout << "Exception Thrown:\n\t" << e.what() << std::endl;
				std::wstringstream errStream;
				errStream << L"Exception Thrown:\t" << e.what();
				testFailureReasons.push_back(errStream.str());
				passed = false;

			}

		}


		// wait for user input.
		std::wcout << L"Test 2 - Enumeration & Descriptors - " << (passed ? L"PASSED" : L"FAILED") << std::endl << std::endl;
		testResults.push_back(passed);

	}



// TEST 3 - Threaded Transfers
	{

		// wait for user input.
		std::wcout << L"Test 3 - Asynchronous/Threaded Transfers" << std::endl << std::endl;
		testNames.push_back(L"Asynchronous & Threaded Transfers");
		bool passed = true;

		try
		{
			std::wcout << L"Performing initial device test & transfer" << std::endl;
			std::list<std::shared_ptr<LibUSB::Device>> deviceList = LibUSB::LibUSB::FindDevice(vendorID, productID);

			if (deviceList.empty())
			{
				throw std::runtime_error("No compatible device found");
			}

			std::shared_ptr<LibUSB::Device> pDevice = *deviceList.begin();

			pDevice->Open();

			std::shared_ptr<LibUSB::ControlTransfer> pTransfer = std::static_pointer_cast<LibUSB::ControlTransfer>(pDevice->getControlEndpoint()->CreateTransfer());

			pTransfer->SetupPacket(0,0,0);

			// Fire the event
			std::wcout << L"Starting a transfer." << std::endl;

			pTransfer->AsyncStart();		// Async start spawns a new thread, all the gory details are hidden away!

			long Problems = 0;
			while(!pTransfer->isComplete() || (Problems < 99))		// isComplete will perform the check without blocking
			{
				Problems++;
			}

			// To wait on the result instead
			pTransfer->WaitForCompletion();	// Blocks, then returns isComplete() result.

			std::wcout << L"If you're having concurrency problems, I feel bad for you, son." << std::endl << "\tI got " << std::dec << Problems << L" problems...but USB ain't one!" << std::endl;

			if(pTransfer->isComplete())
			{

				if (pTransfer->isSuccessful())
				{
					std::wcout << L"Completed successfully!" << std::endl;
				}
				else
				{
					std::wcout << L"Test failed - unsuccessful transfer." << std::endl;
					testFailureReasons.push_back(L"Unsuccessful transfer attempt.");
					passed = false;
				}

			}
		}
		catch (std::exception e)
		{

			std::wcout << L"Exception Thrown:\n\t" << e.what() << std::endl;
			std::wstringstream errStream;
			errStream << L"Exception Thrown:\t" << e.what();
			testFailureReasons.push_back(errStream.str());
			passed = false;

		}


		// wait for user input.
		std::wcout << L"Test complete." << std::endl;
		testResults.push_back(passed);

	}

// Test 4 - Interrupt Transfers
/*
	Interrupts are designed to be low-latency messages, and only move in one direction, per endpoint.


*/
	{

		// wait for user input.
		std::wcout << L"Test 4 - Interrupt Transfers" << std::endl << std::endl;
		testNames.push_back(L"Interrupt Transfers");

		bool passed = true;

		try
		{

			// Initialization
			std::wcout << L"Initializing Device object and grabbing endpoints..." << std::endl;
			std::list<std::shared_ptr<LibUSB::Device>> deviceList = LibUSB::LibUSB::FindDevice(vendorID, productID);

			if (deviceList.empty())
			{
				throw std::runtime_error("No compatible device found");
			}

			std::shared_ptr<LibUSB::Device> pDevice = *deviceList.begin();

			pDevice->Open();

			std::shared_ptr<LibUSB::Interface> pInterface = pDevice->getActiveConfiguration()->getInterface(0);


					// Note that at this point, the alternate setting is in it's default position. You can get endpoints, or choose a different alternate setting, if you wanted.

			pInterface->Claim();	// This is necessary before allocating transfers.

					// You can choose your alternate setting now, however, the 'current' alternate setting is automatically once you claim() (last chosen, or the default)


			// We need an endpoint object in order to create transfers (Oh yeah, don't be cute and deallocate it, either.  Or you'll break your transfers!)

			std::shared_ptr<LibUSB::Endpoint> pInterruptOUTEndpoint = pInterface->getEndpoint(1);		// Note that endpoint 0 is ALWAYS the default control endpoint, no matter the interface.
			std::shared_ptr<LibUSB::Endpoint> pInterruptINEndpoint = pInterface->getEndpoint(2);


			// Create Transfer objects and allocate memory
			std::wcout << L"Creating transfer objects..." << std::endl;

			std::shared_ptr<LibUSB::InterruptTransfer> pTransferOUT = std::static_pointer_cast<LibUSB::InterruptTransfer>(pInterruptOUTEndpoint->CreateTransfer());

			std::shared_ptr<LibUSB::InterruptTransfer> pTransferIN = std::static_pointer_cast<LibUSB::InterruptTransfer>(pInterruptINEndpoint->CreateTransfer());

			size_t pingDataSize = 0;
			std::shared_ptr<unsigned char> pPingString = allocString("PING!", pingDataSize);
			pTransferOUT->setTransferBuffer(pPingString, pingDataSize);

			size_t returnDataSize = 32; // 6 * sizeof(wchar_t);
			std::shared_ptr<unsigned char> pReturnString(new unsigned char[returnDataSize], [](unsigned char* ptr){ delete [] ptr; });
			std::fill_n(pReturnString.get(), returnDataSize, '\0');
			pTransferIN->setTransferBuffer(pReturnString, returnDataSize);



			// Send "Ping!" to the device
			std::wcout << L"Sending Interrupt with string data \"Ping!\"" << std::endl;
			pTransferOUT->Start();

			if (pTransferOUT->isComplete() && pTransferOUT->isSuccessful())
			{
				std::wcout << L"Interrupt sent successfully." << std::endl;
			}
			else
			{
				std::wcout << L"Interrupt not sent!" << std::endl;
				testFailureReasons.push_back(L"Interrupt send unsuccessful.");
				passed = false;
			}

			// Wait a wink.
		//	std::this_thread::sleep_for(std::chrono::milliseconds(100));

			// Receive Interrupt from device
			std::wcout << L"Awaiting interrupt from device..." << std::endl;
			pTransferIN->SetTimeout(std::chrono::milliseconds(300));
			pTransferIN->Start();		// Normally, you'd handle this asynchronously.

			if (pTransferIN->isComplete() && pTransferIN->isSuccessful())
			{
				std::wcout << L"Interrupt received: \"" << (wchar_t*)pTransferIN->getTransferBuffer().get() << L"\" (" << pTransferIN->BytesTransferred() << L")" << std::endl;
			}
			else
			{
				std::wcout << L"Interrupt not received! (Reason " << pTransferIN->Result() << L")"<< std::endl;
				std::wstringstream errorStr;
				if(passed)
				{

					errorStr << L"Return interrupt not received (Reason " << pTransferIN->Result() << L")";
				}
				else
				{
					errorStr << testFailureReasons.back() << L"Interrupt not received! (Reason " << pTransferIN->Result() << L")";
					testFailureReasons.pop_back();
				}
				testFailureReasons.push_back(errorStr.str());
				passed = false;
			}

 			pInterface->Release();



		}
		catch(std::exception &e)
		{

			std::wcout << "Exception Thrown:\n\t" << e.what() << std::endl;
			std::wstringstream errStream;
			errStream << L"Exception Thrown:\t" << e.what();
			testFailureReasons.push_back(errStream.str());
			passed = false;

		}

		// wait for user input.
		std::wcout << L"Test 4 - Interrupt Transfers - " << (passed ? L"PASSED" : L"FAILED") << std::endl << std::endl;
		testResults.push_back(passed);
	}




// Test 5 - Bulk Transfers
/*

	Bulk transfers are designed to carry data with high integrity, but no time/speed guarantees, and only move in one direction per endpoint.

*/
	{

		// wait for user input.
		std::wcout << L"Test 5 - Bulk Transfers" << std::endl << std::endl;
		testNames.push_back(L"Bulk Transfers");

		bool passed = true;

		try
		{

			// Initialization
			std::wcout << L"Initializing Device object and grabbing endpoints..." << std::endl;
			std::list<std::shared_ptr<LibUSB::Device>> deviceList = LibUSB::LibUSB::FindDevice(vendorID, productID);

			if (deviceList.empty())
			{
				throw std::runtime_error("No compatible device found");
			}

			std::shared_ptr<LibUSB::Device> pDevice = *deviceList.begin();

			pDevice->Open();

			std::shared_ptr<LibUSB::Interface> pInterface = pDevice->getActiveConfiguration()->getInterface(0);


					// Note that at this point, the alternate setting is in it's default position. You can get endpoints, or choose a different alternate setting, if you wanted.

			pInterface->Claim();	// This is necessary before allocating transfers.

					// You can choose your alternate setting now, however, the 'current' alternate setting is automatically once you claim() (last chosen, or the default)


			// We need an endpoint object in order to create transfers (Oh yeah, don't be cute and deallocate it, either.  Or you'll break your transfers!)

			std::shared_ptr<LibUSB::Endpoint> pBulkOUTEndpoint = pInterface->getEndpoint(4);		// Note that endpoint 0 is ALWAYS the default control endpoint, no matter the interface.
			std::shared_ptr<LibUSB::Endpoint> pBulkINEndpoint = pInterface->getEndpoint(3);


			// Create Transfer objects and allocate memory
			std::wcout << L"Creating transfer objects..." << std::endl;


			if (pBulkOUTEndpoint->Direction() != LibUSB::Direction_t::DIR_OUT)
			{
				throw std::logic_error("Wrong endpoint!");
			}

			if (pBulkINEndpoint->Direction() != LibUSB::Direction_t::DIR_IN)
			{
				throw std::logic_error("Wrong endpoint!");
			}

			std::shared_ptr<LibUSB::BulkTransfer> pTransferOUT = std::static_pointer_cast<LibUSB::BulkTransfer>(pBulkOUTEndpoint->CreateTransfer());

			std::shared_ptr<LibUSB::BulkTransfer> pTransferIN = std::static_pointer_cast<LibUSB::BulkTransfer>(pBulkINEndpoint->CreateTransfer());

			size_t pingDataSize = 0;
			std::shared_ptr<unsigned char> pPingString = allocString("apostle", pingDataSize);
			pTransferOUT->setTransferBuffer(pPingString, pingDataSize);

			size_t returnDataSize = pingDataSize;
			std::shared_ptr<unsigned char> pReturnString(new unsigned char[returnDataSize], [](unsigned char* ptr){ delete [] ptr; });
			std::fill_n(pReturnString.get(), returnDataSize, '\0');
			pTransferIN->SetTimeout(std::chrono::milliseconds(5000));
			pTransferIN->setTransferBuffer(pReturnString, returnDataSize);



			// Send "Ping!" to the device
			std::wcout << L"Sending Bulk string data \"" << (char*)pPingString.get() << "\"" << std::endl;
			pTransferOUT->Start();

			if (pTransferOUT->isComplete() && pTransferOUT->isSuccessful())
			{
				std::wcout << L"Bulk data sent successfully." << std::endl;
			}
			else
			{
				std::wcout << L"data not sent!" << std::endl;
				testFailureReasons.push_back(L"Bulk OUT unsuccessful.");
				passed = false;
			}

			// Wait a wink.
		//	std::this_thread::sleep_for(std::chrono::milliseconds(100));

			// Receive Interrupt from device
			std::wcout << L"Awaiting Bulk data from device..." << std::endl;
			pTransferIN->SetTimeout(std::chrono::milliseconds(500));
			pTransferIN->Start();		// Normally, you'd handle this asynchronously.

			if (pTransferIN->isComplete() && pTransferIN->isSuccessful())
			{
				std::wcout << L"Bulk IN received: \"" << (char*)pTransferIN->getTransferBuffer().get() << L"\" (" << pTransferIN->BytesTransferred() << L")" << std::endl;
			}
			else
			{
				std::wcout << L"Data not received! (Reason " << pTransferIN->Result() << L")"<< std::endl;
				std::wstringstream errorStr;
				if(passed)
				{

					errorStr << L"Return data IN not received (Reason " << pTransferIN->Result() << L")";
				}
				else
				{
					errorStr << testFailureReasons.back() << L"Data IN not received! (Reason " << pTransferIN->Result() << L")";
					testFailureReasons.pop_back();
				}
				testFailureReasons.push_back(errorStr.str());
				passed = false;
			}

 			pInterface->Release();



		}
		catch(std::exception &e)
		{

			std::wcout << "Exception Thrown:\n\t" << e.what() << std::endl;
			std::wstringstream errStream;
			errStream << L"Exception Thrown:\t" << e.what();
			testFailureReasons.push_back(errStream.str());
			passed = false;

		}

		// wait for user input.
		std::wcout << L"Test 4 - Bulk Transfers - " << (passed ? L"PASSED" : L"FAILED") << std::endl << std::endl;
		testResults.push_back(passed);
	}


// Test 6 - Isochronous Transfers
/*

	Isochronous transfers are designed to carry data with low latency, but with no delivery guarantee (data packets can be dropped), and only move in one direction per endpoint.

*/
	{

		// wait for user input.
		std::wcout << L"Test 6 - Isochronous Transfers" << std::endl << std::endl;
		testNames.push_back(L"Isochronous Transfers");

		bool passed = true;

		try
		{

			// Initialization
			std::wcout << L"Initializing Device object and grabbing endpoints..." << std::endl;
			std::list<std::shared_ptr<LibUSB::Device>> deviceList = LibUSB::LibUSB::FindDevice(vendorID, productID);

			if (deviceList.empty())
			{
				throw std::runtime_error("No compatible device found");
			}

			std::shared_ptr<LibUSB::Device> pDevice = *deviceList.begin();

			pDevice->Open();

			std::shared_ptr<LibUSB::Interface> pInterface = pDevice->getActiveConfiguration()->getInterface(0);


					// Note that at this point, the alternate setting is in it's default position. You can get endpoints, or choose a different alternate setting, if you wanted.

			pInterface->Claim();	// This is necessary before allocating transfers.

					// You can choose your alternate setting now, however, the 'current' alternate setting is automatically once you claim() (last chosen, or the default)


			// We need an endpoint object in order to create transfers (Oh yeah, don't be cute and deallocate it, either.  Or you'll break your transfers!)

			std::shared_ptr<LibUSB::Endpoint> pIsochronousOUTEndpoint = pInterface->getEndpoint(5);		// Note that endpoint 0 is ALWAYS the default control endpoint, no matter the interface.
			std::shared_ptr<LibUSB::Endpoint> pIsochronousINEndpoint = pInterface->getEndpoint(6);


			// Create Transfer objects and allocate memory
			std::wcout << L"Creating transfer objects..." << std::endl;


			if (pIsochronousOUTEndpoint->Direction() != LibUSB::Direction_t::DIR_OUT)
			{
				throw std::logic_error("Wrong endpoint!");
			}

			if (pIsochronousINEndpoint->Direction() != LibUSB::Direction_t::DIR_IN)
			{
				throw std::logic_error("Wrong endpoint!");
			}

			std::shared_ptr<LibUSB::IsochronousTransfer> pTransferOUT = std::static_pointer_cast<LibUSB::IsochronousTransfer>(pIsochronousOUTEndpoint->CreateTransfer());

			std::shared_ptr<LibUSB::IsochronousTransfer> pTransferIN = std::static_pointer_cast<LibUSB::IsochronousTransfer>(pIsochronousINEndpoint->CreateTransfer());

			size_t pingDataSize = 0;
			std::shared_ptr<unsigned char> pPingString = allocString("apostle", pingDataSize);

			pTransferOUT->setNumPackets(1);
			pTransferOUT->setTransferBuffer(pPingString, pingDataSize);

			size_t returnDataSize = pingDataSize;
			std::shared_ptr<unsigned char> pReturnString(new unsigned char[returnDataSize], [](unsigned char* ptr){ delete [] ptr; });
			std::fill_n(pReturnString.get(), returnDataSize, '\0');

			pTransferIN->SetTimeout(std::chrono::milliseconds(5000));
			pTransferIN->setNumPackets(1);		// Just sending one packet

			pTransferIN->setTransferBuffer(pReturnString, returnDataSize);



			// Send "Ping!" to the device
			std::wcout << L"Sending Isochronous string data \"" << (char*)pPingString.get() << "\"" << std::endl;
			pTransferOUT->Start();

			if (pTransferOUT->isComplete() && pTransferOUT->isSuccessful())
			{
				std::wcout << L"Isochronous data sent successfully." << std::endl;
			}
			else
			{
				std::wcout << L"data not sent!" << std::endl;
				testFailureReasons.push_back(L"Isochronous OUT unsuccessful.");
				passed = false;
			}

			// Wait a wink.
		//	std::this_thread::sleep_for(std::chrono::milliseconds(100));

			// Receive Interrupt from device
			std::wcout << L"Awaiting Isochronous data from device..." << std::endl;
			pTransferIN->SetTimeout(std::chrono::milliseconds(0));
			pTransferIN->Start();		// Normally, you'd handle this asynchronously.

			if (pTransferIN->isComplete() && pTransferIN->isSuccessful())
			{
				std::wcout << L"Isochronous IN received: \"" << (char*)pTransferIN->getTransferBuffer().get() << L"\" (" << pTransferIN->BytesTransferred() << L")" << std::endl;
			}
			else
			{
				std::wcout << L"Data not received! (Reason " << pTransferIN->Result() << L")"<< std::endl;
				std::wstringstream errorStr;
				if(passed)
				{

					errorStr << L"Return data IN not received (Reason " << pTransferIN->Result() << L")";
				}
				else
				{
					errorStr << testFailureReasons.back() << L"Data IN not received! (Reason " << pTransferIN->Result() << L")";
					testFailureReasons.pop_back();
				}
				testFailureReasons.push_back(errorStr.str());
				passed = false;
			}

 			pInterface->Release();



		}
		catch(std::exception &e)
		{

			std::wcout << "Exception Thrown:\n\t" << e.what() << std::endl;
			std::wstringstream errStream;
			errStream << L"Exception Thrown:\t" << e.what();
			testFailureReasons.push_back(errStream.str());
			passed = false;

		}

		// wait for user input.
		std::wcout << L"Test 6 - Isochronous Transfers - " << (passed ? L"PASSED" : L"FAILED") << std::endl << std::endl;
		testResults.push_back(passed);
	}


// Test 5 - Demonstrate using a derived class built on top of device to make "pretty" device interfaces.
/*
	I designed this not as a thin set of wrappers over libusb, but as a true OO approach to a usb device interface.
	In my own designs, I plan to encapsulate LibUSB::Device inside of my own device class.

	The LibUSB::Device class has a neat notification function that makes it easy/possible to process completed transfers as events

*/
/*	{
		// wait for user input.
		std::wcout << L"Test 4 - Basics - Press any key to continue (ESC to exit)" << std::endl << std::endl;
		if(_getch() == 27)
		{
			return 0;
		}
		bool passed = true;

		try
		{

		}
		catch(std::exception& e)
		{

			std::wcout << "Exception Thrown:\n\t" << e.what() << std::endl;
			std::wstringstream errStream;
			errStream << L"Exception Thrown:\t" << e.what();
			testFailureReasons.push_back(errStream.str());
			passed = false;

		}

		// wait for user input.
		std::wcout << L"Test 2 - Basics - " << (passed ? L"PASSED" : L"FAILED") << std::endl << std::endl;
		_getch();
	}
*/

	// Display test results

	std::wcout << L"###################### RESULTS ######################" << std::endl;
	int i = 1;
	bool failBit = false;
	while(!testNames.empty())
	{
		std::wcout << L"Test " << i++ << L" - " << testNames.front();
		std::wcout << L"\t" << (testResults.front() ? L"PASSED" : L"FAILED!") << std::endl;
		if (!testResults.front())
		{
			std::wcout << L"\tReason: " << testFailureReasons.front() << std::endl;
			testFailureReasons.pop_front();
			failBit = true;
		}

		testNames.pop_front();
		testResults.pop_front();

		std::wcout << std::endl;
	}

	std::wcout << L"Finished - Press ENTER to exit." << std::endl << std::endl;
	std::cin.get();

	// Return the result.
	return failBit ? EXIT_FAILURE : EXIT_SUCCESS;
}

std::shared_ptr<unsigned char> allocWString( const wchar_t* pString, size_t& outSize )
{

	size_t strSize = wcslen(pString);

	outSize = (strSize * sizeof(wchar_t)) + 2;

	std::shared_ptr<unsigned char> outString(new unsigned char[outSize], [](unsigned char * ptr){ delete [] ptr; });

	wcscpy((wchar_t*)outString.get(), pString);

	return outString;

}

std::shared_ptr<unsigned char> allocString( const char* pString, size_t& outSize )
{

	size_t strSize = strlen(pString);

	outSize = (strSize * sizeof(char)) + 1;

	std::shared_ptr<unsigned char> outString(new unsigned char[outSize], [](unsigned char * ptr){ delete [] ptr; });

	strcpy((char*)outString.get(), pString);

	return outString;

}
