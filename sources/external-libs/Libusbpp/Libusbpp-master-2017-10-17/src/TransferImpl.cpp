/*
 * Copyright (C) 2012, Anthony Clay, ZarthCode LLC, all rights reserved.
 * Copyright (C) 2016, Stephan Linz, Li-Pro.Net, all rights reserved.
 *
 * This file is part of the LibUSB C++ wrapper library (libusbpp).
 *
 * libusbpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libusbpp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libusbpp.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdexcept>
#include <sstream>
#include <cstring>

#include <libusbpp.hpp>
#include <libusbpp/Exception.hpp>

#include "TransferImpl.hpp"
#include "DeviceImpl.hpp"
#include "EndpointImpl.hpp"
#include "LibusbImpl.hpp"


LibUSB::TransferImpl::TransferImpl(std::weak_ptr<EndpointImpl> pEndpointImpl)
	: m_Timeout(0), m_Complete(false), m_Submitted(false), m_transferSize(0)
{

	if (pEndpointImpl.expired())
	{
		throw std::logic_error("LibUSB::TransferImpl::TransferImpl(): Given an expired EndpointImpl.");
	}

	m_pEndpointImpl = pEndpointImpl;


}

LibUSB::TransferImpl::~TransferImpl()
{
	// Make sure that a transfer isn't active.



}

void LibUSB::TransferImpl::AllocTransfer( int iso_packets /* = 0 */ )
{

	// Create a libusb transfer object.
	m_pTransfer.reset(libusb_alloc_transfer(iso_packets), TransferDeleter());

	if (m_pTransfer.get() == nullptr)
	{

		throw std::runtime_error("LibUSB::TransferImpl::AllocTransfer(): Allocation failed!");

	}
}

void LibUSB::TransferImpl::setBuffer( std::shared_ptr<unsigned char> pTransferBuffer )
{

	if (m_Submitted)
	{
		throw std::logic_error("LibUSB::TransferImpl::setBuffer(): Cannot change buffer after submission.  Reset() first.");
	}
	m_pTransferBuffer = pTransferBuffer;

}

std::shared_ptr<unsigned char> LibUSB::TransferImpl::getBuffer()
{

	/// \todo Operation must be complete.


	return m_pTransferBuffer;

}

void LibUSB::TransferImpl::Start()
{

	if (m_Submitted && !m_Complete)
	{
		throw std::logic_error("LibUSB::TransferImpl::Start(): Transfer has already been submitted, but not complete. You must Reset()");
	}

	m_Submitted = false;
	m_Complete = false;

	// 1. Create & Fill the transfer object
	Setup();

	// 2. Ensure that there is a libusb transfer object.
	if (m_pTransfer.get() == nullptr)
	{
	 	throw std::logic_error("LibUSB::TransferImpl::Start(): Libusb transfer struct not allocated!");
	}

	// 3. Ensure that the callback can reach this transfer.
	m_TransferMap.insert(make_pair(this, shared_from_this()));

	// 4. Submit the transfer
	int Result = libusb_submit_transfer(m_pTransfer.get());

	switch (Result)
	{
	case LIBUSB_SUCCESS:
		// No error
		break;
	case LIBUSB_ERROR_NO_DEVICE:
		throw std::runtime_error("LibUSB::TransferImpl::Start(): Transfer submission failed.  Device has been disconnected.");
		break;
	case LIBUSB_ERROR_BUSY:
		throw std::logic_error("LibUSB::TransferImpl::Start(): Transfer has already been submitted. Internal error - this shouldn't happen.");
		break;
	case LIBUSB_ERROR_NOT_SUPPORTED:
		if (m_pTransfer->type == LIBUSB_TRANSFER_TYPE_ISOCHRONOUS)
		{
			throw std::logic_error("LibUSB::TransferImpl::Start(): The transfer flags are not supported by the OS. (Hint: Check that your driver supports isochronous transfers)");
		}
		throw std::logic_error("LibUSB::TransferImpl::Start(): The transfer flags are not supported by the OS.");
		break;

	default:
		throw LibUSBException("LibUSB::TransferImpl::Start(): Failed: ", Result);
		break;
	}


	m_Submitted = true;


	// 4. Perform the transfer right now.

	m_eventCompleted = 0;

	int CancelReason = 0;

	while (!m_Complete && (CancelReason == 0))
	{


		int	Result = libusb_handle_events_completed(LibUSB::Impl_->m_pLibusb_context.get(), &m_eventCompleted );



		switch (Result)
		{

		case LIBUSB_SUCCESS:
			// Means nothing unless m_Complete is true :-)
			break;

		case LIBUSB_ERROR_INTERRUPTED:
			// Try again.
			break;

		default:

			// Cancel the transfer.
			int CancelResult = libusb_cancel_transfer(m_pTransfer.get());
			CancelReason = Result;

			switch (CancelResult)
			{
			case LIBUSB_SUCCESS:
				// Cancelled successfully.
				break;

			case LIBUSB_ERROR_NOT_FOUND:
				// Already complete or canceled.
				if (!m_Complete)
				{
					// I'm totally confused.
					throw std::logic_error("LibUSB::TransferImpl::Start(): Failed - Cancelation Fuzzy.");
				}
				break;
			default:
				// Combo failure!
				std::stringstream strstr;
				strstr << "LibUSB::TransferImpl::Start(): Cancellation Failed(" << std::dec << CancelResult << ") - Cancel Reason: ";
				throw LibUSBException(strstr.str(), CancelReason);
				break;
			}

			// Wait for completion.
			while(!m_Complete)
			{
				if(libusb_handle_events_completed(LibUSB::Impl_->m_pLibusb_context.get(), &m_eventCompleted ) < 0)
					break;
			}
			break;
		}



	}

	// That's all.

}

void LibUSB::TransferImpl::setTimeout( std::chrono::milliseconds timeout )
{

	m_Timeout = std::chrono::duration_cast<std::chrono::duration<unsigned int, std::milli>>(timeout).count();

}

void LibUSB::TransferImpl::AsyncTransferCallback( libusb_transfer* pTransfer )
{

	TransferImpl* index = (TransferImpl*) pTransfer->user_data;

	/// Obtain the transfer's shared_ptr
	m_TransferMap[index].lock()->NotifyComplete();


}

void LibUSB::TransferImpl::NotifyComplete()
{

	// Mark as completed.
	m_Complete = true;

	// Ensure the async transfer ends.
	m_eventCompleted++;


	// Get a pointer to the parent device
	std::shared_ptr<Device> pDevice = m_pEndpointImpl.lock()->getDeviceImpl().lock()->getDevice().lock();

	// Act on the result
	pDevice->TransferEventNotification(m_pParentTransfer.lock());


	// Remove the TransferMap entry.
	m_TransferMap.erase(this);
}

void LibUSB::TransferImpl::Reset()
{

	if (m_Submitted && !m_Complete)
	{
		throw std::logic_error("LibUSB::TransferImpl::Reset(): Cannot Reset() Transfer that is in progress!");
	}

	m_transferSize = 0;
	m_pTransferBuffer.reset();

	AllocTransfer();

	m_Submitted = false;
	m_Complete = false;

}

void LibUSB::TransferImpl::setDataTransferSize( size_t transferSize )
{

	m_transferSize = transferSize;
}

void LibUSB::TransferImpl::init( std::weak_ptr<Transfer> pTransferParent )
{

	if (pTransferParent.expired())
	{
		throw std::logic_error("LibUSB::TransferImpl::init(): Expired Transfer pointer.");
	}

	m_pParentTransfer = pTransferParent;
}

void LibUSB::TransferImpl::Cancel()
{

	// This is going to require some more thought.
	throw std::runtime_error("LibUSB::TransferImpl::Cancel(): Not implemented!");

}

size_t LibUSB::TransferImpl::bytesTransferred() const
{

	if (!m_Complete)
	{
		// Not good.
		throw std::logic_error("LibUSB::TransferImpl::bytesTransferred(): Transfer is not complete.");

	}

	return m_pTransfer->actual_length;
}

bool LibUSB::TransferImpl::isComplete() const
{

	return m_Complete;

}

bool LibUSB::TransferImpl::isSuccessful() const
{

	if (!m_Complete)
	{
		throw std::logic_error("LibUSB::TransferImpl::isSuccessful(): Should not be called unless transfer has been completed.");
	}

	// Examine the transfer status
	try {

		HandlePendingExceptions();

	}
	catch (std::exception& e)
	{
		// This is just an opportunity to debug.
		throw e;
	}


	return (m_pTransfer->status == LIBUSB_TRANSFER_COMPLETED);


}

void LibUSB::TransferImpl::HandlePendingExceptions()const
{

	// Act on the result
	switch (m_pTransfer->status)
	{
	case LIBUSB_TRANSFER_COMPLETED:
		// Transfer completed without error.
		// Note that this does not indicate that the entire amount of requested data was transferred.


		break;

	case LIBUSB_TRANSFER_ERROR:
		// Transfer failed.
		throw std::runtime_error("LibUSB::TransferImpl::HandlePendingExceptions(): Transfer failed.");
		break;

	case LIBUSB_TRANSFER_TIMED_OUT:
		// Transfer timed out.
		/// \todo Add optional/configurable exceptions
		// throw std::runtime_error("LibUSB::TransferImpl::HandlePendingExceptions(): Transfer timed out.");
		break;

	case LIBUSB_TRANSFER_CANCELLED:
		// Transfer was canceled.
		/// \todo Add optional/configurable exceptions
		// throw std::runtime_error("LibUSB::TransferImpl::HandlePendingExceptions(): Transfer cancelled.");
		break;
	case LIBUSB_TRANSFER_STALL:
		// For bulk/interrupt endpoints: halt condition detected (endpoint stalled).
		// For control endpoints: control request not supported.
		/// \todo Add optional/configurable exceptions
		// throw std::runtime_error("LibUSB::TransferImpl::HandlePendingExceptions(): Transfer stalled.");
		break;

	case LIBUSB_TRANSFER_NO_DEVICE:
		// Device was disconnected.
		/// \todo create a custom exception class for "No device"
		throw std::runtime_error("LibUSB::TransferImpl::HandlePendingExceptions(): No device.");
		break;
	case LIBUSB_TRANSFER_OVERFLOW:
		// Device sent more data than requested.
		/// \todo Add custom exception for overflow conditions.
		throw std::logic_error("LibUSB::TransferImpl::HandlePendingExceptions(): Overflow - device sent more data than requested.");
		break;

	default:

		throw std::logic_error("LibUSB::TransferImpl::HandlePendingExceptions(): Unknown transfer status.");
		break;
	}

}

LibUSB::TransferResult_t LibUSB::TransferImpl::Result() const
{

	if (!m_Complete)
	{
		std::logic_error("LibUSB::TransferImpl::Result(): Cannot get results from incomplete transfers.");
	}

	TransferResult_t result = FAILED;

	switch(m_pTransfer->status)
	{
	case LIBUSB_TRANSFER_COMPLETED:

		// Transfer completed without error.
		// Note that this does not indicate that the entire amount of requested data was transferred.
		result = TransferResult_t::COMPLETED;
		break;

	case LIBUSB_TRANSFER_ERROR:

		// Transfer failed.
		result = TransferResult_t::FAILED;
		break;

	case LIBUSB_TRANSFER_TIMED_OUT:

		// Transfer timed out.
		result = TransferResult_t::TIMED_OUT;
		break;

	case LIBUSB_TRANSFER_CANCELLED:

		// Transfer was canceled.
		result = TransferResult_t::CANCELLED;
		break;

	case LIBUSB_TRANSFER_STALL:

		// For bulk/interrupt endpoints: halt condition detected (endpoint stalled).
		// For control endpoints: control request not supported.
		result = TransferResult_t::STALLED;
		break;

	case LIBUSB_TRANSFER_NO_DEVICE:

		// Device was disconnected.
		result = TransferResult_t::NO_DEVICE;
		break;

	case LIBUSB_TRANSFER_OVERFLOW:

		// Device sent more data than requested.
		result = TransferResult_t::DATA_OVERFLOW;
		break;

	default:
		std::logic_error("LibUSB::TransferImpl::Result(): Internal error - unrecognized transfer status.");
		break;

	}

	return result;

}


std::map<LibUSB::TransferImpl*, std::weak_ptr<LibUSB::TransferImpl>> LibUSB::TransferImpl::m_TransferMap;


// **********************************
// Control Transfer Implementation
// **********************************

LibUSB::ControlTransferImpl::ControlTransferImpl(std::weak_ptr<EndpointImpl> pEndpointImpl)
	: TransferImpl(pEndpointImpl),
	m_RequestType(LIBUSB_REQUEST_TYPE_VENDOR),
	m_RequestRecipient(LIBUSB_RECIPIENT_ENDPOINT),
	m_DataTransferDirection(LIBUSB_ENDPOINT_OUT),
	m_Request(0),
	m_wValue(0),
	m_wIndex(0),
	m_Setup(false)
{


}

LibUSB::ControlTransferImpl::~ControlTransferImpl()
{

}

void LibUSB::ControlTransferImpl::Setup()
{
	if (!m_Setup)
	{
		// Cannot continue
		throw std::logic_error("LibUSB::ControlTransferImpl::Setup(): Control packet has not been filled out using SetupPacket().");
	}



	if (m_pTransfer.get() == nullptr)
	{

		// Create a libusb transfer object with no iso packets.
		AllocTransfer();

	}

	// Validate the buffer
	if (m_pTransferBuffer.get() == nullptr)
	{

		if ((m_transferSize > 0) && (m_DataTransferDirection == LIBUSB_ENDPOINT_OUT))
		{
			// You have to actually provide the data you want to send, you know.
			throw std::logic_error("void LibUSB::ControlTransferImpl::Setup(): Control transfer specifies data packets to be sent OUT, but no data buffer was provided!");

		}

		// Allocate a transfer buffer for the setup packet plus expected-data to be received.
		auto deleter=[&](unsigned char*ptr){ delete [] ptr; };

		m_pTransferBuffer.reset((unsigned char*)memset(new unsigned char [m_transferSize + LIBUSB_CONTROL_SETUP_SIZE], '\0', m_transferSize + LIBUSB_CONTROL_SETUP_SIZE), deleter);

	}


	// Setup a control transfer.
	libusb_fill_control_setup(m_pTransferBuffer.get(), m_RequestType|m_RequestRecipient|m_DataTransferDirection, m_Request, m_wValue, m_wIndex, m_transferSize);


	// Fill the transfer object
	libusb_fill_control_transfer(m_pTransfer.get(), m_pEndpointImpl.lock()->getDeviceImpl().lock()->m_pHandle.get(),
		m_pTransferBuffer.get(), AsyncTransferCallback, this, m_Timeout);

	// Done.
}

void LibUSB::ControlTransferImpl::SetupPacket( uint8_t Request, uint16_t wValue, uint16_t wIndex, DataTransferDirection_t transferDirection /*= HOST_TO_DEVICE */, RequestType_t requestType /*= REQ_VENDOR*/, RequestRecipient_t recipient /*= REC_ENDPOINT*/ )
{

	m_Request = Request;

	m_wValue = wValue;
	m_wIndex = wIndex;

	switch (transferDirection)
	{
	case DataTransferDirection_t::HOST_TO_DEVICE:
		m_DataTransferDirection = LIBUSB_ENDPOINT_OUT;
		break;
	case DataTransferDirection_t::DEVICE_TO_HOST:
		m_DataTransferDirection = LIBUSB_ENDPOINT_IN;
		break;
	default:
		throw std::logic_error("LibUSB::ControlTransferImpl::SetupPacket(): Invalid data transfer direction.");
		break;
	}
	switch (requestType)
	{

	case RequestType_t::REQ_STANDARD:
		m_RequestType = LIBUSB_REQUEST_TYPE_STANDARD;
		break;

	case RequestType_t::REQ_CLASS:
		m_RequestType = LIBUSB_REQUEST_TYPE_CLASS;
		break;

	case RequestType_t::REQ_VENDOR:
		m_RequestType = LIBUSB_REQUEST_TYPE_VENDOR;
		break;

	case RequestType_t::REQ_RESERVED:
		m_RequestType = LIBUSB_REQUEST_TYPE_RESERVED;
		break;

	default:
		throw std::logic_error("LibUSB::ControlTransferImpl::SetupPacket(): Invalid request type.");
		break;
	}

	switch (recipient)
	{
	case RequestRecipient_t::REC_DEVICE:
		m_RequestRecipient = LIBUSB_RECIPIENT_DEVICE;
		break;

	case RequestRecipient_t::REC_INTERFACE:
		m_RequestRecipient = LIBUSB_RECIPIENT_INTERFACE;
		break;

	case RequestRecipient_t::REC_ENDPOINT:
		m_RequestRecipient = LIBUSB_RECIPIENT_ENDPOINT;
		break;

	case RequestRecipient_t::REC_OTHER:
		m_RequestRecipient = LIBUSB_RECIPIENT_OTHER;
		break;

	default:
		throw std::logic_error("LibUSB::ControlTransferImpl::SetupPacket(): Invalid recipient type.");
		break;
	}

	/// Force user to setup every time
	m_Setup = true;
}

void LibUSB::ControlTransferImpl::Reset()
{

	TransferImpl::Reset();


	/// \todo Allow a user to retain setup information after a reset - to resend a command easily?

	m_Setup = false;
}



// **********************************
// Interrupt Transfer Implementation
// **********************************

LibUSB::InterruptTransferImpl::InterruptTransferImpl( std::weak_ptr<EndpointImpl> pEndpointImpl )
	: TransferImpl(pEndpointImpl)
{


}


LibUSB::InterruptTransferImpl::~InterruptTransferImpl()
{


}

void LibUSB::InterruptTransferImpl::Setup()
{


	if (m_pTransfer.get() == nullptr)
	{
		AllocTransfer();
	}

	// Validate the buffer
	if (m_pTransferBuffer.get() == nullptr)
	{

		if ((m_transferSize > 0))
		{

			// This configuration is not valid.
			throw std::logic_error("LibUSB::InterruptTransferImpl::Setup(): TransferBuffer has not been set, yet transfer size is > 0.");

		}

		// Expect to receive a zero-length packet.

	}

	libusb_fill_interrupt_transfer(	m_pTransfer.get(),
									m_pEndpointImpl.lock()->getDeviceImpl().lock()->m_pHandle.get(),
									m_pEndpointImpl.lock()->Address(),
									m_pTransferBuffer.get(),
									m_transferSize,
									AsyncTransferCallback,
									this,
									m_Timeout);

	// That was easy :-)

	// Done.
}

void LibUSB::InterruptTransferImpl::Reset()
{

	TransferImpl::Reset();

}


LibUSB::BulkTransferImpl::BulkTransferImpl( std::weak_ptr<EndpointImpl> pEndpointImpl )
	: TransferImpl(pEndpointImpl)
{


}

LibUSB::BulkTransferImpl::~BulkTransferImpl()
{


}

void LibUSB::BulkTransferImpl::Setup()
{

	if (m_pTransfer.get() == nullptr)
	{
		AllocTransfer();
	}

	// Validate the buffer
	if (m_pTransferBuffer.get() == nullptr)
	{

		if ((m_transferSize > 0))
		{

			// This configuration is not valid.
			throw std::logic_error("LibUSB::BulkTransferImpl::Setup(): TransferBuffer has not been set, yet transfer size is > 0.");

		}

		// Expect to receive a zero-length packet.

	}

	libusb_fill_bulk_transfer( m_pTransfer.get(),
		m_pEndpointImpl.lock()->getDeviceImpl().lock()->m_pHandle.get(),
		m_pEndpointImpl.lock()->Address(),
		m_pTransferBuffer.get(),
		m_transferSize,
		AsyncTransferCallback,
		this,
		m_Timeout);

	// That was easy :-)

	// Done.
}

LibUSB::IsochronousTransferImpl::IsochronousTransferImpl( std::weak_ptr<EndpointImpl> pEndpointImpl )
	: TransferImpl(pEndpointImpl), m_numPackets(0)
{


}

LibUSB::IsochronousTransferImpl::~IsochronousTransferImpl()
{


}

void LibUSB::IsochronousTransferImpl::Reset()
{

	TransferImpl::Reset();
}

void LibUSB::IsochronousTransferImpl::Setup()
{

	if (m_pTransfer.get() == nullptr)
	{
		AllocTransfer(m_numPackets);
	}

	// Validate the buffer
	if (m_pTransferBuffer.get() == nullptr)
	{

		if ((m_transferSize > 0))
		{

			// This configuration is not valid.
			throw std::logic_error("LibUSB::IsochronousTransferImpl::Setup(): TransferBuffer has not been set, yet transfer size is > 0.");

		}

		// Expect to receive a zero-length packet.

	}

	// Fill the transfer
	libusb_fill_iso_transfer( m_pTransfer.get(),
		m_pEndpointImpl.lock()->getDeviceImpl().lock()->m_pHandle.get(),
		m_pEndpointImpl.lock()->Address(),
		m_pTransferBuffer.get(),
		m_transferSize,
		m_numPackets,
		AsyncTransferCallback,
		this,
		m_Timeout);

	libusb_set_iso_packet_lengths(m_pTransfer.get(), m_transferSize/m_numPackets);


	// That wasn't as easy :-)

	// Done.


}

void LibUSB::IsochronousTransferImpl::setNumPackets( int numPackets )
{

	// Not allowed if a transfer is in progress.

	// A new transfer object has to be created
	if (numPackets != m_numPackets)
	{

		// Drop the transfer object.
		m_pTransfer.reset();
		m_numPackets = numPackets;

	}

}
