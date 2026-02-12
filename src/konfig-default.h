

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define WEB_SERVER_PASSWORD ""

#define PIN_OUT1 13
#define PIN_OUT2 15
#define PIN_SDA 4
#define PIN_SCL 5

#define DEBUG

#define RUN_INTERVAL 1000
#define BAUD_RATE 115200


#define PEMANAS PIN_OUT1
#define KIPAS PIN_OUT2

// #define INVERT_OUT
#ifdef INVERT_OUT
#define NYALA LOW
#define MATI HIGH
#else
#define NYALA HIGH
#define MATI LOW
#endif
