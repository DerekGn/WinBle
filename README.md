# WinBle

A set of classes for enumerating and interacting with Bluetooth low energy devices on the windows platform.

## Requirements

* Windows 10
* Devices must be paired and enumerated by the windows ble driver

## Example Code

### Read a Characteristic

``` csharp
// The characteristic must have IsReadable set to true or an exception will be thrown
BleGattCharacteristicValue value = characteristic->getValue();
```

### Write a Characteristic Value

``` csharp
// The characteristic must have IsSignedWritable, IsWritable, IsWritableWithoutResponse set to true or an exception will be thrown
UCHAR values[] = { 'H', 'I', '\r', '\n' };

characteristic->setValue(values, 4);
```

### Subscribe to a Characteristic Notification

``` csharp
// The callback for the characteristic notification
void HandleCallback(BleGattNotificationData& data)
{
	cout << "Recieved callback data: " << data.getDataSize() << endl;
}

// the callback function
const std::function<void(BleGattNotificationData&)> callback = HandleCallback;

// enable notifications for the characteristic IsNotifiable or IsIndicatable must be true for the characteristic or an exception will be thrown
characteristic->enableNotifications(callback);

// disable the notification
characteristic->disableNotifications();
```
