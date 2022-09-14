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
#include "HandleWrapper.h"
#include "CallbackScope.h"
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
		nullptr,
		&expectedDescriptorBufferCount,
		BLUETOOTH_GATT_FLAG_NONE);

	if (HRESULT_FROM_WIN32(ERROR_NOT_FOUND) != hr)
	{
		if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr)
		{
			Utility::throwHResultException("Unable to determine the number of gatt descriptors.", hr);
		}
		
		if (expectedDescriptorBufferCount > 0)
		{
			pDescriptorBuffer = (PBTH_LE_GATT_DESCRIPTOR)
				malloc(expectedDescriptorBufferCount
					* sizeof(BTH_LE_GATT_DESCRIPTOR));

			if (pDescriptorBuffer == nullptr)
			{
				Utility::handleMallocFailure(sizeof(PBTH_LE_GATT_DESCRIPTOR) * expectedDescriptorBufferCount);
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
				Utility::throwHResultException("Unable to determine the number of gatt services.", hr);
			}

			if (*pGattDescriptorsCount != expectedDescriptorBufferCount) {
				throw BleException("descriptor count expected and descriptor count actual mismatch");
			}
		}
	}

	return pDescriptorBuffer;
}

VOID WINAPI BleGattCharacteristic::NotificationCallback(BTH_LE_GATT_EVENT_TYPE, PVOID eventOutParameter, PVOID context)
{
	auto *pEvent = (BLUETOOTH_GATT_VALUE_CHANGED_EVENT *)eventOutParameter;
	auto* callbackContext = static_cast<CallbackContext*>(context);

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
		auto notification = new BleGattNotificationData(pEvent->CharacteristicValue);

		callbackContext->getNotificationHandler()(*notification);
	}
}

BleGattCharacteristic::BleGattCharacteristic(
	BleDeviceContext &bleDeviceContext,
	PBTH_LE_GATT_SERVICE pGattService,
	PBTH_LE_GATT_CHARACTERISTIC pGattCharacteristic) :
	_bleDeviceContext(bleDeviceContext),
	_eventHandle(INVALID_HANDLE_VALUE),
	_pGattCharacteristic(pGattCharacteristic),
	_pGattService(pGattService)
{
}

BleGattCharacteristic::~BleGattCharacteristic()
{
	if (_eventHandle != INVALID_HANDLE_VALUE)
		BluetoothGATTUnregisterEvent(_eventHandle, BLUETOOTH_GATT_FLAG_NONE);

	if (_pGattDescriptors)
		free(_pGattDescriptors);
}

USHORT BleGattCharacteristic::getServiceHandle() const
{
	return _pGattCharacteristic->ServiceHandle;
}

BTH_LE_UUID BleGattCharacteristic::getCharacteristicUuid() const
{
	return _pGattCharacteristic->CharacteristicUuid;
}

USHORT BleGattCharacteristic::getAttributeHandle() const
{
	return _pGattCharacteristic->AttributeHandle;
}

USHORT BleGattCharacteristic::getCharacteristicValueHandle() const
{
	return _pGattCharacteristic->CharacteristicValueHandle;
}

BOOLEAN BleGattCharacteristic::getIsBroadcastable() const
{
	return _pGattCharacteristic->IsBroadcastable;
}

BOOLEAN BleGattCharacteristic::getIsReadable() const
{
	return _pGattCharacteristic->IsReadable;
}

BOOLEAN BleGattCharacteristic::getIsWritable() const
{
	return _pGattCharacteristic->IsWritable;
}

BOOLEAN BleGattCharacteristic::getIsWritableWithoutResponse() const
{
	return _pGattCharacteristic->IsWritableWithoutResponse;
}

BOOLEAN BleGattCharacteristic::getIsSignedWritable() const
{
	return _pGattCharacteristic->IsSignedWritable;
}

BOOLEAN BleGattCharacteristic::getIsNotifiable() const
{
	return _pGattCharacteristic->IsNotifiable;
}

BOOLEAN BleGattCharacteristic::getIsIndicatable() const
{
	return _pGattCharacteristic->IsIndicatable;
}

BOOLEAN BleGattCharacteristic::getHasExtendedProperties() const
{
	return _pGattCharacteristic->HasExtendedProperties;
}

void BleGattCharacteristic::registerNotificationHandler(function<void(BleGattNotificationData&)> notificationHandler)
{
	if (_callbackContext.IsRegistered())
		return;

	if (_pGattCharacteristic->IsNotifiable || _pGattCharacteristic->IsIndicatable)
	{
		BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION registration{};
		registration.NumCharacteristics = 1;
		registration.Characteristics[0] = *_pGattCharacteristic;

		_callbackContext.Register(std::move(notificationHandler), _pGattCharacteristic);

		HandleWrapper hBleService(
			openBleInterfaceHandle(
				mapServiceUUID(&_pGattService->ServiceUuid), 
				GENERIC_READ | GENERIC_WRITE));

		HRESULT hr = BluetoothGATTRegisterEvent(hBleService.get(), CharacteristicValueChangedEvent,
			&registration, &NotificationCallback, &_callbackContext, &_eventHandle, BLUETOOTH_GATT_FLAG_NONE);

		if (hr != S_OK)
		{
			Utility::throwHResultException("Unable to subscribe to the characteristic.", hr);
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
			Utility::throwHResultException("Unable to unsubscribe from the characteristic.", hr);
		}
	}
}

BleGattCharacteristicValue BleGattCharacteristic::getValue()
{
	PBTH_LE_GATT_CHARACTERISTIC_VALUE pCharValueBuffer = nullptr;
	USHORT charValueDataSize = 0;

	if (_pGattCharacteristic->IsReadable) 
	{
		HandleWrapper hBleService(
			openBleInterfaceHandle(mapServiceUUID(&_pGattService->ServiceUuid),
				GENERIC_READ));

		HRESULT hr = BluetoothGATTGetCharacteristicValue(
			hBleService.get(),
			_pGattCharacteristic,
			0,
			nullptr,
			&charValueDataSize,
			BLUETOOTH_GATT_FLAG_NONE);

		if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) 
		{
			Utility::throwHResultException("Unable to determine the characteristic value size.", hr);
		}

		pCharValueBuffer = (PBTH_LE_GATT_CHARACTERISTIC_VALUE)malloc(charValueDataSize);

		if (pCharValueBuffer == nullptr) 
		{
			Utility::handleMallocFailure(charValueDataSize);
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
			nullptr,
			BLUETOOTH_GATT_FLAG_NONE);

		if (S_OK != hr)
		{
			Utility::throwHResultException("Unable to read the characteristic value.", hr);
		}
	}
	else
	{
		throw BleException("characteristic is not readable");
	}

	return BleGattCharacteristicValue(pCharValueBuffer);
}

void BleGattCharacteristic::setValue(UCHAR const * data, ULONG size)
{
	if (_pGattCharacteristic->IsSignedWritable || _pGattCharacteristic->IsWritable || _pGattCharacteristic->IsWritableWithoutResponse)
	{
		size_t required_size = sizeof(BTH_LE_GATT_CHARACTERISTIC_VALUE) + size;

		auto gatt_value = (PBTH_LE_GATT_CHARACTERISTIC_VALUE)malloc(required_size);

		if (gatt_value != nullptr)
		{
			ZeroMemory(gatt_value, required_size);

			gatt_value->DataSize = size;
			memcpy(gatt_value->Data, data, size);

			HandleWrapper hBleService(
				openBleInterfaceHandle(mapServiceUUID(&_pGattService->ServiceUuid),
					GENERIC_WRITE));

			HRESULT hr = BluetoothGATTSetCharacteristicValue(hBleService.get(), _pGattCharacteristic, gatt_value, NULL, BLUETOOTH_GATT_FLAG_NONE);

			free(gatt_value);

			if (HRESULT_FROM_WIN32(S_OK) != hr)
			{
				Utility::throwHResultException("Unable to read the characteristic value.", hr);
			}
		}
		else
		{
			throw BleException("Unable to allocate characteristic value memory");
		}
	}
	else
	{
		throw BleException("characteristic is not writable");
	}
}

void BleGattCharacteristic::enumerateBleDescriptors()
{
	if (_pGattDescriptors)
		free(_pGattDescriptors);

	_gattDescriptorsCount = 0;
	_pGattDescriptors = getGattDescriptors(_bleDeviceContext.getBleDeviceHandle(), _pGattCharacteristic, &_gattDescriptorsCount);

	for (size_t i = 0; i < _gattDescriptorsCount; i++)
		_bleGattDescriptors.push_back(make_shared<BleGattDescriptor>(_bleDeviceContext, _pGattService, &_pGattDescriptors[i]));

}

const BleGattCharacteristic::BleGattDescriptors& BleGattCharacteristic::getBleDescriptors() const
{
	return _bleGattDescriptors;
}
