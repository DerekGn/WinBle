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

using namespace std;

#include <Windows.h>
#include <Bluetoothleapis.h>

#include "BleGattCharacteristic.h"

#include <list>

class BleGattService
{
	private:
		
		BleDeviceContext& bleDeviceContext;

		USHORT gattCharacteristicsCount = 0;

		PBTH_LE_GATT_SERVICE pGattService = nullptr;

		list<BleGattCharacteristic*> bleGattCharacteristics;

		PBTH_LE_GATT_CHARACTERISTIC pGattCharacteristics = nullptr;

		static PBTH_LE_GATT_CHARACTERISTIC getGattCharacteristics(HANDLE hBleDeviceHandle, 
			PBTH_LE_GATT_SERVICE pGattService, USHORT * pGattCharcteristicsCount);

	public:
		BleGattService(BleDeviceContext & bleDeviceContext, PBTH_LE_GATT_SERVICE pGattService);

		~BleGattService();

		BTH_LE_UUID getServiceUuid();

		USHORT getServiceAttributeHandle();

		typedef list<BleGattCharacteristic*> BleGattCharacteristics;

		const BleGattCharacteristics& getBleCharacteristics();
};

#endif
