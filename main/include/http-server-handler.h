#ifndef HTTP_SERVER_HANDLER_H
#define HTTP_SERVER_HANDLER_H

#include "esp_http_server.h"

httpd_handle_t start_webserver(void);
void stop_webserver(httpd_handle_t server);
char* get_http_response();

esp_err_t get_handler(httpd_req_t *req);
esp_err_t get_favicon_handler(httpd_req_t *req);
esp_err_t post_handler(httpd_req_t *req);

#endif