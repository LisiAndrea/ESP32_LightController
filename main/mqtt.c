
#include "json.c"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_system.h"
#include "mqtt_client.h"
#include "driver/gpio.h"

bool A_state = false; 
bool B_state = false;
bool C_state = false;
bool D_state = false;
char rTopic[TOPIC_MAX_LEN];
int msg_id;
char *response;
char rAction[20];
bool req;
esp_mqtt_client_handle_t client;

void init_relays(){
  gpio_set_direction(GPIO_RELAYA, GPIO_MODE_OUTPUT);
  gpio_set_direction(GPIO_RELAYB, GPIO_MODE_OUTPUT);
  gpio_set_direction(GPIO_RELAYC, GPIO_MODE_OUTPUT);
  gpio_set_direction(GPIO_RELAYD, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_RELAYA, A_state);
  gpio_set_level(GPIO_RELAYB, B_state);
  gpio_set_level(GPIO_RELAYC, C_state);
  gpio_set_level(GPIO_RELAYD, D_state);
}


void manage_relay(esp_mqtt_event_handle_t ev){
    client = ev->client;
    
    strncpy(rTopic, ev->topic + TOPIC_START_LEN, TOPIC_MAX_LEN);
    if (strncmp(rTopic, "RELAY", 5) == 0){
        strncpy(rAction, ev->data, (size_t)ev->data_len);
        req = parse_json(rAction);
        strncpy(rTopic, rTopic + 6, (size_t)7);
        if(strncmp(rTopic, "A", 1) == 0){
            if(req != A_state){
                printf("REQ: %d", req);
                printf("A_STATE: %d", A_state);
                A_state = !A_state;
                gpio_set_level(GPIO_RELAYA, A_state);
            }
        }else if(strncmp(rTopic, "B", 1) == 0){
            if(req != B_state){
                B_state = !B_state;
                gpio_set_level(GPIO_RELAYB, B_state);
            }
        }else if(strncmp(rTopic, "C", 1) == 0){
            if(req != C_state){
                C_state = !C_state;
                gpio_set_level(GPIO_RELAYC, C_state);
            }
        }else if(strncmp(rTopic, "D", 1) == 0){
            if(req != D_state){
                D_state = !D_state;
                gpio_set_level(GPIO_RELAYD, D_state);
            }
        }else if(strncmp(rTopic, "Z", 1) == 0){
            if(req != A_state){
                A_state = !A_state;
                gpio_set_level(GPIO_RELAYA, A_state);
            }
            vTaskDelay(  500 / portTICK_PERIOD_MS );
            if(req != B_state){
                B_state = !B_state;
                gpio_set_level(GPIO_RELAYB, B_state);
            }
            vTaskDelay(  500 / portTICK_PERIOD_MS );
            if(req != C_state){
                C_state = !C_state;
                gpio_set_level(GPIO_RELAYC, C_state);
            }
            vTaskDelay(  500 / portTICK_PERIOD_MS );
            if(req != D_state){
                D_state = !D_state;
                gpio_set_level(GPIO_RELAYD, D_state);
            }
        }else{
            printf("Unknownd topic: %s", rTopic);
            goto END_REQ;
        }
    }else if(strncmp(rTopic, "SYS", 3) == 0){
        strncpy(rTopic, rTopic + 4, (size_t)11);
        if(strncmp(rTopic, "RESTART", 7) == 0){
            printf("SYS_RESTART");
            esp_restart();
        }else if(strncmp(rTopic, "DETAIL", 6) == 0){
            rStatus->A = A_state;
            rStatus->B = B_state;
            rStatus->C = C_state;
            rStatus->D = D_state;
            response = build_json();
            msg_id = esp_mqtt_client_publish(client, WRITE_SYS_TOPIC, response, 0, 0, 0);
        }else{
            printf("Undefined SYS topic: %s", rTopic);
            goto END_REQ;
        }
    }else{
        printf("Undefined topic: %s", rTopic);
        goto END_REQ;
    }

    END_REQ:
        memset(rTopic, 0, sizeof rTopic);
        memset(rAction, 0, sizeof rAction);
}


static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    client = event->client;
    
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            uint8_t array_size = sizeof(sub_topic) / sizeof(sub_topic[0]);
            for(int i = 0; i<array_size; i++){
                printf(sub_topic[i]);
                msg_id = esp_mqtt_client_subscribe(client, sub_topic[i], 0);
                ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            }
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            vTaskDelay(  30000 / portTICK_PERIOD_MS );
            esp_restart();
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            vTaskDelay(  30000 / portTICK_PERIOD_MS );
            esp_restart();
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            manage_relay(event);
            break;
        case MQTT_EVENT_ANY:
            printf("ANY EVENT\r\n");
            break;
        case MQTT_EVENT_BEFORE_CONNECT:
            printf("MQTT_EVENT_BEFORE_CONNECT\r\n");
            break;
        case MQTT_EVENT_DELETED:
            printf("MQTT_EVENT_DELETED\r\n");
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            vTaskDelay(  30000 / portTICK_PERIOD_MS );
            esp_restart();
            break;
    }
    return ESP_OK;
}

static void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = MQTT_BROKER_URL,
        .username = MQTT_CLIENT_NAME,
        .password = MQTT_CLIENT_PWD,
        .event_handle = mqtt_event_handler,
        .port = 1883
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    //esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}


void mqtt_main(){
    init_relays();
    mqtt_app_start();
}
