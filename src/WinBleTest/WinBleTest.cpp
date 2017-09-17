using namespace std;

#include <Utility.h>
#include <BleDevice.h>
#include <BleException.h>
#include <BleGattServices.h>
#include <BleDeviceEnumerator.h>

#include <algorithm>
#include <iostream>
#include <iomanip>

//{49535343-FE7D-4AE5-8FA9-9FAFD205E455}
static const GUID UUID_SERIAL_SERVICE = { 0x49535343, 0xFE7D, 0x4AE5,{ 0x8F, 0xA9, 0x9F, 0xAF, 0xD2, 0x05, 0xE4, 0x55 } };

// {49535343-8841-43F4-A8D4-ECBE34729BB3}
static const GUID UUID_TX_CHARACTERISTIC = { 0x49535343, 0x8841, 0x43F4,{ 0xA8, 0xD4, 0xEC, 0xBE, 0x34, 0x72, 0x9B, 0xB3 } };

// {49535343-1E4D-4BD9-BA61-23C647249616}
static const GUID UUID_RX_CHARACTERISTIC = { 0x49535343, 0x1E4D, 0x4BD9,{ 0xBA, 0x61, 0x23, 0xC6, 0x47, 0x24, 0x96, 0x16 } };

void HandleCallback(BleGattNotificationData& data)
{
	cout << "Recieved callback data: " << data.getDataSize() << endl;
}

int main()
{
	try
	{
		BleEnumerator.enumerate();

		for (BleDeviceInfo *i : BleEnumerator.getBleDevices())
			wcout << "Name: " << i->getName()
			<< " HardwareId: " << i->getHardwareId()
			<< " InstanceId: " << i->getInstanceId() << endl;
		
		if (BleEnumerator.getBleDevices().size() >= 1)
		{
			cout << "Opening device" << endl;

			BleDevice bleDevice = BleDevice(BleEnumerator.getBleDevices().front()->getInstanceId());

			cout << "BleGattServices Count: " << bleDevice.getBleGattServices().size() << endl;

			for (BleGattService *s : bleDevice.getBleGattServices())
			{
				cout << "\tService - Guid: " << Util.guidToString(s->getServiceUuid().Value.LongUuid) << setbase(16)
					<< " Short Id:" << s->getServiceUuid().Value.ShortUuid << endl;

				for (BleGattCharacteristic *c : s->getBleCharacteristics())
				{
					cout << "\t\tCharacteristic - Guid: " << Util.guidToString(c->getCharacteristicUuid().Value.LongUuid)
						<< setbase(16) << " Short Id:" << c->getCharacteristicUuid().Value.ShortUuid << endl;

					cout << "\t\t\tIsBroadcastable: " << +c->getIsBroadcastable() << endl
						<< "\t\t\tIsIndicatable: " << +c->getIsIndicatable() << endl
						<< "\t\t\tIsNotifiable: " << +c->getIsNotifiable() << endl
						<< "\t\t\tIsReadable: " << +c->getIsReadable() << endl
						<< "\t\t\tIsSignedWritable: " << +c->getIsSignedWritable() << endl
						<< "\t\t\tIsWritable: " << +c->getIsWritable() << endl
						<< "\t\t\tIsWritableWithoutResponse:" << +c->getIsWritableWithoutResponse() << endl;
				}
			}

			list<BleGattService*> services = bleDevice.getBleGattServices();

			auto it = find_if(begin(services), end(services), [&](BleGattService *s)
			{
				return s->getServiceUuid().Value.ShortUuid == GATT_UUID_DEVICE_INFO_SVC;
			});

			if (it != services.end())
			{
				BleGattService* service = ((BleGattService*)*it);

				cout << "Found Device Information Service: " << service->getServiceUuid().Value.ShortUuid << endl;
				list<BleGattCharacteristic *> characteristics = service->getBleCharacteristics();

				auto cit = find_if(begin(characteristics), end(characteristics), [&](BleGattCharacteristic *c) 
				{
					return c->getCharacteristicUuid().Value.ShortUuid == GATT_UUID_MANU_NAME;
				});

				if (cit != characteristics.end())
				{
					BleGattCharacteristic * characteristic = ((BleGattCharacteristic*)*cit);

					BleGattCharacteristicValue value = characteristic->getValue();

					cout << "Manuf :" << value.getData() << endl;
				}
			}

			it = find_if(begin(services), end(services), [&](BleGattService *s)
			{
				return s->getServiceUuid().Value.LongUuid == UUID_SERIAL_SERVICE;
			});

			if (it != services.end())
			{
				BleGattService* service = ((BleGattService*)*it);

				cout << "Found Serial Service: " << setbase(16) << service->getServiceUuid().Value.ShortUuid << endl;

				list<BleGattCharacteristic *> characteristics = service->getBleCharacteristics();

				auto cit = find_if(begin(characteristics), end(characteristics), [&](BleGattCharacteristic *c)
				{
					return c->getCharacteristicUuid().Value.LongUuid == UUID_RX_CHARACTERISTIC;
				});

				if (cit != characteristics.end())
				{
					cout << "Found Serial Rx characteristic: " << setbase(16) << service->getServiceUuid().Value.ShortUuid << endl;

					BleGattCharacteristic * characteristic = ((BleGattCharacteristic*)*cit);

					std::function<void(BleGattNotificationData&)> callback = HandleCallback;

					characteristic->registerCallback(callback);
				}

				cit = find_if(begin(characteristics), end(characteristics), [&](BleGattCharacteristic *c)
				{
					return c->getCharacteristicUuid().Value.LongUuid == UUID_TX_CHARACTERISTIC;
				});

				if (cit != characteristics.end())
				{
					cout << "Found Serial Tx characteristic: " << setbase(16) << service->getServiceUuid().Value.ShortUuid << endl;

					BleGattCharacteristic * characteristic = ((BleGattCharacteristic*)*cit);
				}
			}
		}
		else
		{
			cout << "No devices found" << endl;
		}
	}
	catch (BleException * e)
	{
		cout << endl << "An error occurred:" << endl;
		cout << "  [" << e->Message() << "]" << endl;
		delete e;
	}
}



