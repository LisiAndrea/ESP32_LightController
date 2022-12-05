#include <stdbool.h>

#define EXAMPLE_ESP_WIFI_SSID      "SHOME-800"//CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      "testnetwork"//CONFIG_ESP_WIFI_PASSWORD
#define HOSTNAME                   "ACT01"
#define EXAMPLE_ESP_MAXIMUM_RETRY  20//CONFIG_ESP_MAXIMUM_RETRY
#define WIFI_CONNECTED_BIT BIT0     // WIFI connected
#define WIFI_FAIL_BIT      BIT1     // wifi not connected after max retries 

#define RELAYA_TOPIC               
#define RELAYB_TOPIC               
#define RELAYC_TOPIC               
#define RELAYD_TOPIC              
#define RELAYALL_TOPIC             


#define MQTT_CLIENT_NAME    "act01"
#define MQTT_CLIENT_PWD     "pwd"
#define MQTT_BROKER_URL     "mqtt://192.168.8.206:1883"

#define TOPIC_MAX_LEN 19
#define TOPIC_START_LEN 8

#define GPIO_RELAYA   21
#define GPIO_RELAYB   19
#define GPIO_RELAYC   18
#define GPIO_RELAYD   5

#define WRITE_SYS_TOPIC     "/ACT/01/SYS/RESPONSE"
// static data
/* FreeRTOS event group to signal when we are connected*/

static const char* TAG = "ACT01";
static char * sub_topic[] = {
    "/ACT/01/RELAY/A",
    "/ACT/01/RELAY/B",
    "/ACT/01/RELAY/C",
    "/ACT/01/RELAY/D",
    "/ACT/01/RELAY/Z",
    "/ACT/01/SYS/RESTART",
    "/ACT/01/SYS/DETAIL"
};

struct relay_status {
    bool A;
    bool B;
    bool C;
    bool D;
    char *details;
} relay_status; 
static struct relay_status *rStatus = &relay_status;