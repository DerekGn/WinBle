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

#ifndef BLERADIOS_H
#define BLERADIOS_H

#include <Windows.h>

class BluetoothRadio
{
	private:
		HANDLE _hBluetoothRadio = nullptr;

		BLUETOOTH_RADIO_INFO _radio_info{ sizeof(BLUETOOTH_RADIO_INFO)};

		void getRadioInformation();

	public:
		/// <summary>
		/// Constructs an instance of a <see cref="BluetoothRadio"/>
		/// </summary>
		/// <param name="hBluetoothRadio">The bluetooth radio handle</param>
		BluetoothRadio(HANDLE hBluetoothRadio);

		/// <summary>
		/// Destructor
		/// </summary>
		~BluetoothRadio();

		/// <summary>
		/// Get the bluetooth radios address
		/// </summary>
		/// <returns>The bluetooth address</returns>
		BLUETOOTH_ADDRESS getAddress();

		/// <summary>
		/// Get the bluetooth radios name
		/// </summary>
		/// <returns></returns>
		wstring getName();

		/// <summary>
		/// Get the bluetooth class of device
		/// </summary>
		ULONG getClassOfDevice();

		/// <summary>
		/// Get the Lmp sub version
		/// </summary>
		/// <returns></returns>
		USHORT getLmpSubVersion();

		/// <summary>
		/// Get the manufacturers Id
		/// </summary>
		/// <returns></returns>
		USHORT getManufacturer();
};
#endif