/*
 * Ubidots C client.
 *
 * Copyright (c) 2013 Ubidots.
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include <jansson.h>
#include "ubidots.h"
#include "ubirequest.c"


/**
 * Save a value to Ubidots.
 * @arg client       Pointer to UbidotsClient
 * @arg variable_id  The ID of the variable to save to
 * @arg value        The value to save
 * @arg timestamp    Timestamp (Seconds since epoch)
 */
int ubidots_savevalue(UbidotsClient *client, char *variable_id, double value, int timestamp) {
  char url[80];
  char json_data[80];

  sprintf(url, "%s/variables/%s/values", client->base_url, variable_id);
  sprintf(json_data, "{'value': %g, 'timestamp': %d}", value, timestamp);

  return ubi_request("POST", url, client->token, json_data, NULL);
}


/**
 * Initialize a Ubidots session. This is most likely the first Ubidots
 * library function you will call.
 * @arg api_key  Your API key for the Ubidots API.
 * @return Upon success, a pointer to a UbidotsClient struct. Upon error, NULL.
 */
UbidotsClient* ubidots_init(char *api_key) {
  return ubidots_init_with_base_url(api_key, DEFAULT_BASE_URL);
}


UbidotsClient* ubidots_init_with_base_url(char *api_key, char *base_url) {
  // Perform an API request to generate a new token for the given API key.
  // ----------------------------------------------------------------------
  char url[80];
  char token_hack[80];
  int rc;
  json_t *j_root, *j_token;
  
  sprintf(url, "%s/auth/token", base_url);
  sprintf(token_hack, "/%s", api_key);
  
  rc = ubi_request("POST", url, token_hack, "", &j_root);
  
  if (rc)
    return NULL;
  
  j_token = json_object_get(j_root, "token");

  // Allocate and set fields of struct
  // ---------------------------------
  UbidotsClient *client = malloc(sizeof(UbidotsClient));  

  strncpy(client->base_url, base_url, STRLEN_BASE_URL);
  strncpy(client->api_key, api_key, STRLEN_API_KEY);
  strncpy(client->token, json_string_value(j_token), STRLEN_TOKEN);

  json_decref(j_root);

  return client;
}


/**
 * End a ubidots session. After calling this function with UbidotsClient* client,
 * no more functions may be called with it.
 */
void ubidots_cleanup(UbidotsClient *client) {
  free(client);
}


int main() {
  UbidotsClient *client = ubidots_init("74ccf3b7957fe38e3382c9fd107d70870edbb462");

  if (client == NULL) {
    printf("client = NULL\n");
    return 1;
  }

  printf("base_url: %s\n", client->base_url);
  printf("api_key:  %s\n", client->api_key);
  printf("token:    %s\n", client->token);
  
  int i;
  for (i=0; i < 10; i++) {
    printf("Saving value #%d\n", i + 1);
    ubidots_savevalue(client, "528fb6bdf91b283cf96fe784", 8.0, (int)(time(NULL)));
  }

  ubidots_cleanup(client);

  return 0;
}
