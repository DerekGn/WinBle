# WinBle

A set of classes for enumerating and interacting with Bluetooth low energy devices on the windows platform.

## Requirements

* Windows 10
* Devices must be paired and enumerated by the windows ble driver

## Example Code

### Read a Characteristic

``` cpp
// The characteristic must have IsReadable set to true or an exception will be thrown
BleGattCharacteristicValue value = characteristic->getValue();
```

### Write a Characteristic Value

``` cpp
// The characteristic must have IsSignedWritable, IsWritable, IsWritableWithoutResponse set to true or an exception will be thrown
UCHAR values[] = { 'H', 'I', '\r', '\n' };

characteristic->setValue(values, 4);
```

### Subscribe to a Characteristic Notification

``` cpp
// The callback for the characteristic notification
void HandleCallback(BleGattNotificationData& data)
{
    cout << "Received callback data: " << data.getDataSize() << endl;
}

// Enumerate characteristic descriptors
characteristic->enumerateBleDescriptors();

// Get the list of descriptors
list<BleGattDescriptor *> descriptors = characteristic->getBleDescriptors();

if (descriptors.size() > 0)
{
    // Get the characteristic descriptor
    BleGattDescriptor *descriptor = descriptors.front();

    // The callback function
    const std::function<void(BleGattNotificationData&)> callback = HandleCallback;

    // Enable notifications for the characteristic IsNotifiable or IsIndicatable must be true for the characteristic or an exception will be thrown
    characteristic->enableNotifications(callback);

    // Enable the subscription notification for the characteristic
    descriptor->setIsSubscribeToNotification();

    // Disable the notification
    characteristic->disableNotifications();

    // Clear the subscription notification
    descriptor->clearIsSubscribeToNotification();
}
```
# Windows Bluetooth Tracing

Windows Bluetooth tracing requires the download of a WPRP file from the Microsoft Git hub repo [eere](https://github.com/microsoft/busiotools/tree/master/bluetooth/tracing). The readme.md document in the Microsoft Git hub repo describes the steps to enable the tracing.

When this tracing is complete the stop command will write the trace contents to an ETL file that is provided as a parameter to the command.

The generated ETL file is a binary file in order to parse it to a readable form, a tool called BTETLParse.exe is required to convert the ETL. The tool supports a number of different formats of output, namely:

- CFA
    - Can be opened with Teledyne LeCroy Wireless Protocol Suite 
- HCI
    - This is a plain text file
- PCAP
    - This can be opened with wireshark

Instructions for the BTETLParse.exe tool can be found [Here](https://docs.microsoft.com/en-us/windows-hardware/drivers/bluetooth/testing-btp-tools-btetlparse)

The BTETLParse.exe tool can be downloaded from [Here](https://docs.microsoft.com/en-us/windows-hardware/drivers/bluetooth/testing-btp-software-package)
