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

#include "BleGattDescriptor.h"
#include "HandleWrapper.h"
#include "BleFunctions.h"
#include "BleException.h"
#include "Utility.h"

#include <sstream>

using namespace std;

void BleGattDescriptor::setDescriptorValue(PBTH_LE_GATT_DESCRIPTOR_VALUE newValue)
{
	HandleWrapper hBleService(
		openBleInterfaceHandle(
			mapServiceUUID(&_pGattService->ServiceUuid),
			GENERIC_READ | GENERIC_WRITE));

	HRESULT hr = BluetoothGATTSetDescriptorValue(
		hBleService.get(),
		_pGattDescriptor,
		newValue,
		BLUETOOTH_GATT_FLAG_NONE);

	if (S_OK != hr) {
		Utility::throwHResultException("Unable to set the descriptor value.", hr);
	}
}

BleGattDescriptor::BleGattDescriptor(
	BleDeviceContext& bleDeviceContext,
	PBTH_LE_GATT_SERVICE pGattService,
	PBTH_LE_GATT_DESCRIPTOR pGattDescriptor) :
	_bleDeviceContext(bleDeviceContext),
	_pGattService(pGattService)
{
	if (!pGattDescriptor)
		throw BleException("pGattDescriptor is nullptr");
	
	_pGattDescriptor = pGattDescriptor;
}

USHORT BleGattDescriptor::getServiceHandle() const
{
	return _pGattDescriptor->ServiceHandle;
}

USHORT BleGattDescriptor::getCharacteristicHandle() const
{
	return _pGattDescriptor->CharacteristicHandle;
}

BTH_LE_GATT_DESCRIPTOR_TYPE BleGattDescriptor::getDescriptorType() const
{
	return _pGattDescriptor->DescriptorType;
}

BTH_LE_UUID BleGattDescriptor::getDescriptorUuid() const
{
	return _pGattDescriptor->DescriptorUuid;
}

USHORT BleGattDescriptor::getAttributeHandle() const
{
	return _pGattDescriptor->AttributeHandle;
}

BleGattDescriptorValue* BleGattDescriptor::getValue()
{
	USHORT descValueDataSize;

	HandleWrapper hBleService(
		openBleInterfaceHandle(
			mapServiceUUID(&_pGattService->ServiceUuid), 
			GENERIC_READ));

	HRESULT hr = BluetoothGATTGetDescriptorValue(
		hBleService.get(),
		_pGattDescriptor,
		0,
		nullptr,
		&descValueDataSize,
		BLUETOOTH_GATT_FLAG_NONE);

	if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr)
	{
		Utility::throwHResultException("Unable to determine the descriptor value size.", hr);
	}

	auto pDescValueBuffer = (PBTH_LE_GATT_DESCRIPTOR_VALUE)malloc(descValueDataSize);

	if (pDescValueBuffer == nullptr)
	{
		Utility::handleMallocFailure(descValueDataSize);
	}
	else
	{
		RtlZeroMemory(pDescValueBuffer, descValueDataSize);
	}

	hr = BluetoothGATTGetDescriptorValue(
		hBleService.get(),
		_pGattDescriptor,
		(ULONG)descValueDataSize,
		pDescValueBuffer,
		nullptr,
		BLUETOOTH_GATT_FLAG_NONE);

	if (hr != S_OK)
	{
		Utility::throwHResultException("Unable to read the descriptor value size.", hr);
	}
	
	return new BleGattDescriptorValue(pDescValueBuffer);
}

void BleGattDescriptor::setIsSubscribeToNotification()
{
	BTH_LE_GATT_DESCRIPTOR_VALUE newValue;

	RtlZeroMemory(&newValue, sizeof(newValue));

	newValue.DescriptorType = ClientCharacteristicConfiguration;
	newValue.ClientCharacteristicConfiguration.IsSubscribeToNotification = TRUE;

	setDescriptorValue(&newValue);
}

void BleGattDescriptor::clearIsSubscribeToNotification()
{
	BTH_LE_GATT_DESCRIPTOR_VALUE newValue;

	RtlZeroMemory(&newValue, sizeof(newValue));

	newValue.DescriptorType = ClientCharacteristicConfiguration;
	newValue.ClientCharacteristicConfiguration.IsSubscribeToNotification = FALSE;

	setDescriptorValue(&newValue);
}

void BleGattDescriptor::setIsSubscribeToIndication()
{
	BTH_LE_GATT_DESCRIPTOR_VALUE newValue;

	RtlZeroMemory(&newValue, sizeof(newValue));

	newValue.DescriptorType = ClientCharacteristicConfiguration;
	newValue.ClientCharacteristicConfiguration.IsSubscribeToIndication = TRUE;

	setDescriptorValue(&newValue);
}

void BleGattDescriptor::clearIsSubscribeToIndication()
{
	BTH_LE_GATT_DESCRIPTOR_VALUE newValue;

	RtlZeroMemory(&newValue, sizeof(newValue));

	newValue.DescriptorType = ClientCharacteristicConfiguration;
	newValue.ClientCharacteristicConfiguration.IsSubscribeToIndication = FALSE;

	setDescriptorValue(&newValue);
}
