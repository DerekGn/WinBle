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

HANDLE getBleInterfaceHandle(GUID interfaceUUID, DWORD dwDesiredAccess, DWORD dwShareMode)
{
	HDEVINFO hDI;
	SP_DEVICE_INTERFACE_DATA did;
	SP_DEVINFO_DATA dd;
	HANDLE hComm = NULL;

	if ((hDI = SetupDiGetClassDevs(&interfaceUUID, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT)) == INVALID_HANDLE_VALUE)
	{
		stringstream msg;
		msg << "Unable to open device information set for device interface UUID: ["
			<< Util.guidToString(interfaceUUID) << "] Reason: ["
			<< Util.getLastError(GetLastError()) << "]";

		throw BleException(msg.str());
	}

	did.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	dd.cbSize = sizeof(SP_DEVINFO_DATA);

	DWORD i = 0;

	for (i = 0; SetupDiEnumDeviceInterfaces(hDI, NULL, &interfaceUUID, i, &did); i++)
	{
		SP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData;

		DeviceInterfaceDetailData.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		DWORD size = 0;

		if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, NULL, 0, &size, 0))
		{
			int err = GetLastError();

			if (err == ERROR_NO_MORE_ITEMS) break;

			PSP_DEVICE_INTERFACE_DETAIL_DATA pInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(GPTR, size);

			pInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, pInterfaceDetailData, size, &size, &dd))
			{
				GlobalFree(pInterfaceDetailData);
				break;
			}

			_wcsupr_s(pInterfaceDetailData->DevicePath, wcslen(pInterfaceDetailData->DevicePath) + 1);

			hComm = CreateFile(pInterfaceDetailData->DevicePath,
				dwDesiredAccess,
				dwShareMode,
				NULL,
				OPEN_EXISTING, 0, NULL);

			GlobalFree(pInterfaceDetailData);

			if (hComm == INVALID_HANDLE_VALUE)
			{
				stringstream msg;
				msg << "Unable to open device information service handle for interface UUID: ["
					<< Util.guidToString(interfaceUUID) << "] Reason: ["
					<< Util.getLastError(GetLastError()) << "]";

				throw BleException(msg.str());
			}

			GlobalFree(pInterfaceDetailData);
		}
	}

	SetupDiDestroyDeviceInfoList(hDI);

	return hComm;
}

void releaseBleInterfaceHandle(HANDLE hinterfaceHandle)
{
	if(hinterfaceHandle)
		CloseHandle(hinterfaceHandle);
}
