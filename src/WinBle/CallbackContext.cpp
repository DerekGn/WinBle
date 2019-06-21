#include "CallbackContext.h"

CallbackContext::CallbackContext(function<void(BleGattNotificationData&)> notificationHandler, 
	PBTH_LE_GATT_CHARACTERISTIC pGattCharacteristic) :
	_notificationHandler(_notificationHandler)
{	
}

CallbackContext::~CallbackContext()
{
}

function<void(BleGattNotificationData&)> CallbackContext::getNotificationHandler()
{
	return _notificationHandler;
}

PBTH_LE_GATT_CHARACTERISTIC CallbackContext::getGattCharacteristic()
{
	return _pGattCharacteristic;
}
