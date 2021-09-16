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

#ifndef CALLBACKCONTEXT_H
#define CALLBACKCONTEXT_H

#include <Windows.h>
#include <mutex>

#include <bluetoothleapis.h>
#include "BleGattNotificationData.h"

#include <functional>
#include <condition_variable>

using namespace std;

class CallbackContext
{
	private:

		bool _isRegistered{};
		std::mutex _unRegisterMutex;
	
		bool _isInCallback{};
		std::mutex _isInCallbackMutex;
		condition_variable _isInCallbackCondition;

		PBTH_LE_GATT_CHARACTERISTIC _pGattCharacteristic;

		function<void(BleGattNotificationData&)> _notificationHandler;

		void SetInCallback();
		void UnsetInCallback();

		friend class CallbackScope;

	public:
		// Stores arguments and atomically sets _isRegistered
	
		void Register(
			function<void(BleGattNotificationData&)> notificationHandler, 
			PBTH_LE_GATT_CHARACTERISTIC pGattCharacteristic);

		// Atomically unset _isRegistered and waits for any pending callback to complete
	
		void Unregister();

		bool IsRegistered();
	
		function<void(BleGattNotificationData&)> getNotificationHandler() const;

		PBTH_LE_GATT_CHARACTERISTIC getGattCharacteristic() const;
};

#endif
