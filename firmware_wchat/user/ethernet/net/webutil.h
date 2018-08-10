


#ifndef	_HTTPUTIL_H
#define	_HTTPUTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "data_struct.h"
#include "w5500.h"
#include "W5500_conf.h"
#include "utility.h"
#include "web_server.h"
#include "socket.h"


extern eeprom_lgi_t   gEepromLgi;
extern eeprom_mqtt_t  gEepromMqtt;
extern eeprom_lnet_t  gEepromLnet;





void runWebServer(void);
void proc_http(SOCKET s, unsigned char *buf);
void make_cgi_response(uint16_t a, char *b, char *c);
bool loginCheck(st_http_request *http_request);



void http_cgi_response(uint16_t delay, char* http_response, SOCKET s);
void http_cgi_response_to_index(uint16_t delay, char* http_response, SOCKET s);
static void http_get_cgi_handler(char* uri_name, uint8_t* buf);
static void make_basic_config_setting_json_callback(char* buf);
static void make_status_json_callback(char* buf);
static void save_admin_config(st_http_request *http_request);
static void save_net_config(st_http_request *http_request);


#endif


