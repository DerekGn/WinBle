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

#include "BleGattService.h"
#include "BleFunctions.h"
#include "BleException.h"
#include "Utility.h"

#include <sstream>

using namespace std;

PBTH_LE_GATT_CHARACTERISTIC BleGattService::getGattCharacteristics(HANDLE hBleDeviceHandle, 
	PBTH_LE_GATT_SERVICE pGattService, USHORT * pGattCharcteristicsCount)
{
	PBTH_LE_GATT_CHARACTERISTIC pCharBuffer = nullptr;
	USHORT expectedCharBufferCount;
	*pGattCharcteristicsCount = 0;

	HRESULT hr = BluetoothGATTGetCharacteristics(
		hBleDeviceHandle,
		pGattService,
		0,
		NULL,
		&expectedCharBufferCount,
		BLUETOOTH_GATT_FLAG_NONE);

	if (HRESULT_FROM_WIN32(ERROR_NOT_FOUND) != hr)
	{
		if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr)
		{
			stringstream msg;
			msg << "Unable to determine the number of gatt characteristics. Reason: ["
				<< Util.getLastError(hr) << "]";

			throw BleException(msg.str());
		}

		if (expectedCharBufferCount > 0)
		{
			pCharBuffer = (PBTH_LE_GATT_CHARACTERISTIC)
				malloc(expectedCharBufferCount * sizeof(BTH_LE_GATT_CHARACTERISTIC));

			if (NULL == pCharBuffer)
			{
				Util.handleMallocFailure(sizeof(BTH_LE_GATT_SERVICE) * expectedCharBufferCount);
			}
			else
			{
				RtlZeroMemory(pCharBuffer,
					expectedCharBufferCount * sizeof(BTH_LE_GATT_CHARACTERISTIC));
			}

			hr = BluetoothGATTGetCharacteristics(
				hBleDeviceHandle,
				pGattService,
				expectedCharBufferCount,
				pCharBuffer,
				pGattCharcteristicsCount,
				BLUETOOTH_GATT_FLAG_NONE);

			if (S_OK != hr)
			{
				stringstream msg;
				msg << "Unable to determine the number of gatt characteristics. Reason: ["
					<< Util.getLastError(hr) << "]";

				throw BleException(msg.str());
			}

			if (*pGattCharcteristicsCount != expectedCharBufferCount)
			{
				throw BleException("characteristic count expected and characteristic count actual mismatch");
			}
		}
	}
	return pCharBuffer;
}

BleGattService::BleGattService(BleDeviceContext& bleDeviceContext, PBTH_LE_GATT_SERVICE pGattService)
	:_bleDeviceContext(bleDeviceContext)
{
	if (!pGattService)
		throw BleException("pGattService is nullptr");

	_pGattService = pGattService;
}

BleGattService::~BleGattService()
{
	for (BleGattCharacteristic *c : _bleGattCharacteristics)
		delete(c);

	if (_pGattCharacteristics)
		free(_pGattCharacteristics);

	if(_hBleService)
		CloseHandle(_hBleService);
}

BTH_LE_UUID BleGattService::getServiceUuid()
{
	return _pGattService->ServiceUuid;
}

USHORT BleGattService::getServiceAttributeHandle()
{
	return _pGattService->AttributeHandle;
}

void BleGattService::enumerateBleCharacteristics()
{
	for (BleGattCharacteristic *c : _bleGattCharacteristics)
		delete(c);

	_pGattCharacteristics = getGattCharacteristics(_bleDeviceContext.getBleDeviceHandle(), _pGattService, &_gattCharacteristicsCount);

	_hBleService = openBleInterfaceHandle(
		mapServiceUUID(&_pGattService->ServiceUuid),
		GENERIC_READ);

	for (size_t i = 0; i < _gattCharacteristicsCount; i++)
		_bleGattCharacteristics.push_back(new BleGattCharacteristic(_bleDeviceContext, _pGattService, &_pGattCharacteristics[i]));
}

const BleGattService::BleGattCharacteristics & BleGattService::getBleCharacteristics()
{
	return _bleGattCharacteristics;
}
