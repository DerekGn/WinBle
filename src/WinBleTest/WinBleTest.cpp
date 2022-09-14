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

#include <Utility.h>
#include <BleDevice.h>
#include <BleException.h>
#include <BleGattServices.h>
#include <BleDeviceEnumerator.h>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include "WinBleTest.h"

using namespace std;

//{49535343-FE7D-4AE5-8FA9-9FAFD205E455}
static const GUID UUID_SERIAL_SERVICE = { 0x49535343, 0xFE7D, 0x4AE5,{ 0x8F, 0xA9, 0x9F, 0xAF, 0xD2, 0x05, 0xE4, 0x55 } };

// {49535343-8841-43F4-A8D4-ECBE34729BB3}
static const GUID UUID_RX_CHARACTERISTIC = { 0x49535343, 0x8841, 0x43F4,{ 0xA8, 0xD4, 0xEC, 0xBE, 0x34, 0x72, 0x9B, 0xB3 } };

// {49535343-1E4D-4BD9-BA61-23C647249616}
static const GUID UUID_TX_CHARACTERISTIC = { 0x49535343, 0x1E4D, 0x4BD9,{ 0xBA, 0x61, 0x23, 0xC6, 0x47, 0x24, 0x96, 0x16 } };

void HandleCallback(BleGattNotificationData& data)
{
	cout << "Received callback data: " << data.getDataSize() << "Value: " << data.getData() << endl;
}

void readCharacteristicValueAndDisplay(BleGattService::BleGattCharacteristics const &characteristics, USHORT uuid)
{
	auto cit = find_if(begin(characteristics), end(characteristics), [&](shared_ptr<BleGattCharacteristic> const &c)
	{
		return c->getCharacteristicUuid().Value.ShortUuid == uuid;
	});

	if (cit != characteristics.end())
	{
		auto characteristic = cit->get();

		BleGattCharacteristicValue value = characteristic->getValue();

		cout << "Value :" << value.getData() << endl;
	}
}

void displayServices(BleDevice const &bleDevice)
{
	cout << "BleGattServices Count: " << bleDevice.getBleGattServices().size() << endl;

	for (shared_ptr<BleGattService> const& s : bleDevice.getBleGattServices())
	{
		cout << "\tService - Guid: ["
			<< Utility::guidToString(s->getServiceUuid().Value.LongUuid) << "]"
			<< setbase(16)
			<< " Short Id: [" << s->getServiceUuid().Value.ShortUuid << "]" << endl;

		s->enumerateBleCharacteristics();

		for (shared_ptr<BleGattCharacteristic> const& c : s->getBleCharacteristics())
		{
			cout << "\t\tCharacteristic - Guid: ["
				<< Utility::guidToString(c->getCharacteristicUuid().Value.LongUuid) << "]"
				<< setbase(16)
				<< " Short Id: [" << c->getCharacteristicUuid().Value.ShortUuid << "]" << endl;

			cout << "\t\t\tIsBroadcastable: " << +c->getIsBroadcastable() << endl
				<< "\t\t\tIsIndicatable: " << +c->getIsIndicatable() << endl
				<< "\t\t\tIsNotifiable: " << +c->getIsNotifiable() << endl
				<< "\t\t\tIsReadable: " << +c->getIsReadable() << endl
				<< "\t\t\tIsSignedWritable: " << +c->getIsSignedWritable() << endl
				<< "\t\t\tIsWritable: " << +c->getIsWritable() << endl
				<< "\t\t\tIsWritableWithoutResponse:" << +c->getIsWritableWithoutResponse() << endl;
		}
	}
}

void displayDeviceInformation(BleDevice::BleGattServices const& services)
{
	auto it = find_if(begin(services), end(services), [&](shared_ptr<BleGattService> const& s)
	{
		return s->getServiceUuid().Value.ShortUuid == GATT_UUID_DEVICE_INFO_SVC;
	});

	if (it != services.end())
	{
		auto service = it->get();

		cout << "Found Device Information Service: [" << service->getServiceUuid().Value.ShortUuid << "]" << endl;

		BleGattService::BleGattCharacteristics const& characteristics = service->getBleCharacteristics();

		readCharacteristicValueAndDisplay(characteristics, GATT_UUID_MANU_NAME);

		readCharacteristicValueAndDisplay(characteristics, GATT_UUID_SW_VERSION_STR);

		readCharacteristicValueAndDisplay(characteristics, GATT_UUID_MODEL_NUMBER_STR);
	}
}

void testSerialService(BleDevice::BleGattServices const& services)
{
	auto it = find_if(begin(services), end(services), [&](shared_ptr<BleGattService> const& s)
	{
		return s->getServiceUuid().Value.LongUuid == UUID_SERIAL_SERVICE;
	});

	if (it != services.end())
	{
		auto service = it->get();

		cout << "Found Serial Service: [" << setbase(16) << Utility::guidToString(service->getServiceUuid().Value.LongUuid) << "]" << endl;

		BleGattService::BleGattCharacteristics const& characteristics = service->getBleCharacteristics();

		auto txit = find_if(begin(characteristics), end(characteristics), [&](shared_ptr<BleGattCharacteristic> const& c)
		{
			return c->getCharacteristicUuid().Value.LongUuid == UUID_TX_CHARACTERISTIC;
		});

		if (txit != characteristics.end())
		{
			auto characteristic = txit->get();

			cout << "Found Serial Rx characteristic: "
				<< setbase(16) << "["
				<< Utility::guidToString(characteristic->getCharacteristicUuid().Value.LongUuid)
				<< "] writing to characteristic" << endl;

			UCHAR values[] = { 'H', 'I', '\r', '\n' };
			characteristic->setValue(values, 4);

			const std::function<void(BleGattNotificationData&)> callback = HandleCallback;

			cout << "Enumerating Ble Descriptors" << endl;

			characteristic->enumerateBleDescriptors();

			//get descriptor
			BleGattCharacteristic::BleGattDescriptors const& descriptors = characteristic->getBleDescriptors();

			if (descriptors.size() > 0)
			{
				BleGattDescriptor* descriptor = descriptors.front().get();

				BleGattDescriptorValue const* descriptorValue = descriptor->getValue();

				characteristic->registerNotificationHandler(callback);

				descriptor->setIsSubscribeToNotification();

				cout << "Press any key to continue" << endl;

				std::cin.get();

				characteristic->unregisterNotificationHandler();

				descriptor->clearIsSubscribeToNotification();
			}
			else
			{
				cout << "Unable to find descriptors for serial characteristic [" << Utility::guidToString(UUID_TX_CHARACTERISTIC) << "]" << endl;
			}
		}
		else
		{
			cout << "Unable to find serial characteristic [" << Utility::guidToString(UUID_TX_CHARACTERISTIC) << "]" << endl;
		}
	}
}

int main()
{
	try
	{
		BleDeviceEnumerator::enumerate();

		for (BleDeviceInfo const *i : BleDeviceEnumerator::getBleDevices())
			wcout << "Name: " << i->getName()
			<< " HardwareId: " << i->getHardwareId()
			<< " InstanceId: " << i->getInstanceId() << endl;
		
		if (BleDeviceEnumerator::getBleDevices().size() >= 1)
		{
			cout << "Opening device" << endl;

			auto bleDevice = BleDevice(BleDeviceEnumerator::getBleDevices().front()->getInstanceId());

			bleDevice.enumerateBleServices();

			displayServices(bleDevice);

			BleDevice::BleGattServices const &services = bleDevice.getBleGattServices();

			displayDeviceInformation(services);

			testSerialService(services);
		}
		else
		{
			cout << "No devices found" << endl;
		}
	}
	catch (BleException const &e)
	{
		cout << endl << "An error occurred:" << endl;
		cout << "  [" << e.what() << "]" << endl;
	}
}
