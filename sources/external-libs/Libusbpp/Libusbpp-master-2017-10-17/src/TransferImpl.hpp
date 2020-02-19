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

#ifndef __LIBUSBPP_TRANSFER_IMPL_HPP
#define __LIBUSBPP_TRANSFER_IMPL_HPP

#include <memory>
#include <chrono>
#include <map>
#include <atomic>

#include <libusb.h>

#include <libusbpp/TransferDefs.hpp>
#include <libusbpp/Transfer.hpp>

#include "EndpointImpl.hpp"


namespace LibUSB
{

	class TransferDeleter
	{
	public:
		void operator()(libusb_transfer* pTransfer) { libusb_free_transfer(pTransfer); };

	};

	/// Base USB Transfer implementation.
	class TransferImpl : public std::enable_shared_from_this<TransferImpl>
	{
	public:

		/// Constructor
		TransferImpl(std::weak_ptr<EndpointImpl> pEndpointImpl);

		/// Destructor
		virtual ~TransferImpl();

		/// Set the transfer buffer.
		void setBuffer(std::shared_ptr<unsigned char> pTransferBuffer);

		/// Returns the transfer buffer
		std::shared_ptr<unsigned char> getBuffer();

		/// Sets the data transfer size
		void setDataTransferSize(size_t transferBytes);

		/// Returns the number of bytes transferred.
		size_t bytesTransferred()const;

		/// Initializes the pointer to the parent Transfer object
		void init(std::weak_ptr<Transfer> pTransferParent);

		/// Start the transfer
		/// \todo Add timeout support.
		virtual void Start();

		/// Start the transfer, with communication flags.
//		void Start(std::shared_ptr<std::atomic_flag> );

		/// Cancel the transfer
		virtual void Cancel();

		/// Resets the transfer for reuse
		virtual void Reset();

		/// Returns TRUE if the transfer is in progress
		bool isRunning()const;

		/// Returns TRUE if the transfer is complete
		bool isComplete()const;

		/// Returns TRUE if completed successfully, throws pending exceptions
		bool isSuccessful()const;

		/// Returns the results of the transfer
		TransferResult_t Result()const;

		/// Set timeout
		void setTimeout(std::chrono::milliseconds timeout);

	protected:

		/// Fill the transfer object
		virtual void Setup() = 0;

		/// Create libusb transfer struct
		void AllocTransfer(int iso_packets = 0);

		/// Transfer Timeout
		unsigned int m_Timeout;

		/// Transfer buffer
		std::shared_ptr<unsigned char> m_pTransferBuffer;

		/// Transfer buffer size
		size_t m_transferSize;

		/// LibUSB transfer struct
		std::shared_ptr<libusb_transfer> m_pTransfer;

		/// Endpoint this transfer belongs to.
		std::weak_ptr<EndpointImpl> m_pEndpointImpl;

		/// Libusb asynchronous transfer callback function
		static void
#if defined(_WIN32)
		__stdcall
#endif
		AsyncTransferCallback(libusb_transfer* pTransfer);


		/// Notifies the device that the usb transfer is completed.
		virtual void NotifyComplete();

		/// Throws pending exceptions
		virtual void HandlePendingExceptions()const;

	private:


		/// Weak pointers to all active transfer objects
		static std::map<TransferImpl*, std::weak_ptr<TransferImpl>> m_TransferMap;

		/// Pointer to the Parent Transfer object
		std::weak_ptr<Transfer> m_pParentTransfer;

		/// Indicates that the transfer is complete
		std::atomic<bool> m_Complete;

		/// Synchronous completion flag.
		int m_eventCompleted;

		/// Indicates that the transfer has been submitted.
		bool m_Submitted;



	};





	// Control Transfer Implementation
	class ControlTransferImpl : public TransferImpl
	{
	public:

		ControlTransferImpl(std::weak_ptr<EndpointImpl> pEndpointImpl);
		virtual ~ControlTransferImpl();

		/// Control transfer setup information
		virtual void SetupPacket(uint8_t Request, uint16_t wValue, uint16_t wIndex, DataTransferDirection_t transferDirection /* = HOST_TO_DEVICE */, RequestType_t requestType /*= REQ_VENDOR*/, RequestRecipient_t recipient /*= REC_ENDPOINT*/ );

		/// Resets the transfer object for reuse.
		virtual void Reset();
	protected:

		virtual void Setup();

	private:

		uint8_t m_RequestType;

		uint8_t m_RequestRecipient;

		uint8_t m_DataTransferDirection;

		uint8_t m_Request;

		uint16_t m_wValue;

		uint16_t m_wIndex;

		/// Indicates that setup has been performed.
		bool m_Setup;

	};

	// Isochronous Transfer Implementation
	class IsochronousTransferImpl : public TransferImpl
	{
	public:

		IsochronousTransferImpl(std::weak_ptr<EndpointImpl> pEndpointImpl);
		virtual ~IsochronousTransferImpl();

		/// Resets the transfer object for reuse.
		virtual void Reset();

		/// Sets the number of isochronous packets
		virtual void setNumPackets(int numPackets);

	protected:

		virtual void Setup();

	private:

		int m_numPackets;
	};

	// Interrupt Transfer Implementation
	class InterruptTransferImpl : public TransferImpl
	{
	public:

		InterruptTransferImpl(std::weak_ptr<EndpointImpl> pEndpointImpl);
		virtual ~InterruptTransferImpl();

		/// Resets the transfer object for reuse.
		virtual void Reset();

	protected:

		virtual void Setup();

	private:
	};

	// Bulk Transfer Implementation
	class BulkTransferImpl : public TransferImpl
	{
	public:

		BulkTransferImpl(std::weak_ptr<EndpointImpl> pEndpointImpl);
		virtual ~BulkTransferImpl();

	protected:

		virtual void Setup();

	private:
	};

}

#endif // __LIBUSBPP_TRANSFER_IMPL_HPP
