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

#ifndef BLEGATTSERVICE_H
#define BLEGATTSERVICE_H

#include <Windows.h>
#include <bluetoothleapis.h>

#include <list>

#include "BleGattCharacteristic.h"
#include "HandleWrapper.h"

using namespace std;

/// <summary>
/// Represents a service of a bluetooth low energy device
/// </summary>
class BleGattService
{
	private:
		
		shared_ptr<HandleWrapper> _hBleService;

		BleDeviceContext& _bleDeviceContext;

		USHORT _gattCharacteristicsCount = 0;

		PBTH_LE_GATT_SERVICE _pGattService = nullptr;

		list<shared_ptr<BleGattCharacteristic>> _bleGattCharacteristics;

		PBTH_LE_GATT_CHARACTERISTIC _pGattCharacteristics = nullptr;

		/// <summary>
		/// Get the list of characteristics for a service
		/// </summary>
		/// <param name="hBleDeviceHandle">The device handle</param>
		/// <param name="pGattService">The service to retrieve characteristics</param>
		/// <param name="pGattCharcteristicsCount">The characteristic count</param>
		/// <returns>A pointer to the first <see cref="PBTH_LE_GATT_CHARACTERISTIC"/> in the list</returns>
		static PBTH_LE_GATT_CHARACTERISTIC getGattCharacteristics(HANDLE hBleDeviceHandle, 
			PBTH_LE_GATT_SERVICE pGattService, USHORT * pGattCharcteristicsCount);

	public:
		/// <summary>
		/// Create an instance of a <see cref="BleGattService"/>
		/// </summary>
		/// <param name="bleDeviceContext">The parent ble device context</param>
		/// <param name="pGattService">The contained <see cref="PBTH_LE_GATT_SERVICE"/></param>
		BleGattService(BleDeviceContext & bleDeviceContext, PBTH_LE_GATT_SERVICE pGattService);

		~BleGattService();

		/// <summary>
		/// Gets the services UUID
		/// </summary>
		BTH_LE_UUID getServiceUuid() const;

		/// <summary>
		/// Gets the services attribute handle
		/// </summary>
		USHORT getServiceAttributeHandle() const;

		/// <summary>
		/// Enumerate this services list of ble characteristics
		/// </summary>
		/// <remarks>must be called prior to calling get characteristics</remarks>
		void enumerateBleCharacteristics();

		using BleGattCharacteristics = list<shared_ptr<BleGattCharacteristic>>;

		/// <summary>
		/// Gets the services list of characteristics
		/// </summary>
		const BleGattCharacteristics& getBleCharacteristics() const;
};

#endif
