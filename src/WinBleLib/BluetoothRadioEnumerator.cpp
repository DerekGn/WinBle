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
#include <bthdef.h>
#include <devguid.h>
#include <setupapi.h>
#include <bluetoothapis.h>

#include "Utility.h"
#include "BluetoothRadio.h"
#include "WinBleException.h"
#include "BluetoothRadioEnumerator.h"

#include <sstream>

using namespace std;

BluetoothRadioEnumerator RadioEnumerator;

BluetoothRadioEnumerator::BluetoothRadioEnumerator()
{
}

BluetoothRadioEnumerator::~BluetoothRadioEnumerator()
{
	_enumeratedRadios.clear();
}

void BluetoothRadioEnumerator::enumerate()
{
	_enumeratedRadios.clear();

	BLUETOOTH_FIND_RADIO_PARAMS find_radio_params{ sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
	HANDLE hRadio = nullptr;

	HBLUETOOTH_RADIO_FIND hfind = BluetoothFindFirstRadio(&find_radio_params, &hRadio);

	if (hfind != NULL)
	{
		_enumeratedRadios.push_back(new BluetoothRadio(hRadio));

		while (BluetoothFindNextRadio(hfind, &hRadio))
		{
			_enumeratedRadios.push_back(new BluetoothRadio(hRadio));
		}

		DWORD error = GetLastError();

		BluetoothFindDeviceClose(hfind);

		if (error != ERROR_SUCCESS && error != ERROR_NO_MORE_ITEMS)
		{
			Util.throwLastErrorException("Unable to get handle to bluetooth radio.");
		}
	}
	else
	{
		Util.throwLastErrorException("Unable to get handle to bluetooth find.");
	}
}

const BluetoothRadioEnumerator::BluetoothRadios& BluetoothRadioEnumerator::getBluetoothRadios()
{
	return _enumeratedRadios;
}
