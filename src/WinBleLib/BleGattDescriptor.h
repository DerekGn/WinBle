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

#ifndef BLEGATTDESCRIPTOR_H
#define BLEGATTDESCRIPTOR_H

#include <Windows.h>
#include <bluetoothleapis.h>

#include "BleDeviceContext.h"
#include "BleGattDescriptorValue.h"

#include <list>

using namespace std;

/// <summary>
/// Represents a descriptor for a characteristic of a bluetooth low energy device
/// </summary>
class BleGattDescriptor
{
	private:
		
		PBTH_LE_GATT_SERVICE _pGattService;

		BleDeviceContext& _bleDeviceContext;

		PBTH_LE_GATT_DESCRIPTOR _pGattDescriptor = nullptr;

		void setDescriptorValue(PBTH_LE_GATT_DESCRIPTOR_VALUE value);

	public:
		/// <summary>
		/// Create an instance of a <see cref="BleGattDescriptor"/>
		/// </summary>
		/// <param name="bleDeviceContext">The parent ble device context</param>
		/// <param name="pGattService">The Gatt service</parma>
		/// <param name="pGattDescriptor">The contained <see cref="PBTH_LE_GATT_DESCRIPTOR"/></param>
		BleGattDescriptor(BleDeviceContext& bleDeviceContext, PBTH_LE_GATT_SERVICE pGattService, PBTH_LE_GATT_DESCRIPTOR pGattDescriptor);

		~BleGattDescriptor() = default;

		/// <summary>
		/// Get the descriptors service handle
		/// </summary>
		USHORT getServiceHandle() const;

		/// <summary>
		/// Get the descriptors characteristic handle
		/// </summary>
		USHORT getCharacteristicHandle() const;

		/// <summary>
		/// Get the descriptors type
		/// </summary>
		BTH_LE_GATT_DESCRIPTOR_TYPE getDescriptorType() const;

		/// <summary>
		/// Get the descriptors UUID
		/// </summary>
		BTH_LE_UUID getDescriptorUuid() const;
		
		/// <summary>
		/// Get the descriptors attribute handle
		/// </summary>
		USHORT getAttributeHandle() const;

		/// <summary>
		/// Gets the descriptors value
		/// </summary>
		BleGattDescriptorValue* getValue();

		/// <summary>
		/// Subscribe to notifications
		/// </summary>
		void setIsSubscribeToNotification();

		/// <summary>
		/// Unsubscribe from notifications
		/// </summary>
		void clearIsSubscribeToNotification();

		/// <summary>
		/// Subscribe to indications
		/// </summary>
		void setIsSubscribeToIndication();

		/// <summary>
		/// Unsubscribe from indications
		/// </summary>
		void clearIsSubscribeToIndication();
};
#endif
