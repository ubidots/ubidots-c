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
 * Instantiate a collection.
 * @arg n  Number of values in this collection.
 * @return A pointer to a collection.
 */
UbidotsCollection* ubidots_collection_init(int n) {
  UbidotsCollection *coll = malloc(sizeof(UbidotsCollection));

  coll->n = n;
  coll->i = 0;
  coll->variable_ids = malloc(sizeof(char*) * n);
  coll->values = malloc(sizeof(float) * n);

  return coll;
}


/**
 * Add a value to a collection.
 * @arg coll         Pointer to the collection made by ubidots_collection_init().
 * @arg variable_id  The ID of the variable this value is associated with.
 * @arg value        The value.
 */
void ubidots_collection_add(UbidotsCollection *coll, char *variable_id, double value) {
  int i = coll->i;

  int len = sizeof(char) * strlen(variable_id);
  coll->variable_ids[i] = malloc(len + 1);
  strcpy(coll->variable_ids[i], variable_id);

  coll->values[i] = value;

  coll->i++;
}


/**
 * Save a collection.
 * @arg coll Collection to save.
 * @reutrn Zero upon success, non-zero upon error.
 */
int ubidots_collection_save(UbidotsClient *client, UbidotsCollection *coll) {
  // Compute URL
  char url[80];

  sprintf(url, "%s/collections/values", client->base_url);

  // Encode JSON Payload
  json_t *j_root = json_array();
  int i, n = coll->n;
  char *json_data;

  for (i = 0; i < n; i++) {
    json_t *j_obj = json_object();
    json_object_set_new(j_obj, "variable", json_string(coll->variable_ids[i]));
    json_object_set_new(j_obj, "value", json_real(coll->values[i]));
    json_array_append_new(j_root, j_obj);
  }

  json_data = json_dumps(j_root, 0);

  // Perform Request
  int rc = ubi_request("POST", url, client->token, json_data, NULL);

  // Cleanup
  json_decref(j_root);
  free(json_data);

  return rc;
}


/**
 * Cleanup a collection when after it is no longer being used.
 * @arg coll Pointer to the collection made by ubidots_collection_init().
 */
void ubidots_collection_cleanup(UbidotsCollection *coll) {
  int i, n = coll->n;

  for (i = 0; i < n; i++) {
    free(coll->variable_ids[i]);
  }

  free(coll->variable_ids);
  free(coll->values);
  free(coll);
}


/**
 * Save a value to Ubidots.
 * @arg client       Pointer to UbidotsClient
 * @arg variable_id  The ID of the variable to save to
 * @arg value        The value to save
 * @arg timestamp    Timestamp (millesconds since epoch). Pass TIMESTAMP_NOW
 *                   to have the timestamp automatically calculated.
 */
int ubidots_save_value(UbidotsClient *client, char *variable_id, double value, long long timestamp) {
  char url[80];
  char json_data[80];

  if (timestamp == TIMESTAMP_NOW)
    timestamp = (long long)time(NULL) * 1000;

  sprintf(url, "%s/variables/%s/values", client->base_url, variable_id);
  sprintf(json_data, "{\"value\": %g, \"timestamp\": %lld}", value, timestamp);

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
  UbidotsClient *client = ubidots_init_with_base_url("5b3d2347ff3b57879e18c4f94c809f728eaeeac2", "http://app.draft.ubidots.com/api");

  if (client == NULL) {
    printf("client = NULL\n");
    return 1;
  }

  printf("base_url: %s\n", client->base_url);
  printf("api_key:  %s\n", client->api_key);
  printf("token:    %s\n", client->token);
  
  /*
  int i;
  for (i=0; i < 10; i++) {
    double value = i * 100;
    printf("Saving value #%d...", i + 1);
    ubidots_save_value(client, "528fb6bdf91b283cf96fe784", value, TIMESTAMP_NOW);
    printf("done\n");
  }
  */

  UbidotsCollection *coll = ubidots_collection_init(1);
  ubidots_collection_add(coll, "5294fea7f91b284ce2ae829a", 1500);
  ubidots_collection_save(client, coll);
  ubidots_collection_cleanup(coll);

  ubidots_cleanup(client);

  return 0;
}
