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

#ifndef BLEGATTDESCRIPTORVALUE_H
#define BLEGATTDESCRIPTORVALUE_H

#include <Windows.h>
#include <bluetoothleapis.h>

using namespace std;

/// <summary>
/// Represents a descriptor value read from a characteristic of a bluetooth low energy device
/// </summary>
class BleGattDescriptorValue
{
	private:
		PBTH_LE_GATT_DESCRIPTOR_VALUE _pGattDescriptorValue = nullptr;

	public:
		/// <summary>
		/// Create an instance of a <see cref="BleGattDescriptor"/>
		/// </summary>
		/// <param name="pGattDescriptorValue">The contained <see cref="PBTH_LE_GATT_DESCRIPTOR_VALUE"/></param>
		explicit BleGattDescriptorValue(PBTH_LE_GATT_DESCRIPTOR_VALUE pGattDescriptorValue);

		~BleGattDescriptorValue();

		/// <summary>
		/// Get the descriptor type
		/// </summary>
		BTH_LE_GATT_DESCRIPTOR_TYPE GetDescriptorType();

		/// <summary>
		/// Get the descriptor UUID
		/// </summary>
		BTH_LE_UUID getDescriptorUuid();

		/// <summary>
		/// Indicates if a descriptor is reliable write enabled
		/// </summary>
		BOOLEAN getIsReliableWriteEnabled();

		/// <summary>
		/// Indicates if a descriptor is auxiliaries write enabled
		/// </summary>
		BOOLEAN getIsAuxiliariesWritable();

		/// <summary>
		/// Indicates if a descriptor can be subscribed too for notification
		/// </summary>
		BOOLEAN getIsSubscribeToNotification();

		/// <summary>
		/// Indicates if a descriptor can be subscribed too for indication
		/// </summary>
		BOOLEAN getIsSubscribeToIndication();

		/// <summary>
		/// Indicates if a descriptor is broadcast
		/// </summary>
		BOOLEAN getIsBroadcast();

		/// <summary>
		/// Gets the descriptor format
		/// </summary>
		unsigned char getFormat();

		/// <summary>
		/// Gets the descriptor exponent
		/// </summary>
		unsigned char getExponent();

		/// <summary>
		/// Gets the descriptor unit
		/// </summary>
		BTH_LE_UUID getUnit();

		/// <summary>
		/// Gets the descriptor name space
		/// </summary>
		unsigned char getNameSpace();

		/// <summary>
		/// Gets the descriptor description
		/// </summary>
		BTH_LE_UUID getDescription();

		/// <summary>
		/// Gets the descriptor data size
		/// </summary>
		unsigned long getDataSize();

		/// <summary>
		/// Gets the descriptor data
		/// </summary>
		unsigned char* getData();

		/// <summary>
		/// Gets the descriptor value
		/// </summary>
		PBTH_LE_GATT_DESCRIPTOR_VALUE getValue();
};

#endif
