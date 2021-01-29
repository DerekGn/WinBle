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
#include "CallbackScope.h"
#include "FileHandleWrapper.h"
#include "BleFunctions.h"
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
				<< Util.getLastError(hr) << "]";

			throw BleException(msg.str());
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
					<< Util.getLastError(hr) << "]";

				throw BleException(msg.str());
			}

			if (*pGattDescriptorsCount != expectedDescriptorBufferCount) {
				throw BleException("descriptor count expected and descriptor count actual mismatch");
			}
		}
	}

	return pDescriptorBuffer;
}

VOID WINAPI BleGattCharacteristic::NotificationCallback(BTH_LE_GATT_EVENT_TYPE eventType, PVOID eventOutParameter, PVOID context)
{
	BLUETOOTH_GATT_VALUE_CHANGED_EVENT *pEvent = (BLUETOOTH_GATT_VALUE_CHANGED_EVENT *)eventOutParameter;
	CallbackContext* callbackContext = static_cast<CallbackContext*>(context);

	// Assures that after unregisterNotificationHandler is called, no more callbacks will be invoked.

	if (! callbackContext->IsRegistered())
	{
		return;
	}

	// Assures that unregisterNotificationHandler will wait until the last invocation of the callback
	// has completed

	CallbackScope callScope(*callbackContext);
	
	if (pEvent->ChangedAttributeHandle == callbackContext->getGattCharacteristic()->AttributeHandle)
	{
		PBYTE p = (PBYTE) malloc(pEvent->CharacteristicValue->DataSize);

		BleGattNotificationData* notification = new BleGattNotificationData(pEvent->CharacteristicValue->DataSize, p);

		callbackContext->getNotificationHandler()(*notification);
	}
}

BleGattCharacteristic::BleGattCharacteristic(
	BleDeviceContext &bleDeviceContext,
	PBTH_LE_GATT_SERVICE pGattService,
	PBTH_LE_GATT_CHARACTERISTIC pGattCharacteristic) :
	_pGattCharacteristic(pGattCharacteristic),
	_bleDeviceContext(bleDeviceContext),
	_eventHandle(INVALID_HANDLE_VALUE),
	_pGattService(pGattService)
{
}

BleGattCharacteristic::~BleGattCharacteristic()
{
	if (_eventHandle != INVALID_HANDLE_VALUE)
		BluetoothGATTUnregisterEvent(_eventHandle, BLUETOOTH_GATT_FLAG_NONE);

	for (BleGattDescriptor *d : _bleGattDescriptors)
		delete(d);

	if (_pGattDescriptors)
		free(_pGattDescriptors);
}

USHORT BleGattCharacteristic::getServiceHandle()
{
	return _pGattCharacteristic->ServiceHandle;
}

BTH_LE_UUID BleGattCharacteristic::getCharacteristicUuid()
{
	return _pGattCharacteristic->CharacteristicUuid;
}

USHORT BleGattCharacteristic::getAttributeHandle()
{
	return _pGattCharacteristic->AttributeHandle;
}

USHORT BleGattCharacteristic::getCharacteristicValueHandle()
{
	return _pGattCharacteristic->CharacteristicValueHandle;
}

BOOLEAN BleGattCharacteristic::getIsBroadcastable()
{
	return _pGattCharacteristic->IsBroadcastable;
}

BOOLEAN BleGattCharacteristic::getIsReadable()
{
	return _pGattCharacteristic->IsReadable;
}

BOOLEAN BleGattCharacteristic::getIsWritable()
{
	return _pGattCharacteristic->IsWritable;
}

BOOLEAN BleGattCharacteristic::getIsWritableWithoutResponse()
{
	return _pGattCharacteristic->IsWritableWithoutResponse;
}

BOOLEAN BleGattCharacteristic::getIsSignedWritable()
{
	return _pGattCharacteristic->IsSignedWritable;
}

BOOLEAN BleGattCharacteristic::getIsNotifiable()
{
	return _pGattCharacteristic->IsNotifiable;
}

BOOLEAN BleGattCharacteristic::getIsIndicatable()
{
	return _pGattCharacteristic->IsIndicatable;
}

BOOLEAN BleGattCharacteristic::getHasExtendedProperties()
{
	return _pGattCharacteristic->HasExtendedProperties;
}

void BleGattCharacteristic::registerNotificationHandler(function<void(BleGattNotificationData&)> notificationHandler)
{
	if (_callbackContext.IsRegistered())
		return;

	if (_pGattCharacteristic->IsNotifiable || _pGattCharacteristic->IsIndicatable)
	{
		BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION registration;
		registration.NumCharacteristics = 1;
		registration.Characteristics[0] = *_pGattCharacteristic;

		_callbackContext.Register(std::move(notificationHandler), _pGattCharacteristic);

		FileHandleWrapper hBleService(
			openBleInterfaceHandle(
				mapServiceUUID(&_pGattService->ServiceUuid), 
				GENERIC_READ | GENERIC_WRITE));

		HRESULT hr = BluetoothGATTRegisterEvent(hBleService.get(), CharacteristicValueChangedEvent,
			&registration, &NotificationCallback, &_callbackContext, &_eventHandle, BLUETOOTH_GATT_FLAG_NONE);

		if (S_OK != hr)
		{
			stringstream msg;
			msg << "Unable to subscribe to the characteristic. Reason: ["
				<< Util.getLastError(hr) << "]";

			throw BleException(msg.str());
		}
	}
	else
	{
		throw BleException("characteristic is not notifiable or indicatable");
	}
}

void BleGattCharacteristic::unregisterNotificationHandler()
{
	if (_callbackContext.IsRegistered())
	{
		HRESULT hr = BluetoothGATTUnregisterEvent(_eventHandle, BLUETOOTH_GATT_FLAG_NONE);

		// Waits until the last invocation of the callback has completed
		
		_callbackContext.Unregister();

		if (S_OK != hr)
		{
			stringstream msg;
			msg << "Unable to unsubscribe from the characteristic. Reason: ["
				<< Util.getLastError(hr) << "]";

			throw BleException(msg.str());
		}
	}
}

BleGattCharacteristicValue BleGattCharacteristic::getValue()
{
	PBTH_LE_GATT_CHARACTERISTIC_VALUE pCharValueBuffer = nullptr;
	USHORT charValueDataSize = 0;

	if (_pGattCharacteristic->IsReadable) 
	{
		FileHandleWrapper hBleService(
			openBleInterfaceHandle(mapServiceUUID(&_pGattService->ServiceUuid),
				GENERIC_READ));

		HRESULT hr = BluetoothGATTGetCharacteristicValue(
			hBleService.get(),
			_pGattCharacteristic,
			0,
			NULL,
			&charValueDataSize,
			BLUETOOTH_GATT_FLAG_NONE);

		if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) 
		{
			stringstream msg;
			msg << "Unable to determine the characeristic value size. Reason: ["
				<< Util.getLastError(hr) << "]";

			throw BleException(msg.str());
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
			hBleService.get(),
			_pGattCharacteristic,
			(ULONG)charValueDataSize,
			pCharValueBuffer,
			NULL,
			BLUETOOTH_GATT_FLAG_NONE);

		if (S_OK != hr)
		{
			stringstream msg;
			msg << "Unable to read the characeristic value. Reason: ["
				<< Util.getLastError(hr) << "]";

			throw BleException(msg.str());
		}
	}
	else
	{
		throw BleException("characteristic is not readable");
	}

	return BleGattCharacteristicValue(pCharValueBuffer);
}

void BleGattCharacteristic::setValue(UCHAR * data, ULONG size)
{
	if (_pGattCharacteristic->IsSignedWritable || _pGattCharacteristic->IsWritable || _pGattCharacteristic->IsWritableWithoutResponse)
	{
		size_t required_size = sizeof(BTH_LE_GATT_CHARACTERISTIC_VALUE) + size;

		PBTH_LE_GATT_CHARACTERISTIC_VALUE gatt_value = (PBTH_LE_GATT_CHARACTERISTIC_VALUE)malloc(required_size);

		ZeroMemory(gatt_value, required_size);

		gatt_value->DataSize = (ULONG)size;
		memcpy(gatt_value->Data, data, size);

		FileHandleWrapper hBleService(
			openBleInterfaceHandle(mapServiceUUID(&_pGattService->ServiceUuid),
			GENERIC_WRITE));

		HRESULT hr = BluetoothGATTSetCharacteristicValue(hBleService.get(), _pGattCharacteristic, gatt_value, NULL, BLUETOOTH_GATT_FLAG_NONE);

		delete(gatt_value);

		if (HRESULT_FROM_WIN32(S_OK) != hr)
		{
			stringstream msg;
			msg << "Unable to write the characeristic value. Reason: ["
				<< Util.getLastError(hr) << "]";

			throw BleException(msg.str());
		}
	}
	else
	{
		throw BleException("characteristic is not writable");
	}
}

void BleGattCharacteristic::enumerateBleDescriptors()
{
	for (BleGattDescriptor *d : _bleGattDescriptors)
		delete(d);

	if (_pGattDescriptors)
		free(_pGattDescriptors);

	_gattDescriptorsCount = 0;
	_pGattDescriptors = getGattDescriptors(_bleDeviceContext.getBleDeviceHandle(), _pGattCharacteristic, &_gattDescriptorsCount);

	for (size_t i = 0; i < _gattDescriptorsCount; i++)
		_bleGattDescriptors.push_back(new BleGattDescriptor(_bleDeviceContext, _pGattService, &_pGattDescriptors[i]));

}

const BleGattCharacteristic::BleGattDescriptors& BleGattCharacteristic::getBleDescriptors()
{
	return _bleGattDescriptors;
}
