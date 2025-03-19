#include "MockGNSS.h"

// Define fake functions for GNSS
DEFINE_FAKE_VALUE_FUNC(bool, GNSS_begin, int);
DEFINE_FAKE_VALUE_FUNC(bool, GNSS_isConnected);
DEFINE_FAKE_VALUE_FUNC(bool, GNSS_setI2COutput, uint8_t);
DEFINE_FAKE_VALUE_FUNC(bool, GNSS_setNavigationFrequency, uint8_t);
DEFINE_FAKE_VALUE_FUNC(long, GNSS_getLatitude);
DEFINE_FAKE_VALUE_FUNC(long, GNSS_getLongitude);
DEFINE_FAKE_VALUE_FUNC(long, GNSS_getAltitude);
DEFINE_FAKE_VALUE_FUNC(uint8_t, GNSS_getSIV);
DEFINE_FAKE_VALUE_FUNC(uint32_t, GNSS_getTimeOfWeek);
DEFINE_FAKE_VALUE_FUNC(uint16_t, GNSS_getYear);
DEFINE_FAKE_VALUE_FUNC(uint8_t, GNSS_getMonth);
DEFINE_FAKE_VALUE_FUNC(uint8_t, GNSS_getDay);
DEFINE_FAKE_VALUE_FUNC(uint8_t, GNSS_getHour);
DEFINE_FAKE_VALUE_FUNC(uint8_t, GNSS_getMinute);
DEFINE_FAKE_VALUE_FUNC(uint8_t, GNSS_getSecond);