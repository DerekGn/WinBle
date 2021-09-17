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

#include "BleFunctions.h"
#include "BleException.h"
#include "Utility.h"

#include <windows.h>
#include <Setupapi.h>
#include <BluetoothAPIs.h>

#pragma comment(lib, "Rpcrt4")
#pragma comment(lib, "SetupAPI")
#pragma comment(lib, "BluetoothAPIs")

#include <sstream>

GUID mapServiceUUID(const PBTH_LE_UUID serviceUUID)
{
	if (serviceUUID->IsShortUuid)
	{
		return { serviceUUID->Value.ShortUuid, 0x0000, 0x1000, { 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB } };
	}
	else
	{
		return serviceUUID->Value.LongUuid;
	}
}

HANDLE openBleInterfaceHandle(GUID interfaceUUID, DWORD dwDesiredAccess)
{
	HDEVINFO hDI;
	SP_DEVICE_INTERFACE_DATA did{};
	SP_DEVINFO_DATA dd{};
	GUID BluetoothInterfaceGUID = interfaceUUID;
	HANDLE handle = nullptr;

	if ((hDI = SetupDiGetClassDevs(&BluetoothInterfaceGUID, nullptr, nullptr, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT)) == INVALID_HANDLE_VALUE)
	{
		stringstream stream;
		stream << "Unable to open device information set for device interface UUID: ["
			<< Utility::guidToString(BluetoothInterfaceGUID) << "]";

		Utility::throwLastErrorException(stream.str());

	}

	did.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	dd.cbSize = sizeof(SP_DEVINFO_DATA);

	for (DWORD i = 0; SetupDiEnumDeviceInterfaces(hDI, nullptr, &BluetoothInterfaceGUID, i, &did); i++)
	{
		SP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData{};

		DeviceInterfaceDetailData.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		DWORD size = 0;

		if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, nullptr, 0, &size, nullptr))
		{
			int err = GetLastError();

			if (err == ERROR_NO_MORE_ITEMS) break;

			auto pInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(GPTR, size);

			if (pInterfaceDetailData != nullptr)
			{
				pInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

				if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, pInterfaceDetailData, size, &size, &dd))
					break;

				handle = CreateFile(
					pInterfaceDetailData->DevicePath,
					dwDesiredAccess,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					nullptr,
					OPEN_EXISTING,
					0,
					nullptr);

				GlobalFree(pInterfaceDetailData);

				if (handle == INVALID_HANDLE_VALUE)
				{
					stringstream stream;
					stream << "Unable to file handle for interface UUID: ["
						<< Utility::guidToString(BluetoothInterfaceGUID) << "]";

					throw BleException(stream.str());
				}
			}
			else
			{
				throw BleException("Unable to allocate device interface detail data");
			}
		}
	}

	SetupDiDestroyDeviceInfoList(hDI);
	return handle;
}
