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

#ifndef BLEDEVICEENUMERATOR_H
#define BLEDEVICEENUMERATOR_H

#include <SetupAPI.h>

#include "BleDeviceInfo.h"

#include <list>

using namespace std;

/// <summary>
/// A class that enumerates paired ble devices
/// </summary>
class BleDeviceEnumerator final
{
	private:
		static list<BleDeviceInfo*> _bleEnumeratedDevices;
	
		/// <summary>
		/// Reads a string from the device registry
		/// </summary>
		/// <param name="hDI">The device info handle</param>
		/// <param name="did">The device info data</param>
		/// <param name="property">The property identifier</param>
		/// <returns>The device registry string value</returns>
		static wstring getDeviceRegistryStringProperty(HDEVINFO hDI, SP_DEVINFO_DATA did, int property);

		BleDeviceEnumerator() = delete;
	public:
		~BleDeviceEnumerator();

		/// <summary>
		/// Enumerate the paired ble devices
		/// </summary>
		static void enumerate();

		using BleDevices = list<BleDeviceInfo*>;

		/// <summary>
		/// Gets the enumerated devices
		/// </summary>
		/// <returns>A list of devices enumerated after the call to enumerate</returns>
		static const BleDevices& getBleDevices();
};

#endif