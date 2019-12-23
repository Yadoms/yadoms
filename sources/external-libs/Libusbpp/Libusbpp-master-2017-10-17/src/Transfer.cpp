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

#include <libusbpp/Transfer.hpp>

#include "TransferImpl.hpp"


LibUSB::Transfer::Transfer( std::shared_ptr<LibUSB::TransferImpl> pTransferImpl )
	: m_pTransferImpl(pTransferImpl), m_AsynchronousTransferPending(false)
{

}



LibUSB::Transfer::~Transfer()
{
	if (m_AsynchronousTransferPending)
	{

		// Cancel the transfer, then wait.
//		Cancel();

//		WaitForCompletion();

	}
}

void LibUSB::Transfer::SetTimeout( std::chrono::milliseconds timeout )
{


	if (m_AsynchronousTransferPending)
	{

		// Cannot change timeout right now.
		throw std::logic_error("LibUSB::Transfer::SetTimeout(): Method cannot be called while an asynchronous transfer is pending.");

	}

	m_pTransferImpl->setTimeout(timeout);

}

void LibUSB::Transfer::setTransferBuffer( std::shared_ptr<unsigned char> pBuffer, size_t bufferSize )
{

	if (m_AsynchronousTransferPending)
	{

		// Absolutely not.
		throw std::logic_error("LibUSB::Transfer::setTransferBuffer(): Method cannot be called while an asynchronous transfer is pending.");

	}

	m_pTransferImpl->setBuffer(pBuffer);

	m_pTransferImpl->setDataTransferSize(bufferSize);

}

std::shared_ptr<unsigned char> LibUSB::Transfer::getTransferBuffer()
{

	if (m_AsynchronousTransferPending)
	{

		// No way.
		throw std::logic_error("LibUSB::Transfer::getTransferBuffer(): Method cannot be called while an asynchronous transfer is pending.");

	}

	return m_pTransferImpl->getBuffer();

}

void LibUSB::Transfer::Init()
{

	if (m_AsynchronousTransferPending)
	{

		// Cannot change stuff right now.
		throw std::logic_error("LibUSB::Transfer::Init(): Method cannot be called while an asynchronous transfer is pending.");

	}
	m_pTransferImpl->init(shared_from_this());

}

void LibUSB::Transfer::Start()
{

	if (m_AsynchronousTransferPending)
	{

		// What?  Maybe it would be proper to block, instead?
		throw std::logic_error("LibUSB::Transfer::Start(): Method cannot be called while an asynchronous transfer is pending.");

	}

	try
	{
		m_pTransferImpl->Start();
	}
	catch (...)
	{
		throw;

	}

}

void LibUSB::Transfer::Cancel()
{

	m_pTransferImpl->Cancel();

}

bool LibUSB::Transfer::isComplete()
{

	if (m_AsynchronousTransferPending)
	{

		// Check the status of the thread.
		if (*m_TransferThreadRunning)
		{
			// Nothing to see here, move along.
			return false;
		}
		else
		{
			return WaitForCompletion();
		}

	}



	return m_pTransferImpl->isComplete();

}

size_t LibUSB::Transfer::BytesTransferred() const
{

	if (m_AsynchronousTransferPending)
	{

		// Cannot change this right now.
		throw std::logic_error("LibUSB::Transfer::BytesTransferred(): Method cannot be called while an asynchronous transfer is pending.");

	}

	return m_pTransferImpl->bytesTransferred();

}

void LibUSB::Transfer::Reset()
{

	if (m_AsynchronousTransferPending)
	{

		// Cannot change this right now.
		throw std::logic_error("LibUSB::Transfer::Reset(): Method cannot be called while an asynchronous transfer is pending.");

	}

	m_pTransferImpl->Reset();

}

bool LibUSB::Transfer::isSuccessful() const
{

	if (m_AsynchronousTransferPending)
	{

		// Cannot change this right now.
		throw std::logic_error("LibUSB::Transfer::isSuccessful(): Method cannot be called while an asynchronous transfer is pending.");

	}

	return m_pTransferImpl->isSuccessful();
}

void LibUSB::Transfer::AsyncStart()
{

	if (m_AsynchronousTransferPending)
	{

		// Cannot change this right now.
		throw std::logic_error("LibUSB::Transfer::AsyncStart(): Method cannot be called while an asynchronous transfer is already in progress!");

	}

	// Set the transfer-in-progress flag.
	m_AsynchronousTransferPending = true;

	// Create a promise that will be used to return our copy of the Transfer Implmentation
	auto TransferPromise = std::make_shared<std::promise<std::shared_ptr<LibUSB::TransferImpl>>>();

	m_TransferFuture = TransferPromise->get_future();

	auto pPromisePending = std::make_shared<bool>(true);

	// Share a boolean as a signal.
	m_TransferThreadRunning = pPromisePending;

	std::shared_ptr<LibUSB::TransferImpl> pTransferImplementation = m_pTransferImpl;

	std::thread transferThread([=]()
				{
					bool Result = false;

					try
					{
						pTransferImplementation->Start();
						TransferPromise->set_value(pTransferImplementation);
					}
					catch (...)
					{
						TransferPromise->set_exception(std::current_exception());

					}

					*pPromisePending = false;

				});

	// Let it complete.
	transferThread.detach();

}

bool LibUSB::Transfer::WaitForCompletion()
{

	if (m_TransferFuture.valid())
	{

		// throw std::logic_error("LibUSB::Transfer::WaitForCompletion(): Invalid future.");
		m_pTransferImpl = m_TransferFuture.get();

	}

	m_AsynchronousTransferPending = false;

	return m_pTransferImpl->isComplete();

}

LibUSB::TransferResult_t LibUSB::Transfer::Result() const
{

	if (m_AsynchronousTransferPending)
	{
		/// \todo Add a "PENDING" or "IN_PROGRESS" result?

		// Cannot change the data right now.
		throw std::logic_error("LibUSB::Transfer::Result(): Method cannot be called while an asynchronous transfer is in progress!");

	}

	return m_pTransferImpl->Result();
}


void LibUSB::ControlTransfer::SetupPacket( uint8_t Request, uint16_t wValue, uint16_t wIndex, DataTransferDirection_t transferDirection /*= HOST_TO_DEVICE */, RequestType_t requestType /*= REQ_VENDOR*/, RequestRecipient_t recipient /*= REC_ENDPOINT*/ )
{

	if (m_AsynchronousTransferPending)
	{

		// Cannot change the data right now.
		throw std::logic_error("LibUSB::ControlTransfer::SetupPacket(): Method cannot be called while an asynchronous transfer is in progress!");

	}

	std::static_pointer_cast<ControlTransferImpl>(m_pTransferImpl)->SetupPacket(Request, wValue, wIndex, transferDirection, requestType, recipient);


}

LibUSB::ControlTransfer::ControlTransfer( std::shared_ptr<TransferImpl> pTransferImpl )
	: Transfer(pTransferImpl)
{


}

LibUSB::ControlTransfer::~ControlTransfer()
{

}

LibUSB::InterruptTransfer::InterruptTransfer( std::shared_ptr<TransferImpl> pTransferImpl )
	: Transfer(pTransferImpl)
{


}

LibUSB::InterruptTransfer::~InterruptTransfer()
{

	/// \todo Cancel if a transfer is in progress.
}

LibUSB::BulkTransfer::BulkTransfer( std::shared_ptr<TransferImpl> pTransferImpl )
	: Transfer(pTransferImpl)
{


}

LibUSB::BulkTransfer::~BulkTransfer()
{

		/// \todo Cancel if a transfer is in progress.

}

LibUSB::IsochronousTransfer::IsochronousTransfer( std::shared_ptr<LibUSB::TransferImpl> pTransferImpl )
	:LibUSB::Transfer(pTransferImpl)
{


}

LibUSB::IsochronousTransfer::~IsochronousTransfer()
{


}

void LibUSB::IsochronousTransfer::setNumPackets( int Packets )
{

	if (Packets < 0)
	{
		throw std::range_error("LibUSB::IsochronousTransfer::setNumPackets(): Cannot set number of packets to value below zero.");
	}

	std::static_pointer_cast<IsochronousTransferImpl>(m_pTransferImpl)->setNumPackets(Packets);
}
