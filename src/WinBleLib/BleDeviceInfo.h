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

#ifndef BLEDEVICEINFO_H
#define BLEDEVICEINFO_H

#include <string>

using namespace std;

/// <summary>
/// A class that defines ble device information determined by the <see cref="BleDeviceEnumerator"/>
/// </summary>
class BleDeviceInfo
{
	private:
		wstring _name;

		wstring _hardwareId;

		wstring _instanceId;

	public:
		/// <summary>
		/// Constructs an instance of a <see cref="BleDeviceInfo"/>
		/// </summary>
		/// <param name="name">The device name</param>
		/// <param name="hardwareId">The devices hardware Id</param>
		/// <param name="instanceId">The devices instance Id</param>
		BleDeviceInfo(const wstring& name, const wstring& hardwareId, const wstring& instanceId);

		~BleDeviceInfo() = default;

		/// <summary>
		/// Gets the devices name
		/// </summary>
		wstring getName() const;

		/// <summary>
		/// Gets the devices hardware Id
		/// </summary>
		wstring getHardwareId() const;

		/// <summary>
		/// Gets the devices instance Id
		/// </summary>
		wstring getInstanceId() const;
};

#endif
