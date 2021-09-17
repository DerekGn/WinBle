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

#ifndef BLEDEVICECONTEXT_H
#define BLEDEVICECONTEXT_H

#include <Windows.h>

#include <string>

using namespace std;

static const GUID UUID_DEVICE_INFO = { 0x0000180A, 0x0000, 0x1000, { 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB } };

/// <summary>
/// A device context that encapsulates device information
/// </summary>
class BleDeviceContext
{
	private:
		HANDLE _hBleDevice = nullptr;
		
		wstring _deviceInstanceId;

	public:
		/// <summary>
		/// A <see cref="BleDeviceContext"/>
		/// </summary>
		/// <param name="hBleDevice">The device handle</param>
		/// <param name="deviceInstanceId">The device instance Id</param>
		explicit BleDeviceContext(HANDLE hBleDevice, const wstring& deviceInstanceId);
		
		~BleDeviceContext();

		/// <summary>
		/// Get the device handle
		/// </summary>
		HANDLE getBleDeviceHandle();

		/// <summary>
		/// Gets the device instance Id
		/// </summary>
		wstring getDeviceInstanceId() const;
};

#endif

