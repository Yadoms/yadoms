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

#include <libusbpp/Configuration.hpp>

#include "ConfigurationImpl.hpp"


LibUSB::Configuration::Configuration( std::shared_ptr<ConfigurationImpl> pInit )
{
	// Save the implementation object
	m_pConfigImpl_ = pInit;

}

LibUSB::Configuration::~Configuration()
{
	// Destroy Implementation
	m_pConfigImpl_.reset();
}

std::wstring LibUSB::Configuration::DescriptorString()const
{

	// Obtain the config descriptor string
	return m_pConfigImpl_->DescriptorString();

}

uint8_t LibUSB::Configuration::Value() const
{
	return m_pConfigImpl_->getValue();
}

int LibUSB::Configuration::MaxPower() const
{
	return (int)m_pConfigImpl_->getMaxPower() * 2;
}

const unsigned char * LibUSB::Configuration::getExtraDescriptors() const
{
	return m_pConfigImpl_->getExtraDescriptors();
}

int LibUSB::Configuration::getExtraDescriptorSize() const
{
	return m_pConfigImpl_->getExtraDescriptorSize();
}

void LibUSB::Configuration::SetAsActive()
{
	m_pConfigImpl_->SetAsActive();
}

bool LibUSB::Configuration::hasExtraDescriptors() const
{
	return m_pConfigImpl_->hasExtraDescriptors();
}

bool LibUSB::Configuration::supportsRemoteWakeup() const
{
	return m_pConfigImpl_->supportsRemoteWakeup();
}

bool LibUSB::Configuration::isSelfPowered() const
{
	return m_pConfigImpl_->isSelfPowered();
}

int LibUSB::Configuration::NumInterfaces() const
{
	return m_pConfigImpl_->NumInterfaces();
}

std::shared_ptr<LibUSB::Interface> LibUSB::Configuration::getInterfaceByIndex( int index ) const
{
	return m_pConfigImpl_->getInterfaceByIndex(index);
}

std::shared_ptr<LibUSB::Interface> LibUSB::Configuration::getInterface( int InterfaceNumber ) const
{
	return m_pConfigImpl_->getInterface(InterfaceNumber);
}

