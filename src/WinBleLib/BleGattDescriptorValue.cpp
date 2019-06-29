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

BleGattDescriptorValue::BleGattDescriptorValue(PBTH_LE_GATT_DESCRIPTOR_VALUE pGattDescriptorValue)
{
	if (!pGattDescriptorValue)
		throw BleException("pGattDescriptorValue is nullptr");

	_pGattDescriptorValue = pGattDescriptorValue;
}

BleGattDescriptorValue::~BleGattDescriptorValue()
{
	if (_pGattDescriptorValue)
		free(_pGattDescriptorValue);
}

BTH_LE_GATT_DESCRIPTOR_TYPE BleGattDescriptorValue::GetDescriptorType()
{
	return _pGattDescriptorValue->DescriptorType;
}

BTH_LE_UUID BleGattDescriptorValue::getDescriptorUuid()
{
	return _pGattDescriptorValue->DescriptorUuid;
}

BOOLEAN BleGattDescriptorValue::getIsReliableWriteEnabled()
{
	return _pGattDescriptorValue->CharacteristicExtendedProperties.IsReliableWriteEnabled;
}

BOOLEAN BleGattDescriptorValue::getIsAuxiliariesWritable()
{
	return _pGattDescriptorValue->CharacteristicExtendedProperties.IsAuxiliariesWritable;
}

BOOLEAN BleGattDescriptorValue::getIsSubscribeToNotification()
{
	return _pGattDescriptorValue->ClientCharacteristicConfiguration.IsSubscribeToNotification;
}

BOOLEAN BleGattDescriptorValue::getIsSubscribeToIndication()
{
	return _pGattDescriptorValue->ClientCharacteristicConfiguration.IsSubscribeToIndication;
}

BOOLEAN BleGattDescriptorValue::getIsBroadcast()
{
	return _pGattDescriptorValue->ServerCharacteristicConfiguration.IsBroadcast;
}

unsigned char BleGattDescriptorValue::getFormat()
{
	return _pGattDescriptorValue->CharacteristicFormat.Format;
}

unsigned char BleGattDescriptorValue::getExponent()
{
	return _pGattDescriptorValue->CharacteristicFormat.Exponent;
}

BTH_LE_UUID BleGattDescriptorValue::getUnit()
{
	return _pGattDescriptorValue->CharacteristicFormat.Unit;
}

unsigned char BleGattDescriptorValue::getNameSpace()
{
	return _pGattDescriptorValue->CharacteristicFormat.NameSpace;
}

BTH_LE_UUID BleGattDescriptorValue::getDescription()
{
	return _pGattDescriptorValue->CharacteristicFormat.Description;
}

unsigned long BleGattDescriptorValue::getDataSize()
{
	return _pGattDescriptorValue->DataSize;
}

unsigned char* BleGattDescriptorValue::getData()
{
	return _pGattDescriptorValue->Data;
}

PBTH_LE_GATT_DESCRIPTOR_VALUE BleGattDescriptorValue::getValue()
{
	return _pGattDescriptorValue;
}
