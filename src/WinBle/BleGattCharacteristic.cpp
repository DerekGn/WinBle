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

#include "BleGattCharacteristic.h"
#include "BleException.h"
#include "Utility.h"

#include <sstream>

PBTH_LE_GATT_DESCRIPTOR BleGattCharacteristic::getGattDescriptors(HANDLE hBleDeviceHandle,
	PBTH_LE_GATT_CHARACTERISTIC pGattCharacteristic, USHORT * pGattDescriptorsCount)
{
	PBTH_LE_GATT_DESCRIPTOR pDescriptorBuffer = nullptr;
	USHORT expectedDescriptorBufferCount = 0;
	*pGattDescriptorsCount = 0;

	HRESULT hr = BluetoothGATTGetDescriptors(
		hBleDeviceHandle,
		pGattCharacteristic,
		0,
		NULL,
		&expectedDescriptorBufferCount,
		BLUETOOTH_GATT_FLAG_NONE);

	if (HRESULT_FROM_WIN32(ERROR_NOT_FOUND) != hr)
	{
		if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr)
		{
			stringstream msg;
			msg << "Unable to determine the number of gatt descriptors. Reason: ["
				<< Util.getLastError() << "]";

			throw new BleException(msg.str());
		}
		
		if (expectedDescriptorBufferCount > 0)
		{
			pDescriptorBuffer = (PBTH_LE_GATT_DESCRIPTOR)
				malloc(expectedDescriptorBufferCount
					* sizeof(BTH_LE_GATT_DESCRIPTOR));

			if (NULL == pDescriptorBuffer)
			{
				Util.handleMallocFailure(sizeof(PBTH_LE_GATT_DESCRIPTOR) * expectedDescriptorBufferCount);
			}
			else
			{
				RtlZeroMemory(pDescriptorBuffer, expectedDescriptorBufferCount);
			}

			hr = BluetoothGATTGetDescriptors(
				hBleDeviceHandle,
				pGattCharacteristic,
				expectedDescriptorBufferCount,
				pDescriptorBuffer,
				pGattDescriptorsCount,
				BLUETOOTH_GATT_FLAG_NONE);

			if (S_OK != hr)
			{
				stringstream msg;
				msg << "Unable to determine the number of gatt services. Reason: ["
					<< Util.getLastError() << "]";

				throw new BleException(msg.str());
			}

			if (*pGattDescriptorsCount != expectedDescriptorBufferCount) {
				throw new BleException("descriptor count expected and descriptor count actual mismatch");
			}
		}
	}

	return pDescriptorBuffer;
}

VOID WINAPI BleGattCharacteristic::NotificationCallback(BTH_LE_GATT_EVENT_TYPE eventType, PVOID eventOutParameter, PVOID context)
{
	BLUETOOTH_GATT_VALUE_CHANGED_EVENT *pEvent = (BLUETOOTH_GATT_VALUE_CHANGED_EVENT *)eventOutParameter;
	CallbackContext* callbackContext = static_cast<CallbackContext*>(context);
	if (pEvent->ChangedAttributeHandle == callbackContext->getGattCharacteristic()->AttributeHandle)
	{
		PBYTE p = (PBYTE) malloc(pEvent->CharacteristicValue->DataSize);

		BleGattNotificationData* notification = new BleGattNotificationData(pEvent->CharacteristicValue->DataSize, p);

		callbackContext->getNotificationHandler()(*notification);
	}
}

BleGattCharacteristic::BleGattCharacteristic(BleDeviceContext & _bleDeviceContext, PBTH_LE_GATT_CHARACTERISTIC _pGattCharacteristic) :
	bleDeviceContext(_bleDeviceContext),
	eventHandle(INVALID_HANDLE_VALUE)
{
	if (!_pGattCharacteristic)
		throw new BleException("pGattCharacteristic is nullptr");

	pGattCharacteristic = _pGattCharacteristic;

	bleDeviceContext = _bleDeviceContext;

	gattDescriptorsCount = 0;
	pGattDescriptors = getGattDescriptors(bleDeviceContext.getBleDeviceHandle(), pGattCharacteristic, &gattDescriptorsCount);

	for (size_t i = 0; i < gattDescriptorsCount; i++)
		bleGattDescriptors.push_back(new BleGattDescriptor(bleDeviceContext, &pGattDescriptors[i]));
}

BleGattCharacteristic::~BleGattCharacteristic()
{
	if (eventHandle != INVALID_HANDLE_VALUE)
		BluetoothGATTUnregisterEvent(eventHandle, BLUETOOTH_GATT_FLAG_NONE);

	for (BleGattDescriptor *d : bleGattDescriptors)
		delete(d);

	if (pGattDescriptors)
		free(pGattDescriptors);
}

USHORT BleGattCharacteristic::getServiceHandle()
{
	return pGattCharacteristic->ServiceHandle;
}

BTH_LE_UUID BleGattCharacteristic::getCharacteristicUuid()
{
	return pGattCharacteristic->CharacteristicUuid;
}

USHORT BleGattCharacteristic::getAttributeHandle()
{
	return pGattCharacteristic->AttributeHandle;
}

USHORT BleGattCharacteristic::getCharacteristicValueHandle()
{
	return pGattCharacteristic->CharacteristicValueHandle;
}

BOOLEAN BleGattCharacteristic::getIsBroadcastable()
{
	return pGattCharacteristic->IsBroadcastable;
}

BOOLEAN BleGattCharacteristic::getIsReadable()
{
	return pGattCharacteristic->IsReadable;
}

BOOLEAN BleGattCharacteristic::getIsWritable()
{
	return pGattCharacteristic->IsWritable;
}

BOOLEAN BleGattCharacteristic::getIsWritableWithoutResponse()
{
	return pGattCharacteristic->IsWritableWithoutResponse;
}

BOOLEAN BleGattCharacteristic::getIsSignedWritable()
{
	return pGattCharacteristic->IsSignedWritable;
}

BOOLEAN BleGattCharacteristic::getIsNotifiable()
{
	return pGattCharacteristic->IsNotifiable;
}

BOOLEAN BleGattCharacteristic::getIsIndicatable()
{
	return pGattCharacteristic->IsIndicatable;
}

BOOLEAN BleGattCharacteristic::getHasExtendedProperties()
{
	return pGattCharacteristic->HasExtendedProperties;
}

void BleGattCharacteristic::registerCallback(function<void(BleGattNotificationData&)> notificationHandler)
{
	if (pGattCharacteristic->IsNotifiable || pGattCharacteristic->IsIndicatable)
	{
		BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION registration;
		registration.NumCharacteristics = 1;
		registration.Characteristics[0] = *pGattCharacteristic;
		
		if (callbackContext != nullptr)
			delete callbackContext;

		callbackContext = new CallbackContext(notificationHandler, pGattCharacteristic);

		BluetoothGATTRegisterEvent(bleDeviceContext.getBleServiceHandle(), CharacteristicValueChangedEvent, 
			&registration, &NotificationCallback, callbackContext, &eventHandle, BLUETOOTH_GATT_FLAG_NONE);
	}
	else
	{
		throw new BleException("characteristic is not notifiable or indicatable");
	}
}

BleGattCharacteristicValue BleGattCharacteristic::getValue()
{
	PBTH_LE_GATT_CHARACTERISTIC_VALUE pCharValueBuffer = nullptr;
	USHORT charValueDataSize = 0;

	if (pGattCharacteristic->IsReadable) 
	{
		HRESULT hr = BluetoothGATTGetCharacteristicValue(
			bleDeviceContext.getBleServiceHandle(),
			pGattCharacteristic,
			0,
			NULL,
			&charValueDataSize,
			BLUETOOTH_GATT_FLAG_NONE);

		if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) 
		{
			stringstream msg;
			msg << "Unable to determine the characeristic value size. Reason: ["
				<< Util.getLastError() << "]";

			throw new BleException(msg.str());
		}

		pCharValueBuffer = (PBTH_LE_GATT_CHARACTERISTIC_VALUE)malloc(charValueDataSize);

		if (NULL == pCharValueBuffer) 
		{
			Util.handleMallocFailure(charValueDataSize);
		}
		else 
		{
			RtlZeroMemory(pCharValueBuffer, charValueDataSize);
		}

		hr = BluetoothGATTGetCharacteristicValue(
			bleDeviceContext.getBleServiceHandle(),
			pGattCharacteristic,
			(ULONG)charValueDataSize,
			pCharValueBuffer,
			NULL,
			BLUETOOTH_GATT_FLAG_NONE);

		if (S_OK != hr)
		{
			stringstream msg;
			msg << "Unable to read the characeristic value. Reason: ["
				<< Util.getLastError() << "]";

			throw new BleException(msg.str());
		}
	}
	else
	{
		throw new BleException("characteristic is not readable");
	}

	return BleGattCharacteristicValue(pCharValueBuffer);
}

void BleGattCharacteristic::setValue(UCHAR data)
{
	if (pGattCharacteristic->IsSignedWritable || pGattCharacteristic->IsWritable || pGattCharacteristic->IsWritableWithoutResponse)
	{
		BTH_LE_GATT_CHARACTERISTIC_VALUE gatt_value;

		gatt_value.DataSize = 1;
		gatt_value.Data[0] = data;

		HRESULT hr = BluetoothGATTSetCharacteristicValue(bleDeviceContext.getBleServiceHandle(), pGattCharacteristic, &gatt_value, NULL, BLUETOOTH_GATT_FLAG_NONE);

		if (HRESULT_FROM_WIN32(S_OK) != hr)
		{
			stringstream msg;
			msg << "Unable to write the characeristic value. Reason: ["
				<< Util.getLastError() << "]";

			throw new BleException(msg.str());
		}
	}
	else
	{
		throw new BleException("characteristic is not writable");
	}
}

const BleGattCharacteristic::BleGattDescriptors& BleGattCharacteristic::getBleDescriptors()
{
	return bleGattDescriptors;
}
