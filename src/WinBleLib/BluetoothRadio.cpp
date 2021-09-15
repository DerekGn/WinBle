/*
MIT License

Copyright(c) Derek Goslin 2021

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
#include <bluetoothapis.h>

#include "Utility.h"
#include "BluetoothRadio.h"
#include "WinBleException.h"

#include <sstream>

using namespace std;

void BluetoothRadio::getRadioInformation()
{
	if (BluetoothGetRadioInfo(_hBluetoothRadio, &_radio_info) != ERROR_SUCCESS)
	{
		Util.throwLastErrorException("Unable to get handle to bluetooth radio info.");
	}
}

BluetoothRadio::BluetoothRadio(HANDLE hBluetoothRadio)
{
	if (hBluetoothRadio == NULL)
	{
		throw WinBleException("Handle cannot be NULL");
	}

	_hBluetoothRadio = hBluetoothRadio;

	getRadioInformation();
}

BluetoothRadio::~BluetoothRadio()
{
	if (_hBluetoothRadio != NULL)
	{
		CloseHandle(_hBluetoothRadio);
	}
}

BLUETOOTH_ADDRESS BluetoothRadio::getAddress()
{
	return _radio_info.address;
}

wstring BluetoothRadio::getName()
{
	return _radio_info.szName;
}

ULONG BluetoothRadio::getClassOfDevice()
{
	return _radio_info.ulClassofDevice;
}

USHORT BluetoothRadio::getLmpSubVersion()
{
	return _radio_info.lmpSubversion;
}

USHORT BluetoothRadio::getManufacturer()
{
	return _radio_info.manufacturer;
}
