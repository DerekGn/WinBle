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

#ifndef BLEGATTCHARACTERISTICVALUE_H
#define BLEGATTCHARACTERISTICVALUE_H

#include <Windows.h>
#include <bluetoothleapis.h>

/// <summary>
/// Represents a bluetooth low energy characteristic value read from a device
/// </summary>
class BleGattCharacteristicValue
{
	private:
		PBTH_LE_GATT_CHARACTERISTIC_VALUE pGattCharacteristicValue = nullptr;

	public:
		/// <summary>
		/// Creates an instance of a bluetooth low energy characteristic value
		/// </summary>
		/// <param name="pGattCharacteristicValue">The underlying characteristic value</param>
		explicit BleGattCharacteristicValue(PBTH_LE_GATT_CHARACTERISTIC_VALUE pGattCharacteristicValue);
		
		~BleGattCharacteristicValue();

		/// <summary>
		/// The size of the underlying data
		/// </summary>
		unsigned long getDataSize();

		/// <summary>
		/// Gets the data for this characteristic value
		/// </summary>
		/// <returns>A const pointer to the data</returns>
		const unsigned char* getData();
};

#endif

