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

#include "BleDeviceContext.h"
#include "BleFunctions.h"

BleDeviceContext::BleDeviceContext(HANDLE hBleDevice, const wstring& deviceInstanceId) :
	_hBleDevice(hBleDevice),
	_deviceInstanceId(deviceInstanceId)

{
}

BleDeviceContext::~BleDeviceContext()
{
	// The handle will be cleaned up by the ble device destructor
	_hBleDevice = nullptr;
	_deviceInstanceId.clear();
}

HANDLE BleDeviceContext::getBleDeviceHandle()
{
	return _hBleDevice;
}

wstring BleDeviceContext::getDeviceInstanceId() const
{
	return _deviceInstanceId;
}

