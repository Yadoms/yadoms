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

#ifndef LIBUSBPP_TRANSFER_HPP
#define LIBUSBPP_TRANSFER_HPP

#include <memory>
#include <chrono>
#include <future>

#include <libusbpp/TransferDefs.hpp>


namespace LibUSB
{
	class TransferImpl;

	/// USB Data transfer object.
	class Transfer : public std::enable_shared_from_this<Transfer>
	{

	public:

		/// Constructor
		Transfer(std::shared_ptr<TransferImpl> pTransferImpl);

		/// Destructor
		virtual ~Transfer();

		/// Returns TRUE if the transfer is complete.
		bool isComplete();

		/// Returns TRUE if the transfer isSuccessful.  (Transfer MUST be complete, throws any waiting exceptions)
		bool isSuccessful()const;

		/// Sets the amount of data to transfer in/out from the buffer.
		void setTransferBuffer(std::shared_ptr<unsigned char> pBuffer, size_t transferSize);

		/// Returns transfer buffer
		std::shared_ptr<unsigned char> getTransferBuffer();

		/// Returns the amount of data written/read to/from the buffer.
		size_t BytesTransferred()const;

		/// Sets the timeout period for the transfer (0 denotes infinity)
		void SetTimeout(std::chrono::milliseconds timeout);

		/// Starts the transfer
		virtual void Start();

		/// Starts an asynchronous transfer
		void AsyncStart();

		/// Stops/Cancels the transfer
		virtual void Cancel();

		/// Resets the transfer to be used again.
		virtual void Reset();

		/// Returns the result of the transfer
		TransferResult_t Result()const;

		/// Initializes the object after construction is completed.
		virtual void Init();

		/// Waits until the Transfer is complete
		bool WaitForCompletion();

		/// Resolves Completion






	protected:

		/// Transfer Implementation
		std::shared_ptr<TransferImpl> m_pTransferImpl;

		/// Result of the Transfer (TransferImpl)
		std::future<std::shared_ptr<LibUSB::TransferImpl>> m_TransferFuture;

		/// Asynchronous Transfer Flag (true once the thread is started.)
		bool m_AsynchronousTransferPending;

		/// Indicates that the asynchronous operation is still running
		std::shared_ptr<bool> m_TransferThreadRunning;

	};

	/// USB Control transfer object
	class ControlTransfer : public Transfer
	{
	public:
		ControlTransfer(std::shared_ptr<TransferImpl> pTransferImpl);
		virtual ~ControlTransfer();

		/// USB Control transfer setup method.
		virtual void SetupPacket(uint8_t Request, uint16_t wValue, uint16_t wIndex, DataTransferDirection_t dataDirection = HOST_TO_DEVICE, RequestType_t requestType = REQ_VENDOR, RequestRecipient_t recipient = REC_ENDPOINT);

	protected:
	private:
	};

	/// USB Interrupt Transfer object
	class InterruptTransfer : public Transfer
	{
	public:

		InterruptTransfer(std::shared_ptr<TransferImpl> pTransferImpl);
		virtual ~InterruptTransfer();


	};

	/// USB Bulk Transfer object
	class BulkTransfer : public Transfer
	{
	public:
		BulkTransfer(std::shared_ptr<TransferImpl> pTransferImpl);
		virtual ~BulkTransfer();

	protected:
	private:
	};

	/// USB Isochronous Transfer object
	class IsochronousTransfer : public Transfer
	{
	public:
		IsochronousTransfer(std::shared_ptr<TransferImpl> pTransferImpl);
		virtual ~IsochronousTransfer();

		/// Sets the number of packets
		void setNumPackets(int Packets);
	protected:
	private:
	};
}

#endif // LIBUSBPP_TRANSFER_HPP
