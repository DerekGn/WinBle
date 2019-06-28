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

#ifndef BLEGATTSERVICES_H
#define BLEGATTSERVICES_H

#define GATT_UUID_IMMEDIATE_ALERT_SVC           0x1802          /*  Immediate alert Service*/
#define GATT_UUID_LINK_LOSS_SVC                 0x1803          /*  Link Loss Service*/                             
#define GATT_UUID_TX_POWER_SVC                  0x1804          /*  TX Power Service*/
#define GATT_UUID_CURRENT_TIME_SVC              0x1805          /*  Current Time Service Service*/
#define GATT_UUID_REF_TIME_UPDATE_SVC           0x1806          /*  Reference Time Update Service*/
#define GATT_UUID_NEXT_DST_CHANGE_SVC           0x1807          /*  Next DST Change Service*/
#define GATT_UUID_GLUCOSE_SVC                   0x1808          /*  Glucose Service*/
#define GATT_UUID_HEALTH_THERMOM_SVC            0x1809          /*  Health Thermometer Service*/
#define GATT_UUID_DEVICE_INFO_SVC               0x180A          /*  Device Information Service*/
#define GATT_UUID_HEART_RATE_SVC                0x180D          /*  Heart Rate Service*/
#define GATT_UUID_PHONE_ALERT_STATUS_SVC        0x180E          /* Phone Alert Status Service*/
#define GATT_UUID_BATTERY_SERVICE_SVC           0x180F          /* Battery Service*/
#define GATT_UUID_BLOOD_PRESSURE_SVC            0x1810          /* Blood Pressure Service*/
#define GATT_UUID_ALERT_NTF_SVC                 0x1811          /* Alert Notification Service*/
#define GATT_UUID_HID_SVC                       0x1812          /* HID Service*/
#define GATT_UUID_SCAN_PARAMETERS_SVC           0x1813          /* Scan Parameters Service*/
#define GATT_UUID_RUNNING_SPEED_CADENCE_SVC     0x1814          /* Running Speed and Cadence Service*/
#define GATT_UUID_CYCLING_SPEED_CADENCE_SVC     0x1816          /* Cycling Speed and Cadence Service*/
#define GATT_UUID_CYCLING_POWER_SVC             0x1818          /* Cycling Power Service*/
#define GATT_UUID_LOCATION_AND_NAVIGATION_SVC   0x1819          /* Location and Navigation Service*/
#define GATT_UUID_USER_DATA_SVC                 0x181C          /* User Data Service*/
#define GATT_UUID_WEIGHT_SCALE_SVC              0x181D          /* Weight Scale Service*/

#define GATT_UUID_PRI_SERVICE                   0x2800
#define GATT_UUID_SEC_SERVICE                   0x2801
#define GATT_UUID_INCLUDE_SERVICE               0x2802
#define GATT_UUID_CHAR_DECLARE                  0x2803          /*  Characteristic Declaration*/

#define GATT_UUID_CHAR_EXT_PROP                 0x2900          /*  Characteristic Extended Properties */
#define GATT_UUID_CHAR_DESCRIPTION              0x2901          /*  Characteristic User Description*/
#define GATT_UUID_CHAR_CLIENT_CONFIG            0x2902          /*  Client Characteristic Configuration */
#define GATT_UUID_CHAR_SRVR_CONFIG              0x2903          /*  Server Characteristic Configuration */
#define GATT_UUID_CHAR_PRESENT_FORMAT           0x2904          /*  Characteristic Presentation Format*/
#define GATT_UUID_CHAR_AGG_FORMAT               0x2905          /*  Characteristic Aggregate Format*/
#define GATT_UUID_CHAR_VALID_RANGE              0x2906          /*  Characteristic Valid Range */
#define GATT_UUID_EXT_RPT_REF_DESCR             0x2907
#define GATT_UUID_RPT_REF_DESCR                 0x2908

/* GAP Profile Attributes */
#define GATT_UUID_GAP_DEVICE_NAME               0x2A00
#define GATT_UUID_GAP_ICON                      0x2A01
#define GATT_UUID_GAP_PREF_CONN_PARAM           0x2A04
#define GATT_UUID_GAP_CENTRAL_ADDR_RESOL        0x2AA6

/* Attribute Profile Attribute UUID */
#define GATT_UUID_GATT_SRV_CHGD                 0x2A05

/* Link Loss Service */
#define GATT_UUID_ALERT_LEVEL                   0x2A06          /* Alert Level */
#define GATT_UUID_TX_POWER_LEVEL                0x2A07          /* TX power level */

/* Current Time Service */
#define GATT_UUID_CURRENT_TIME                  0x2A2B          /* Current Time */
#define GATT_UUID_LOCAL_TIME_INFO               0x2A0F          /* Local time info */
#define GATT_UUID_REF_TIME_INFO                 0x2A14          /* reference time information */

/* Network availability Profile */
#define GATT_UUID_NW_STATUS                     0x2A18          /* network availability status */
#define GATT_UUID_NW_TRIGGER                    0x2A1A          /* Network availability trigger */
#define GATT_UUID_ALERT_STATUS                  0x2A3F          /* alert status */
#define GATT_UUID_RINGER_CP                     0x2A40          /* ringer control point */
#define GATT_UUID_RINGER_SETTING                0x2A41          /* ringer setting */
#define GATT_UUID_GM_MEASUREMENT                0x2A18
#define GATT_UUID_GM_CONTEXT                    0x2A34
#define GATT_UUID_GM_CONTROL_POINT              0x2A52
#define GATT_UUID_GM_FEATURE                    0x2A51
#define GATT_UUID_SYSTEM_ID                     0x2A23
#define GATT_UUID_MODEL_NUMBER_STR              0x2A24
#define GATT_UUID_SERIAL_NUMBER_STR             0x2A25
#define GATT_UUID_FW_VERSION_STR                0x2A26
#define GATT_UUID_HW_VERSION_STR                0x2A27
#define GATT_UUID_SW_VERSION_STR                0x2A28
#define GATT_UUID_MANU_NAME                     0x2A29
#define GATT_UUID_IEEE_DATA                     0x2A2A
#define GATT_UUID_PNP_ID                        0x2A50
#define GATT_UUID_HID_INFORMATION               0x2A4A
#define GATT_UUID_HID_REPORT_MAP                0x2A4B
#define GATT_UUID_HID_CONTROL_POINT             0x2A4C
#define GATT_UUID_HID_REPORT                    0x2A4D
#define GATT_UUID_HID_PROTO_MODE                0x2A4E
#define GATT_UUID_HID_BT_KB_INPUT               0x2A22
#define GATT_UUID_HID_BT_KB_OUTPUT              0x2A32
#define GATT_UUID_HID_BT_MOUSE_INPUT            0x2A33
#define GATT_HEART_RATE_MEAS					0x2A37
#define GATT_BODY_SENSOR_LOCATION				0x2A38
#define GATT_HEART_RATE_CNTL_POINT				0x2A39
#define GATT_UUID_BATTERY_LEVEL                 0x2A19
#define GATT_UUID_SC_CONTROL_POINT              0x2A55
#define GATT_UUID_SENSOR_LOCATION               0x2A5D
#define GATT_UUID_RSC_MEASUREMENT               0x2A53
#define GATT_UUID_RSC_FEATURE                   0x2A54
#define GATT_UUID_CSC_MEASUREMENT               0x2A5B
#define GATT_UUID_CSC_FEATURE                   0x2A5C
#define GATT_UUID_SCAN_INT_WINDOW               0x2A4F
#define GATT_UUID_SCAN_REFRESH					0x2A31

#endif