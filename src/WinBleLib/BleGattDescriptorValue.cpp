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

#include "BleGattDescriptorValue.h"
#include "BleException.h"
#include "Utility.h"

BleGattDescriptorValue::BleGattDescriptorValue(PBTH_LE_GATT_DESCRIPTOR_VALUE _pGattDescriptorValue)
{
	if (!_pGattDescriptorValue)
		throw new BleException("pGattDescriptorValue is nullptr");

	pGattDescriptorValue = _pGattDescriptorValue;
}

BleGattDescriptorValue::~BleGattDescriptorValue()
{
	if (pGattDescriptorValue)
		free(pGattDescriptorValue);
}

BTH_LE_GATT_DESCRIPTOR_TYPE BleGattDescriptorValue::GetDescriptorType()
{
	return pGattDescriptorValue->DescriptorType;
}

BTH_LE_UUID BleGattDescriptorValue::getDescriptorUuid()
{
	return pGattDescriptorValue->DescriptorUuid;
}

BOOLEAN BleGattDescriptorValue::getIsReliableWriteEnabled()
{
	return pGattDescriptorValue->CharacteristicExtendedProperties.IsReliableWriteEnabled;
}

BOOLEAN BleGattDescriptorValue::getIsAuxiliariesWritable()
{
	return pGattDescriptorValue->CharacteristicExtendedProperties.IsAuxiliariesWritable;
}

BOOLEAN BleGattDescriptorValue::getIsSubscribeToNotification()
{
	return pGattDescriptorValue->ClientCharacteristicConfiguration.IsSubscribeToNotification;
}

BOOLEAN BleGattDescriptorValue::getIsSubscribeToIndication()
{
	return pGattDescriptorValue->ClientCharacteristicConfiguration.IsSubscribeToIndication;
}

BOOLEAN BleGattDescriptorValue::getIsBroadcast()
{
	return pGattDescriptorValue->ServerCharacteristicConfiguration.IsBroadcast;
}

unsigned char BleGattDescriptorValue::getFormat()
{
	return pGattDescriptorValue->CharacteristicFormat.Format;
}

unsigned char BleGattDescriptorValue::getExponent()
{
	return pGattDescriptorValue->CharacteristicFormat.Exponent;
}

BTH_LE_UUID BleGattDescriptorValue::getUnit()
{
	return pGattDescriptorValue->CharacteristicFormat.Unit;
}

unsigned char BleGattDescriptorValue::getNameSpace()
{
	return pGattDescriptorValue->CharacteristicFormat.NameSpace;
}

BTH_LE_UUID BleGattDescriptorValue::getDescription()
{
	return pGattDescriptorValue->CharacteristicFormat.Description;
}

unsigned long BleGattDescriptorValue::getDataSize()
{
	return pGattDescriptorValue->DataSize;
}

unsigned char* BleGattDescriptorValue::getData()
{
	return pGattDescriptorValue->Data;
}
