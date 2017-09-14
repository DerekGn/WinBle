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

#ifndef BLEGATTCHARACTERISTIC_H
#define BLEGATTCHARACTERISTIC_H

#include <Windows.h>
#include <Bluetoothleapis.h>

#include "CallbackContext.h"
#include "BleGattDescriptor.h"
#include "BleGattNotificationData.h"
#include "BleGattCharacteristicValue.h"

using namespace std;

#include <list>
#include <functional>

/// <summary>
/// Represents a characteristic of a bluetooth low energy device
/// </summary>
class BleGattCharacteristic
{
	private:
		GUID serviceUUID;
		
		USHORT gattDescriptorsCount = 0;

		CallbackContext* callbackContext;

		BleDeviceContext& bleDeviceContext;

		BLUETOOTH_GATT_EVENT_HANDLE eventHandle;
		
		list<BleGattDescriptor*> bleGattDescriptors;

		PBTH_LE_GATT_DESCRIPTOR pGattDescriptors = nullptr;

		PBTH_LE_GATT_CHARACTERISTIC pGattCharacteristic;
		
		/// <summary>
		/// Get the list of gatt desriptors <see cref="PBTH_LE_GATT_DESCRIPTOR"/>
		/// </summary>
		/// <param name="hBleDeviceHandle">The handle to the device to retrive the descriptors</param>
		/// <param name="pGattCharacteristic">The <see cref="PBTH_LE_GATT_CHARACTERISTIC"/> to enumerate the list of descriptors</param>
		/// <param name="pGattDescriptorsCount">The number of descriptors enumerated</param>
		/// <returns>A pointer to the first <see cref="PBTH_LE_GATT_CHARACTERISTIC"/> in the list</returns>
		static PBTH_LE_GATT_DESCRIPTOR getGattDescriptors(HANDLE hBleDeviceHandle, PBTH_LE_GATT_CHARACTERISTIC pGattCharacteristic,
			USHORT * pGattDescriptorsCount);

		/// <summary>
		/// Handle <see cref="BleGattCharacteristic"/> notifications
		/// </summary>
		/// <param name="eventType">The notification</param>
		/// <param name="eventOutParameter">The pointer to the event data associated with the event type</param>
		/// <param name="context">The context passed to the call back</param>
		/// <returns></returns>
		static VOID WINAPI NotificationCallback(BTH_LE_GATT_EVENT_TYPE eventType, PVOID eventOutParameter, PVOID context);
		
	public:
		/// <summary>
		/// Constructs an instance of a <see cref="BleGattCharacteristic"/>
		/// </summary>
		/// <param name="bleDeviceContext">The parent ble device context</param>
		/// <param name="pGattCharacteristic">The contained <see cref="PBTH_LE_GATT_CHARACTERISTIC"/></param>
		BleGattCharacteristic(BleDeviceContext & bleDeviceContext, PBTH_LE_GATT_CHARACTERISTIC pGattCharacteristic);
		
		~BleGattCharacteristic();

		/// <summary>
		/// Get the service handle
		/// </summary>
		USHORT getServiceHandle();

		/// <summary>
		/// Gets the characteristics UUID
		/// </summary>
		BTH_LE_UUID getCharacteristicUuid();

		/// <summary>
		/// Gets the characteristics attribute handle
		/// </summary>
		USHORT getAttributeHandle();
		
		/// <summary>
		/// Gets the characteristics value handle
		/// </summary>
		USHORT getCharacteristicValueHandle();
		
		/// <summary>
		/// Indicates if a characteristic is broadcastable
		/// </summary>
		BOOLEAN getIsBroadcastable();

		/// <summary>
		/// Indicates if a characteristic is readable
		/// </summary>
		BOOLEAN getIsReadable();

		/// <summary>
		/// Indicates if a characteristic is writable
		/// </summary>
		BOOLEAN getIsWritable();

		/// <summary>
		/// Indicates if a characteristic is writable without a response
		/// </summary>
		BOOLEAN getIsWritableWithoutResponse();

		/// <summary>
		/// Indicates if a characteristic is signed writable
		/// </summary>
		BOOLEAN getIsSignedWritable();

		/// <summary>
		/// Indicates if a characteristic is notifiable
		/// </summary>
		BOOLEAN getIsNotifiable();

		/// <summary>
		/// Indicates if a characteristic is indicatable
		/// </summary>
		BOOLEAN getIsIndicatable();

		/// <summary>
		/// Indicates if a characteristic has extended properties
		/// </summary>
		BOOLEAN getHasExtendedProperties();

		/// <summary>
		/// Register the notification callback for this <see cref="BleGattCharacteristic"/>
		/// </summary>
		/// <param name="notificationHandler">The notification function that will be invoked on callback</param>
		/// <remarks>Throws a <see cref="BleGattException"/> if the chararacteritic is not indicatable or notifiable</remarks>
		void registerCallback(function<void(const BleGattNotificationData&)> notificationHandler);

		/// <summary>
		/// Reads a characteristics value
		/// </summary>
		/// <remarks>Throws a BleException if the characteristic is not readable</remarks>
		BleGattCharacteristicValue getValue();

		typedef list<BleGattDescriptor*> BleGattDescriptors;

		/// <summary>
		/// Gets a list of <see cref="BleDescriptors"/> associated with this characteristic
		/// </summary>
		const BleGattDescriptors& getBleDescriptors();
};
#endif
