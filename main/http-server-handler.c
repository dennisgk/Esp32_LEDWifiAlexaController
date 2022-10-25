#include "http-server-handler.h"
#include "led-strip-handler.h"

#define MIN(a,b) (((a)<(b))?(a):(b))

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "nvs.h"

const char* alexa_default_resp_c = "{\"version\":\"1.0\",\"response\":{\"outputSpeech\":{\"type\":\"PlainText\",\"text\":\"null\"},\"shouldEndSession\":true,\"type\":\"_DEFAULT_RESPONSE\"},\"sessionAttributes\":{}}";
char* http_default_resp = NULL;
const char* http_default_resp_c = "<!DOCTYPE html><html><head><title>Light Changer</title>\
<script>\
var lightsOn=0;\
function lightsButtonOnClick(){\
    if(lightsOn == 0){\
        window.location.href = \"/?on=t\";\
    }else{\
        window.location.href = \"/?on=f\";\
    }\
}\
\
function setColorButtonOnClick(){\
    window.location.href = \"/?col=\" + document.getElementById(\"colorInput\").value.substring(1).toUpperCase();\
}\
\
function setColorButtonDefaultOnClick(){\
    window.location.href = \"/?col=DCDCDC\";\
}\
\
function bodyLoad(){\
    if(lightsOn==1){\
        document.getElementById(\"toggleLightsButton\").innerHTML=\"Turn off\";\
    }else{\
        document.getElementById(\"toggleLightsButton\").innerHTML=\"Turn on\";\
    }\
    document.getElementById(\"toggleLightsButton\").onclick = lightsButtonOnClick;\
    document.getElementById(\"setColorButton\").onclick = setColorButtonOnClick;\
    document.getElementById(\"setColorDefaultButton\").onclick = setColorButtonDefaultOnClick;\
}\
window.onload = bodyLoad;\
</script>\
<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.0/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-gH2yIJqKdNHPEq0n4Mqa/HGKIhSkIHeL5AyhkYV8i59U5AR6csBvApHHNl/vI1Bx\" crossorigin=\"anonymous\"></head><body style=\"background-color:#c9f3ff\"> <div class=\"container h-100\"> <h1 class=\"mt-4\" style=\"font-size: calc(1.3vw + 1.3vh);\">WELCOME</h1> <p style=\"font-size: calc(0.9vw + 0.9vh);\">Spend as much time as you would like here</p><div class=\"hstack gap-2 align-items-start\"> <div class=\"w-50\"> <p style=\"font-size: calc(0.9vw + 0.9vh);\">Set light color:</p><input class=\"btn btn-dark mb-3 w-100\" style=\"height:25vh;\" type=\"color\" id=\"colorInput\" value=\"#646464\"><div class=\"d-flex\"> <button style=\"min-height:4vh;font-size: calc(0.9vw + 0.9vh);\" class=\"flex-fill btn btn-dark\" id=\"setColorButton\" type=\"button\">Apply</button><button style=\"min-height:4vh;font-size: calc(0.9vw + 0.9vh);\" class=\"flex-fill ms-2 btn btn-dark\" id=\"setColorDefaultButton\" type=\"button\">Default</button></div> </div><div class=\"w-50\"> <p style=\"font-size: calc(0.9vw + 0.9vh);\">Toggle lights:</p><button style=\"min-height:4vh;font-size: calc(0.9vw + 0.9vh);\" class=\"w-100 btn btn-dark\" id=\"toggleLightsButton\" type=\"button\">Turn </button> </div></div></div></body></html>";

/* type for alexa posts */
typedef struct {
    uint8_t intent_type;
    void* data;
    char* resp;
} alexa_skill_response;

/* URI handler structure for GET /uri */
httpd_uri_t uri_get = {
    .uri      = "/",
    .method   = HTTP_GET,
    .handler  = get_handler,
    .user_ctx = NULL
};

/* URI handler structure for GET favicon.ico override */
httpd_uri_t uri_get_favicon = {
    .uri      = "/favicon.ico",
    .method   = HTTP_GET,
    .handler  = get_favicon_handler,
    .user_ctx = NULL
};

/* URI handler structure for POST /uri */
httpd_uri_t uri_post = {
    .uri      = "/alexa",
    .method   = HTTP_POST,
    .handler  = post_handler,
    .user_ctx = NULL
};

char* get_http_response(){
    char* lights_on_var_loc = strstr(http_default_resp, "lightsOn=");
    if((lcd_current_status & LCD_STATUS_ON) != 0){
        lights_on_var_loc[9] = '1';
    }
    else{
        lights_on_var_loc[9] = '0';
    }
    
    char* default_color_val = strstr(http_default_resp, "id=\"colorInput\" value=\"#");
    char color_hex[10] = {0};
    sprintf(color_hex, "%x", lcd_current_color.r);
    sprintf(color_hex + 2, "%x", lcd_current_color.g);
    sprintf(color_hex + 4, "%x", lcd_current_color.b);

    if(color_hex[0] == 0){ color_hex[0] = '0'; }
    if(color_hex[1] == 0){ color_hex[1] = '0'; }
    if(color_hex[2] == 0){ color_hex[2] = '0'; }
    if(color_hex[3] == 0){ color_hex[3] = '0'; }
    if(color_hex[4] == 0){ color_hex[4] = '0'; }
    if(color_hex[5] == 0){ color_hex[5] = '0'; }
    
    default_color_val[24] = color_hex[0];
    default_color_val[25] = color_hex[1];
    default_color_val[26] = color_hex[2];
    default_color_val[27] = color_hex[3];
    default_color_val[28] = color_hex[4];
    default_color_val[29] = color_hex[5];

    return http_default_resp;
}

/* Our URI handler function to be called during GET /uri request */
esp_err_t get_handler(httpd_req_t *req)
{
    char query_string[80] = {0};

    esp_err_t query_string_err = httpd_req_get_url_query_str(req, query_string, 80);
    if(query_string_err == ESP_OK || query_string_err == ESP_ERR_HTTPD_RESULT_TRUNC){
        char lights_turned_on = 0;

        char param_on_value[2] = {0};
        esp_err_t param_on_err = httpd_query_key_value(query_string, "on", param_on_value, 2);
        if(param_on_err == ESP_OK || param_on_err == ESP_ERR_HTTPD_RESULT_TRUNC){
            if(param_on_value[0] == 't'){
                turn_on_led();
                lights_turned_on = 1;
            }

            if(param_on_value[0] == 'f'){
                turn_off_led();
            }
        }

        char param_col_value[7] = {0};
        esp_err_t param_col_err = httpd_query_key_value(query_string, "col", param_col_value, 7);
        if(param_col_err == ESP_OK || param_col_err == ESP_ERR_HTTPD_RESULT_TRUNC){
            union {
                int32_t i;
                uint8_t dat[4];
            } col_val = {0};
            col_val.i = strtol(param_col_value, NULL, 16);

            if(lights_turned_on){
                vTaskDelay(50 / portTICK_PERIOD_MS);
            }

            set_led_color(col_val.dat[2], col_val.dat[1], col_val.dat[0]);
        }
    }
    /* Send a simple response */
    httpd_resp_send(req, get_http_response(), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

/* Our URI handler function to be called during GET /uri request */
esp_err_t get_favicon_handler(httpd_req_t *req)
{
    /* Send a simple response */
    httpd_resp_send_404(req);
    return ESP_OK;
}

uint8_t recv_http_json_until_value_in_first_scope(httpd_req_t *req, char* lookString, int lookStringLeng){
    char letter[1];
    int scopes = 0;
    int lookStringIt = 0;
    uint8_t found = 0;

    while(httpd_req_recv(req, letter, 1) != 0){
        if(letter[0] == '{'){
            scopes++;
            continue;
        }
        if(letter[0] == '}'){
            scopes--;
            continue;
        }
        if(scopes == 1){
            //ESP_LOGI("HTTP_SERVER", "%c", letter[0]);
            if(letter[0] == lookString[lookStringIt]){
                lookStringIt++;
                if(lookStringIt >= lookStringLeng){
                    //STARTED
                    found = 1;
                    break;
                }
                continue;
            }
            else{
                lookStringIt = 0;
                continue;
            }
        }
    }

    return found;
}

char* http_recv_next_json_scope_dynamic_no_extras(httpd_req_t* req, int* leng){
    char letter[1];
    int scopes = 0;
    uint8_t found = 0;

    while(httpd_req_recv(req, letter, 1) != 0){
        if(letter[0] == '{'){
            found = 1;
            break;
        }
    }

    if(found == 0){
        return NULL;
    }

    int mallocSize = 512;
    int currentLeng = mallocSize;
    char* ret = (char*)malloc(sizeof(char) * currentLeng);
    char* it = ret;
    char* end = ret + currentLeng;
    uint8_t isInQuotes = 0;

    it[0] = '{';
    it++;

    scopes = 1;

    while(httpd_req_recv(req, letter, 1) != 0){
        if(it < end){
            if(((letter[0] != ' ' && letter[0] != 9) || isInQuotes) && letter[0] != 13 && letter[0] != 10){
                it[0] = letter[0];
                it++;
            }
        }
        else{
            int oldLeng = currentLeng;
            currentLeng += mallocSize;
            char* oldRet = ret;
            ret = (char*)malloc(sizeof(char) * currentLeng);
            memcpy(ret, oldRet, oldLeng);
            free(oldRet);
            it = ret + oldLeng;
            end = ret + currentLeng;
        }

        if(letter[0] == '\"' && letter[-1] != '\\'){
            if(isInQuotes == 0){
                isInQuotes = 1;
            }
            else{
                isInQuotes = 0;
            }
        }
        if(letter[0] == '{'){
            scopes++;
        }
        if(letter[0] == '}'){
            scopes--;
            if(scopes == 0){
                break;
            }
        }
    }

    *leng = it - ret;
    return ret;
}

char* http_json_get_value_from_cleaned(char* cleaned_json, int length, char* find_val, int find_val_leng, int* ret_leng){
    
    int find_val_it = 0;
    int scopes = 0;
    uint8_t isInQuotes = 0;
    uint8_t isBeforeColon = 1;

    char* endQuoteLoc = NULL;

    for(char* it = cleaned_json; it < cleaned_json + length; it++){
        if(it[0] == '{'){
            scopes++;
            continue;
        }
        if(it[0] == '}'){
            scopes--;
            continue;
        }

        if(scopes != 1){
            continue;
        }

        if(it[0] == '\"' && it[-1] != '\\'){
            if(isInQuotes == 0){
                isInQuotes = 1;
            }
            else{
                isInQuotes = 0;
                find_val_it = 0;
            }
            continue;
        }

        if(it[0] == ':'){
            isBeforeColon = false;
            continue;
        }

        if(it[0] == ','){
            isBeforeColon = true;
            continue;
        }

        if(it[0] == find_val[find_val_it] && isBeforeColon && isInQuotes){
            find_val_it++;
            if(find_val_it >= find_val_leng){
                if(it + 1 >= cleaned_json + length || it[1] != '\"'){
                    find_val_it = 0;
                    continue;
                }
                endQuoteLoc = it + 1;
                break;
            }
        }
        else{
            find_val_it = 0;
            continue;
        }
    }

    if(endQuoteLoc + 1 >= cleaned_json +length){
        return NULL;
    }

    char* endColonLoc = NULL;

    for(char* it = endQuoteLoc + 1; it < cleaned_json + length; it++){
        if(it[0] == ':'){
            endColonLoc = it;
            break;
        }
    }

    if(endColonLoc + 1 >= cleaned_json +length){
        return NULL;
    }

    char* beginDataType = NULL;

    for(char* it = endColonLoc + 1; it < cleaned_json + length; it++){
        if(it[0] == ' ' || it[0] == 9 || it[0] == 13 || it[0] == 10){
            continue;
        }
        beginDataType = it;
        break;
    }

    if(beginDataType[0] == '\"'){
        char* end = NULL;
        for(char* it = beginDataType + 1; it < cleaned_json + length; it++){
            if(it[0] == '\"' && it[-1] != '\\'){
                end = it;
                break;
            }
        }
        *ret_leng = end - beginDataType + 1;
        return beginDataType;
    }
    else if(beginDataType[0] == 't'){
        *ret_leng = 4;
        return beginDataType;
    }
    else if(beginDataType[0] == 'f'){
        *ret_leng = 5;
        return beginDataType;
    }
    else if(beginDataType[0] == 'n'){
        *ret_leng = 4;
        return beginDataType;
    }
    else if(beginDataType[0] == '{'){
        int int_scopes = 1;
        for(char* it = beginDataType + 1; it < cleaned_json + length; it++){
            if(it[0] == '{'){
                int_scopes++;
                continue;
            }
            if(it[0] == '}'){
                int_scopes--;
                if(int_scopes == 0){
                    *ret_leng = it - beginDataType + 1;
                    return beginDataType;
                }
                continue;
            }
        }
    }
    else if(beginDataType[0] == '['){
        int int_scopes = 1;
        for(char* it = beginDataType + 1; it < cleaned_json + length; it++){
            if(it[0] == '['){
                int_scopes++;
                continue;
            }
            if(it[0] == ']'){
                int_scopes--;
                if(int_scopes == 0){
                    *ret_leng = it - beginDataType + 1;
                    return beginDataType;
                }
                continue;
            }
        }
    }

    return NULL;

}

char* create_alexa_response(const char* text, int textLeng){
    if(textLeng > 137){
        textLeng = 137;
    }
    char* a_resp = (char*)malloc(sizeof(char) * 301);
    strncpy(a_resp, alexa_default_resp_c, 72);
    strncpy(a_resp + 72, text, textLeng);
    strncpy(a_resp + 72 + textLeng, alexa_default_resp_c + 76, 91);
    a_resp[textLeng + 163] = 0;
    return a_resp;
}

alexa_skill_response get_intent_from_post_data(httpd_req_t *req){

    alexa_skill_response resp = {0};
    resp.intent_type = 0;

    uint8_t found = recv_http_json_until_value_in_first_scope(req, "\"request\"", 9);
    if(found == 0){
        return resp;
    }
    found = recv_http_json_until_value_in_first_scope(req, "\"intent\"", 8);
    if(found == 0){
        return resp;
    }
    int intentDataLeng = 0;
    char* intentData = http_recv_next_json_scope_dynamic_no_extras(req, &intentDataLeng);
    if(intentData == NULL){
        return resp;
    }

    int intentNameDataLeng = 0;
    char* intentNameData = http_json_get_value_from_cleaned(intentData, intentDataLeng, "name", 4, &intentNameDataLeng);
    if(intentNameData == NULL){
        free(intentData);
        return resp;
    }

    if(strncmp(intentNameData, "\"ChangeColor\"", 13) == 0){
        int slotsDataLeng = 0;
        char* slotsData = http_json_get_value_from_cleaned(intentData, intentDataLeng, "slots", 5, &slotsDataLeng);
        if(slotsData == NULL){
            free(intentData);
            return resp;
        }
        int colorDataLeng = 0;
        char* colorData = http_json_get_value_from_cleaned(slotsData, slotsDataLeng, "color", 5, &colorDataLeng);
        if(colorData == NULL){
            free(intentData);
            return resp;
        }
        int specColorDataLeng = 0;
        char* specColorData = http_json_get_value_from_cleaned(colorData, colorDataLeng, "value", 5, &specColorDataLeng);
        if(specColorData == NULL){
            free(intentData);
            return resp;
        }

        if(strncmp(specColorData, "\"dim\"", 5) == 0){
            //115   72
            const char* def_resp = "Lights were set to dim";
            char* alexa_text_resp = create_alexa_response(def_resp, strlen(def_resp));
            resp.resp = alexa_text_resp;
            resp.intent_type = 1;
            uint8_t* dat = (uint8_t*)malloc(sizeof(uint8_t) * 3);
            dat[0] = 115;
            dat[1] = 115;
            dat[2] = 115;
            resp.data = (void*)dat;
            free(intentData);   
            return resp;
        }
        if(strncmp(specColorData, "\"default\"", 9) == 0){
            //220
            const char* def_resp = "Lights were set to default";
            char* alexa_text_resp = create_alexa_response(def_resp, strlen(def_resp));
            resp.resp = alexa_text_resp;
            resp.intent_type = 1;
            uint8_t* dat = (uint8_t*)malloc(sizeof(uint8_t) * 3);
            dat[0] = 220;
            dat[1] = 220;
            dat[2] = 220;
            resp.data = (void*)dat;
            free(intentData);
            return resp;
        }
        if(strncmp(specColorData, "\"blue\"", 6) == 0){
            const char* def_resp = "Lights were set to blue";
            char* alexa_text_resp = create_alexa_response(def_resp, strlen(def_resp));
            resp.resp = alexa_text_resp;
            resp.intent_type = 1;
            uint8_t* dat = (uint8_t*)malloc(sizeof(uint8_t) * 3);
            dat[0] = 0;
            dat[1] = 0;
            dat[2] = 255;
            resp.data = (void*)dat;
            free(intentData);
            return resp;
        }
        if(strncmp(specColorData, "\"green\"", 7) == 0){
            const char* def_resp = "Lights were set to green";
            char* alexa_text_resp = create_alexa_response(def_resp, strlen(def_resp));
            resp.resp = alexa_text_resp;
            resp.intent_type = 1;
            uint8_t* dat = (uint8_t*)malloc(sizeof(uint8_t) * 3);
            dat[0] = 0;
            dat[1] = 255;
            dat[2] = 0;
            resp.data = (void*)dat;
            free(intentData);
            return resp;
        }
        if(strncmp(specColorData, "\"red\"", 5) == 0){
            const char* def_resp = "Lights were set to red";
            char* alexa_text_resp = create_alexa_response(def_resp, strlen(def_resp));
            resp.resp = alexa_text_resp;
            resp.intent_type = 1;
            uint8_t* dat = (uint8_t*)malloc(sizeof(uint8_t) * 3);
            dat[0] = 255;
            dat[1] = 0;
            dat[2] = 0;
            resp.data = (void*)dat;
            free(intentData);
            return resp;
        }

        ESP_LOGI("HTTP_SERVER", "%s", specColorData);
    }

    if(strncmp(intentNameData, "\"LightIntent\"", 13) == 0){
        int slotsDataLeng = 0;
        char* slotsData = http_json_get_value_from_cleaned(intentData, intentDataLeng, "slots", 5, &slotsDataLeng);
        if(slotsData == NULL){
            free(intentData);
            return resp;
        }
        int toggledDataLeng = 0;
        char* toggledData = http_json_get_value_from_cleaned(slotsData, slotsDataLeng, "status", 6, &toggledDataLeng);
        if(toggledData == NULL){
            free(intentData);
            return resp;
        }
        int specToggledDataLeng = 0;
        char* specToggledData = http_json_get_value_from_cleaned(toggledData, toggledDataLeng, "value", 5, &specToggledDataLeng);
        if(specToggledData == NULL){
            free(intentData);
            return resp;
        }

        if(strncmp(specToggledData, "\"off\"", 5) == 0){
            const char* def_resp = "Lights were turned off";
            char* alexa_text_resp = create_alexa_response(def_resp, strlen(def_resp));
            resp.resp = alexa_text_resp;
            resp.intent_type = 2;
            uint8_t* dat = (uint8_t*)malloc(sizeof(uint8_t) * 1);
            dat[0] = 0;
            resp.data = (void*)dat;
            free(intentData);
            return resp;
        }
        if(strncmp(specToggledData, "\"on\"", 4) == 0){
            const char* def_resp = "Lights were turned on";
            char* alexa_text_resp = create_alexa_response(def_resp, strlen(def_resp));
            resp.resp = alexa_text_resp;
            resp.intent_type = 2;
            uint8_t* dat = (uint8_t*)malloc(sizeof(uint8_t) * 1);
            dat[0] = 1;
            resp.data = (void*)dat;
            free(intentData);
            return resp;
        }

        ESP_LOGI("HTTP_SERVER", "%s", specToggledData);
    }

    free(intentData);

    return resp;
}

/* Our URI handler function to be called during POST /uri request */
esp_err_t post_handler(httpd_req_t *req)
{
    alexa_skill_response intent = get_intent_from_post_data(req);

    /* Send a simple response */
    if(intent.intent_type == 0){
        httpd_resp_send(req, alexa_default_resp_c, HTTPD_RESP_USE_STRLEN);
    }
    else{
        httpd_resp_send(req, intent.resp, HTTPD_RESP_USE_STRLEN);
    }

    if(intent.intent_type == 1){
        //change color
        uint8_t* dat = (uint8_t*)intent.data;
        set_led_color(dat[0], dat[1], dat[2]);
    }
    if(intent.intent_type == 2){
        //set lights on/off
        uint8_t* dat = (uint8_t*)intent.data;
        if(dat[0] == 0){
            turn_off_led();
        }
        else{
            turn_on_led();
        }
    }

    free(intent.data);
    free(intent.resp);
    return ESP_OK;
}

/* Function for starting the webserver */
httpd_handle_t start_webserver(void)
{
    if(http_default_resp){
        free(http_default_resp);
    }
    int http_default_resp_leng = strlen(http_default_resp_c) + 1;
    http_default_resp = (char*)malloc(sizeof(char) * http_default_resp_leng);
    strcpy(http_default_resp, http_default_resp_c);
    http_default_resp[http_default_resp_leng - 1] = 0;

    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;

    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK) {
        /* Register URI handlers */
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_get_favicon);
        httpd_register_uri_handler(server, &uri_post);
    }
    /* If server failed to start, handle will be NULL */
    return server;
}

/* Function for stopping the webserver */
void stop_webserver(httpd_handle_t server)
{
    if (server) {
        /* Stop the httpd server */
        httpd_stop(server);
    }
}