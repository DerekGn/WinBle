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

#include "CallbackContext.h"

void CallbackContext::Register(
	function<void(BleGattNotificationData&)> notificationHandler, 
	PBTH_LE_GATT_CHARACTERISTIC pGattCharacteristic) 
{	
	_notificationHandler = std::move(notificationHandler);
	_pGattCharacteristic = pGattCharacteristic;

	std::lock_guard<std::mutex> lock(_unRegisterMutex);
	_isRegistered = true;
}

void CallbackContext::Unregister()
{
	{
		std::lock_guard<std::mutex> lock(_unRegisterMutex);
		_isRegistered = false;
	}
	{
		std::unique_lock<std::mutex> lock(_isInCallbackMutex);
		_isInCallbackCondition.wait(lock, [this] { return !_isInCallback;  });
	}
}

bool CallbackContext::IsRegistered()
{
	std::lock_guard<std::mutex> lock(_unRegisterMutex);
	return _isRegistered;
}

void CallbackContext::SetInCallback()
{
	std::unique_lock<std::mutex> lock(_isInCallbackMutex);
	_isInCallback = true;
}

void CallbackContext::UnsetInCallback()
{
	std::unique_lock<std::mutex> lock(_isInCallbackMutex);
	_isInCallback = false;
	_isInCallbackCondition.notify_one();
}

function<void(BleGattNotificationData&)> CallbackContext::getNotificationHandler() const
{
	return _notificationHandler;
}

PBTH_LE_GATT_CHARACTERISTIC CallbackContext::getGattCharacteristic() const
{
	return _pGattCharacteristic;
}
