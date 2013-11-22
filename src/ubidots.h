/*
 * Copyright (c) 2013 Ubidots.
 */

#ifndef _ubidots_h_
#define _ubidots_h_ 1

#define DEFAULT_BASE_URL "http://things.ubidots.com/api"
#define STRLEN_BASE_URL 100
#define STRLEN_API_KEY  100
#define STRLEN_TOKEN    100

typedef struct UbidotsClient {
  char base_url[STRLEN_BASE_URL];
  char api_key[STRLEN_API_KEY];
  char token[STRLEN_TOKEN];
} UbidotsClient;

typedef struct CurlRespString {
  char *ptr;
  size_t len;
} CurlRespString;

UbidotsClient* ubidots_init(char *api_key);
UbidotsClient* ubidots_init_with_base_url(char *api_key, char *base_url);
int ubidots_savevalue(UbidotsClient *client, char *variable_id, double value);
void ubidots_cleanup(UbidotsClient *client);

#endif
