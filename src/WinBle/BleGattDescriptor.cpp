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
#include "BleException.h"
#include "Utility.h"

#include <sstream>

using namespace std;

BleGattDescriptor::BleGattDescriptor(BleDeviceContext& _bleDeviceContext, PBTH_LE_GATT_DESCRIPTOR _pGattDescriptor) :
	bleDeviceContext(_bleDeviceContext)
{
	if (!_pGattDescriptor)
		throw new BleException("pGattDescriptor is nullptr");
	
	pGattDescriptor = _pGattDescriptor;
}

BleGattDescriptor::~BleGattDescriptor()
{

}

USHORT BleGattDescriptor::getServiceHandle()
{
	return pGattDescriptor->ServiceHandle;
}

USHORT BleGattDescriptor::getCharacteristicHandle()
{
	return pGattDescriptor->CharacteristicHandle;
}

BTH_LE_GATT_DESCRIPTOR_TYPE BleGattDescriptor::getDescriptorType()
{
	return pGattDescriptor->DescriptorType;
}

BTH_LE_UUID BleGattDescriptor::getDescriptorUuid()
{
	return pGattDescriptor->DescriptorUuid;
}

USHORT BleGattDescriptor::getAttributeHandle()
{
	return pGattDescriptor->AttributeHandle;
}

BleGattDescriptorValue* BleGattDescriptor::getValue()
{
	USHORT descValueDataSize;

	HRESULT hr = BluetoothGATTGetDescriptorValue(
		bleDeviceContext.getBleServiceHandle(),
		pGattDescriptor,
		0,
		NULL,
		&descValueDataSize,
		BLUETOOTH_GATT_FLAG_NONE);

	if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr)
	{
		stringstream msg;
		msg << "Unable to determine the descriptor value size. Reason: ["
			<< Util.getLastError(hr) << "]";

		throw new BleException(msg.str());
	}

	PBTH_LE_GATT_DESCRIPTOR_VALUE pDescValueBuffer = (PBTH_LE_GATT_DESCRIPTOR_VALUE)malloc(descValueDataSize);

	if (NULL == pDescValueBuffer)
	{
		Util.handleMallocFailure(descValueDataSize);
	}
	else
	{
		RtlZeroMemory(pDescValueBuffer, descValueDataSize);
	}

	hr = BluetoothGATTGetDescriptorValue(
		bleDeviceContext.getBleServiceHandle(),
		pGattDescriptor,
		(ULONG)descValueDataSize,
		pDescValueBuffer,
		NULL,
		BLUETOOTH_GATT_FLAG_NONE);

	if (S_OK != hr)
	{
		stringstream msg;
		msg << "Unable to read the descriptor value size. Reason: ["
			<< Util.getLastError(hr) << "]";

		throw new BleException(msg.str());
	}
	
	return new BleGattDescriptorValue(pDescValueBuffer);
}
