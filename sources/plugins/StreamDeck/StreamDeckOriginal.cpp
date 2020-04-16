#include "StreamDeckOriginal.h"
#include "DeviceManagerHelper.h"
#include <opencv2/opencv.hpp>
#include "ImageHelper.h"
#include <algorithm>

const int CStreamDeckOriginal::ImageReportLength = 8191;
const int CStreamDeckOriginal::KeyCols = 5;
const int CStreamDeckOriginal::KeyRows = 3;
const int CStreamDeckOriginal::KeyCount = KeyCols * KeyRows;
const int CStreamDeckOriginal::DataToSendLength = 17;
const int CStreamDeckOriginal::KeyPixelSize = 72;

CStreamDeckOriginal::CStreamDeckOriginal(CConfiguration& configuration, shared::event::CEventHandler& mainEventHandler, int evtKeyStateReceived)
	: CDeviceManager(configuration, mainEventHandler, evtKeyStateReceived)
{
}

CStreamDeckOriginal::~CStreamDeckOriginal() = default;

void CStreamDeckOriginal::reset()
{
	resetKeyStream();
	unsigned char payload[DataToSendLength];
	payload[0] = 0x0b;
	payload[1] = 0x63;

	hid_send_feature_report(m_handle, payload, DataToSendLength);
}

void CStreamDeckOriginal::resetKeyStream()
{
	unsigned char payload[ImageReportLength] = {0x02};
	hid_write(m_handle, payload, ImageReportLength);
}

void CStreamDeckOriginal::setBrightness(int percent)
{

	// TODO : Create a specific function to convert int to hex
	auto percenut = CDeviceManagerHelper::decimalToHex(std::to_string(percent));
	unsigned char payload[17] = {
		0x05, 0x55, 0xaa, 0xd1, 0x01, percenut
	};
	//unsigned char payload[17] = { 0x05, 0x55, 0xaa, 0xd1, 0x01, percent };
	hid_send_feature_report(m_handle, payload, DataToSendLength);
}

void CStreamDeckOriginal::setKeyImage(std::string& content, int& keyIndex, std::string& customText)
{
	// TODO : Pass text from configuration
	auto data = CImageHelper::stringToVector(content);
	auto img = CImageHelper::renderKeyImage(data, KeyPixelSize, customText);

	auto array = CImageHelper::cvMatToVector(img);

	std::vector<unsigned char> bmp;
	std::vector<unsigned char> header;

	CImageHelper::encodeBMP(bmp, &array[0], 72, 72);
	const int imageReportPayloadLength = bmp.size() / 2;
	// TODO : Handle keys
	const auto key = convertKeyIdOrigin(keyIndex);
	auto pageNumber = 0;
	int bytesRemaining = bmp.size();

	while (bytesRemaining > 0)
	{
		const auto thisLength = std::min(bytesRemaining, imageReportPayloadLength);
		const auto bytesSent = pageNumber * imageReportPayloadLength;
		// Report ID for writing an image
		header.push_back(0x02);
		// Unknown but never changes
		header.push_back(0x01);
		// "Page" number. Each image is sent as 2 pages.
		header.push_back(pageNumber + 1);
		// This is padding
		header.push_back(0);
		//  This is 01 on the second page, Presumably its used to signal that the data is a continuation from the first page
		header.push_back(thisLength == bytesRemaining ? 1 : 0);
		// Hex value for the Button
		header.push_back(key + 1);
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

		hid_write(m_handle, &datatosend[0], ImageReportLength);

		bytesRemaining = bytesRemaining - thisLength;
		pageNumber++;
		header.clear();
		tmp.clear();
		payload.clear();
		datatosend.clear();
	}
}

int CStreamDeckOriginal::convertKeyIdOrigin(int& keyIndex)
{
	const auto keyCol = keyIndex % KeyCols;
	return (keyIndex - keyCol) + ((KeyCols - 1) - keyCol);
}

std::pair<bool, int> CStreamDeckOriginal::readKeyStates()
{
	unsigned char readData[DataToSendLength] = {};
	// First byte is the report ID
	const auto offset = 1;
	// TODO: ajouter mutex ? (ou vérifier que la lib est thread-safe)
	// in case of read error stop thread
	hid_read(m_handle, readData, DataToSendLength);

	const auto readDataVector = CDeviceManagerHelper::unsignedCharToVectorOfUnsignedChar(readData, offset, DataToSendLength);

	return CDeviceManagerHelper::findInVector<unsigned char>(readDataVector, 1);

}
