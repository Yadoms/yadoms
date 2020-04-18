#include "StreamDeckOriginalV2.h"
#include "DeviceManagerHelper.h"
#include "ImageHelper.h"

const int CStreamDeckOriginalV2::ImageReportLength = 1024;
const int CStreamDeckOriginalV2::KeyCols = 5;
const int CStreamDeckOriginalV2::KeyRows = 3;
const int CStreamDeckOriginalV2::KeyCount = KeyCols * KeyRows;
const int CStreamDeckOriginalV2::DataToSendLength = 32;
const int CStreamDeckOriginalV2::KeyPixelSize = 72;
const int CStreamDeckOriginalV2::ImageReportHeaderLength = 8;
const int CStreamDeckOriginalV2::ImageReportPayloadLength = ImageReportLength - ImageReportHeaderLength;

CStreamDeckOriginalV2::CStreamDeckOriginalV2(CConfiguration& configuration,
                                             shared::event::CEventHandler& mainEventHandler, int evtKeyStateReceived)
	: CDeviceManager(configuration, mainEventHandler, evtKeyStateReceived)
{
}

CStreamDeckOriginalV2::~CStreamDeckOriginalV2() = default;

void CStreamDeckOriginalV2::reset()
{
	unsigned char payload[DataToSendLength];
	payload[0] = 0x03;
	payload[1] = 0x02;
	const auto res = hid_send_feature_report(m_handle, payload, DataToSendLength);
	
	if (res == CDeviceManagerHelper::EHidError::kTrue)
	{
		hid_close(m_handle);
		hid_exit();
		throw std::runtime_error("HID ERROR : Unable to send hid feature report when resetting device");
	}
}

void CStreamDeckOriginalV2::resetKeyStream()
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

void CStreamDeckOriginalV2::setBrightness(int percent)
{
	const auto percentInUnsignedCharValue = CDeviceManagerHelper::integerToHex(percent);
	unsigned char payload[DataToSendLength] = {0x03, 0x08, percentInUnsignedCharValue};
	const auto res = hid_send_feature_report(m_handle, payload, DataToSendLength);

	if (res == CDeviceManagerHelper::EHidError::kTrue)
	{
		hid_close(m_handle);
		hid_exit();
		throw std::runtime_error("HID ERROR : Unable to send hid feature report when setting brightness");
	}
}

void CStreamDeckOriginalV2::setKeyImage(std::string& content, int& keyIndex, std::string& customText)
{
	auto data = CImageHelper::stringToVector(content);
	auto img = CImageHelper::renderKeyImage(data, KeyPixelSize, customText);

	auto array = CImageHelper::cvMatToVector(img);
	auto jpegBuffer = CImageHelper::encodeCvMatVectorToJpegFormat(array);
	
	auto pageNumber = 0;
	int bytesRemaining = jpegBuffer.size();

	std::vector<unsigned char> header;
	
	while (bytesRemaining > 0)
	{
		const auto thisLength = std::min(bytesRemaining, ImageReportPayloadLength);
		const auto bytesSent = pageNumber * ImageReportPayloadLength;
		// Report ID for writing an image
		header.push_back(0x02);
		header.push_back(0x07);
		header.push_back(keyIndex);
		header.push_back(thisLength == bytesRemaining ? 1 : 0);
		header.push_back(thisLength & 0xff);
		header.push_back(thisLength >> 8);
		header.push_back(pageNumber & 0xff);
		header.push_back(pageNumber >> 8);
	
		auto payload(header);

		std::vector<unsigned char> tmp(jpegBuffer.begin() + bytesSent, jpegBuffer.begin() + thisLength + bytesSent);

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

std::pair<bool, int> CStreamDeckOriginalV2::readKeyStates()
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
