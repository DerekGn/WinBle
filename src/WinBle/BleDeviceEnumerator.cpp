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
#include <bthdef.h>
#include <devguid.h>
#include <setupapi.h>
#include <bluetoothleapis.h>

#include "BleDeviceEnumerator.h"
#include "BleException.h"
#include "Utility.h"

#include <sstream>
#include <memory>

using namespace std;

BleDeviceEnumerator BleEnumerator;

inline std::wstring& rtrim_null(std::wstring& s)
{
	s.erase(s.find_first_of(L'\0', 0));
	return s;
}

wstring BleDeviceEnumerator::getDeviceRegistryStringProperty(HDEVINFO hDI, SP_DEVINFO_DATA did, int property)
{
	wstring text;
	DWORD bufferSize = 0;

	while (!SetupDiGetDeviceRegistryProperty(
		hDI,
		&did,
		property,
		NULL,
		(PBYTE)&text[0],
		bufferSize,
		&bufferSize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			text.resize(bufferSize);
		else
			break;
	}

	return text;
}

BleDeviceEnumerator::BleDeviceEnumerator()
{
}

BleDeviceEnumerator::~BleDeviceEnumerator()
{
	bleEnumeratedDevices.clear();
}

void BleDeviceEnumerator::enumerate()
{
	bleEnumeratedDevices.clear();

	SP_DEVINFO_DATA did;
	DWORD i;

	HDEVINFO hDI = SetupDiGetClassDevs(&GUID_BLUETOOTHLE_DEVICE_INTERFACE, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

	if (INVALID_HANDLE_VALUE == hDI)
	{
		stringstream msg;
		msg << "Unable to get handle to device information elements. Reason: ["
			<< Util.getLastError(0) << "]";

		throw BleException(msg.str());
	}

	did.cbSize = sizeof(SP_DEVINFO_DATA);

	try
	{
		for (i = 0; SetupDiEnumDeviceInfo(hDI, i, &did); i++)
		{
			wstring name = getDeviceRegistryStringProperty(hDI, did, SPDRP_FRIENDLYNAME);

			wstring hwId = getDeviceRegistryStringProperty(hDI, did, SPDRP_HARDWAREID);

			wstring deviceInstanceId;
			DWORD bufferSize = 0;

			while (!SetupDiGetDeviceInstanceId(
				hDI,
				&did,
				&deviceInstanceId[0],
				bufferSize,
				&bufferSize))
			{
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
					deviceInstanceId.resize(bufferSize);
				else
					break;
			}

			if (name.size() > 0)
			{
				bleEnumeratedDevices.push_back(new BleDeviceInfo(rtrim_null(name), rtrim_null(hwId), rtrim_null(deviceInstanceId)));
			}
		}
	}
	catch (const std::exception&)
	{
		if (hDI)
			SetupDiDestroyDeviceInfoList(hDI);
	}
}

const BleDeviceEnumerator::BleDevices & BleDeviceEnumerator::getBleDevices()
{
	return bleEnumeratedDevices;
}