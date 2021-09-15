/*
MIT License

Copyright(c) Derek Goslin 2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <windows.h>
#include <Setupapi.h>
#include <BluetoothAPIs.h>

#pragma comment(lib, "Rpcrt4")
#pragma comment(lib, "SetupAPI")
#pragma comment(lib, "BluetoothAPIs")

#include "WinBleException.h"
#include "BleDevice.h"
#include "Utility.h"

#include <sstream>

using namespace std;

HANDLE BleDevice::getBleDeviceHandle(wstring deviceInstanceId)
{
	HDEVINFO hDI;
	SP_DEVICE_INTERFACE_DATA did{};
	SP_DEVINFO_DATA dd{};
	HANDLE hComm = NULL;

	hDI = SetupDiGetClassDevs(&GUID_BLUETOOTHLE_DEVICE_INTERFACE, deviceInstanceId.c_str(), NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

	if (hDI == INVALID_HANDLE_VALUE)
	{
		stringstream stream;
		stream << "Unable to open device information set for device interface UUID: ["
			<< Util.convertToString(deviceInstanceId) << "]";

		Util.throwLastErrorException(stream.str());
	}

	did.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	dd.cbSize = sizeof(SP_DEVINFO_DATA);

	DWORD i = 0;

	for (i = 0; SetupDiEnumDeviceInterfaces(hDI, NULL, &GUID_BLUETOOTHLE_DEVICE_INTERFACE, i, &did); i++)
	{
		SP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData{};

		DeviceInterfaceDetailData.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		DWORD size = 0;

		if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, NULL, 0, &size, 0))
		{
			int err = GetLastError();

			if (err == ERROR_NO_MORE_ITEMS) break;

			PSP_DEVICE_INTERFACE_DETAIL_DATA pInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(GPTR, size);

			if (pInterfaceDetailData != NULL)
			{
				pInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

				if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, pInterfaceDetailData, size, &size, &dd))
					break;

				hComm = CreateFile(
					pInterfaceDetailData->DevicePath,
					GENERIC_WRITE | GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					OPEN_EXISTING,
					0,
					NULL);

				GlobalFree(pInterfaceDetailData);
			}
			else
			{
				throw WinBleException("Unable to allocate device interface detail data");
			}
		}
	}

	SetupDiDestroyDeviceInfoList(hDI);

	if (i == 0)
	{
		stringstream msg;
		msg << "Device interface UUID: ["
			<< Util.convertToString(deviceInstanceId) << "] not found";

		throw WinBleException(msg.str());
	}

	return hComm;
}

PBTH_LE_GATT_SERVICE BleDevice::getGattServices(HANDLE _hBleDeviceHandle, USHORT * _pGattServiceCount)
{
	HRESULT hr = BluetoothGATTGetServices(
		_hBleDeviceHandle,
		0,
		NULL,
		_pGattServiceCount,
		BLUETOOTH_GATT_FLAG_NONE);

	if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr)
	{
		Util.throwLastErrorException("Unable to determine the number of gatt services.");
	}

	hr = BluetoothGATTGetServices(
		_hBleDeviceHandle,
		0,
		NULL,
		_pGattServiceCount,
		BLUETOOTH_GATT_FLAG_NONE);

	PBTH_LE_GATT_SERVICE pServiceBuffer = (PBTH_LE_GATT_SERVICE)
		malloc(sizeof(BTH_LE_GATT_SERVICE) * *_pGattServiceCount);

	if (!_pGattServiceCount)
		Util.handleMallocFailure(sizeof(BTH_LE_GATT_SERVICE) * *_pGattServiceCount);
	else
		RtlZeroMemory(pServiceBuffer, sizeof(BTH_LE_GATT_SERVICE) * *_pGattServiceCount);

	USHORT numServices;
	hr = BluetoothGATTGetServices(
		_hBleDeviceHandle,
		*_pGattServiceCount,
		pServiceBuffer,
		&numServices,
		BLUETOOTH_GATT_FLAG_NONE);

	if (S_OK != hr)
	{
		Util.throwLastErrorException("Unable to determine the number of gatt services.");
	}

	return pServiceBuffer;
}

BleDevice::BleDevice(wstring deviceInstanceId) : _hBleDevice(getBleDeviceHandle(deviceInstanceId)), _deviceContext(_hBleDevice, deviceInstanceId)
{
	_deviceInstanceId = deviceInstanceId;
}

BleDevice::~BleDevice()
{
	for (BleGattService *s : _bleGattServices)
		delete(s);

	if (_pGattServiceBuffer)
		free(_pGattServiceBuffer);

	if (_hBleDevice)
		CloseHandle(_hBleDevice);
}

wstring BleDevice::getDeviceIntstanceId()
{
	return _deviceInstanceId;
}

void BleDevice::enumerateBleServices()
{
	for (BleGattService *s : _bleGattServices)
		delete(s);

	_bleGattServices.clear();

	if (_pGattServiceBuffer)
		free(_pGattServiceBuffer);

	_pGattServiceBuffer = getGattServices(_hBleDevice, &_gattServiceCount);

	for (size_t i = 0; i < _gattServiceCount; i++)
		_bleGattServices.push_back(new BleGattService(_deviceContext, &_pGattServiceBuffer[i]));
}

const BleDevice::BleGattServices & BleDevice::getBleGattServices()
{	
	return _bleGattServices;
}
