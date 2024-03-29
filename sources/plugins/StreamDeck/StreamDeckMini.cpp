﻿#include "StreamDeckMini.h"
#include "DeviceManagerHelper.h"
#include "ImageHelper.h"

const int CStreamDeckMini::ImageReportLength = 1024;
const int CStreamDeckMini::KeyCols = 3;
const int CStreamDeckMini::KeyRows = 2;
const int CStreamDeckMini::KeyCount = KeyCols * KeyRows;
const int CStreamDeckMini::DataToSendLength = 17;
const int CStreamDeckMini::KeyPixelSize = 80;
const int CStreamDeckMini::ImageReportHeaderLength = 16;
const int CStreamDeckMini::ImageReportPayloadLength = ImageReportLength - ImageReportHeaderLength;

CStreamDeckMini::CStreamDeckMini(CConfiguration& configuration, shared::event::CEventHandler& mainEventHandler, int evtKeyStateReceived)
	: CDeviceManager(configuration, mainEventHandler, evtKeyStateReceived)
{
}

CStreamDeckMini::~CStreamDeckMini() = default;

void CStreamDeckMini::reset()
{
	unsigned char payload[DataToSendLength];
	payload[0] = 0x0B;
	payload[1] = 0x63;

	const auto res = hid_send_feature_report(m_handle, payload, DataToSendLength);

	if (res == CDeviceManagerHelper::EHidError::kTrue)
	{
		hid_close(m_handle);
		hid_exit();
		throw std::runtime_error("HID ERROR : Unable to send hid feature report when resetting device");
	}
}

void CStreamDeckMini::resetKeyStream()
{
	unsigned char payload[ImageReportLength] = {0x02};
	const auto res = hid_write(m_handle, payload, ImageReportLength);

	if (res == CDeviceManagerHelper::EHidError::kTrue)
	{
		hid_close(m_handle);
		hid_exit();
		throw std::runtime_error("HID ERROR : Unable to write an output report to a HID device when resetting key stream");
	}
}

void CStreamDeckMini::setBrightness(int percent)
{
	const auto percentInUnsignedCharValue = CDeviceManagerHelper::integerToHex(percent);
	
	unsigned char payload[DataToSendLength] = {0x05, 0x55, 0xaa, 0xd1, 0x01, percentInUnsignedCharValue};
	const auto res = hid_send_feature_report(m_handle, payload, DataToSendLength);

	if (res == CDeviceManagerHelper::EHidError::kTrue)
	{
		hid_close(m_handle);
		hid_exit();
		throw std::runtime_error("HID ERROR : Unable to send hid feature report when setting brightness");
	}
}


void CStreamDeckMini::setKeyImage(std::string content, int& keyIndex, std::string customText)
{
	auto data = CImageHelper::stringToVector(content);
	auto img = CImageHelper::renderKeyImage(data, KeyPixelSize, customText, cv::RotateFlags::ROTATE_90_CLOCKWISE);

	auto array = CImageHelper::cvMatToVector(img);

	std::vector<unsigned char> bmp;
	std::vector<unsigned char> header;

	CImageHelper::encodeBMP(bmp, &array[0], KeyPixelSize, KeyPixelSize);

	auto pageNumber = 0;
	int bytesRemaining = bmp.size();

	while (bytesRemaining > 0)
	{
		const auto thisLength = std::min(bytesRemaining, ImageReportPayloadLength);
		const auto bytesSent = pageNumber * ImageReportPayloadLength;
		// Report ID for writing an image
		header.push_back(0x02);
		// Unknown but never changes
		header.push_back(0x01);
		// "Page" number. Each image is sent as 2 pages.
		header.push_back(static_cast<unsigned char>(pageNumber + 1));
		// This is padding
		header.push_back(0);
		//  This is 01 on the second page, Presumably its used to signal that the data is a continuation from the first page
		header.push_back(thisLength == bytesRemaining ? 1 : 0);
		// Hex value for the Button
		header.push_back(static_cast<unsigned char>(keyIndex + 1));
		header.push_back(0);
		header.push_back(0);
		header.push_back(0);
		header.push_back(0);
		header.push_back(0);
		header.push_back(0);
		header.push_back(0);
		header.push_back(0);
		header.push_back(0);
		header.push_back(0);

		auto payload(header);

		std::vector<unsigned char> tmp(bmp.begin() + bytesSent, bmp.begin() + thisLength + bytesSent);

		payload.insert(payload.end(), tmp.begin(), tmp.end());

		std::vector<unsigned char> padding(ImageReportLength - payload.size());

		auto datatosend(payload);

		datatosend.insert(datatosend.end(), padding.begin(), padding.end());

		const auto res = hid_write(m_handle, &datatosend[0], ImageReportLength);

		if (res == CDeviceManagerHelper::EHidError::kTrue)
		{
			hid_close(m_handle);
			hid_exit();
			throw std::runtime_error("HID ERROR : Unable to write an output report to a HID device when setting key image");
		}

		bytesRemaining = bytesRemaining - thisLength;
		pageNumber++;
		header.clear();
		tmp.clear();
		payload.clear();
		datatosend.clear();
	}
}

std::pair<bool, int> CStreamDeckMini::readKeyStates()
{
	unsigned char readData[DataToSendLength] = {};
	// First byte is the report ID
	const auto offset = 1;
	// TODO: ajouter mutex ? (ou vérifier que la lib est thread-safe)
	// in case of read error stop thread
	std::mutex lock;
	lock.lock();

	const auto res = hid_read(m_handle, readData, DataToSendLength);

	if (res == CDeviceManagerHelper::EHidError::kTrue)
	{
		hid_close(m_handle);
		hid_exit();
		throw std::runtime_error("HID ERROR : Unable to read an Input report from a HID device when reading key states");
	}

	lock.unlock();

	const auto readDataVector = CDeviceManagerHelper::unsignedCharToVectorOfUnsignedChar(
		readData, offset, DataToSendLength);

	return CDeviceManagerHelper::findInVector<unsigned char>(readDataVector, 1);
}
