/**
 * Ubidots C Client.
 * @author Daniel da Silva <daniel@ubidots.com>
 */

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "ubidots.h"


/**
 * Returns a char* containing a new token for the given api key and base url.
 * If there was an error, returns NULL.
 */
char* get_token(char* api_key, char* base_url) {
  char token_get_url[100];

  // Build URL to retrieve token
  strncpy(token_get_url, base_url, STRLEN_BASE_URL);
  strcat(token_get_url, "/auth/token");

  return NULL;
}


/**
 * Initialize a Ubidots session. This is most likely the first Ubidots
 * library function you will call. Returns a pointer to a UbidotsClient
 * struct. If there was an error, returns NULL.
 */
UbidotsClient* ubidots_init(char* api_key) {
  return ubidots_init2(api_key, DEFAULT_BASE_URL);
}


UbidotsClient* ubidots_init2(char* api_key, char* base_url) {
  char* token = get_token(api_key, base_url);

  if (token == NULL) {
    return NULL;
  }

  UbidotsClient* client = malloc(sizeof(UbidotsClient));  
  strncpy(client->base_url, base_url, STRLEN_BASE_URL);
  strncpy(client->api_key, api_key, STRLEN_API_KEY);
  strncpy(client->token, token, STRLEN_TOKEN);

  free(token);
  
  return client;
}


/**
 * End a ubidots session. After calling this function with UbidotsClient* client,
 * no more functions may be calls with it.
 */
void ubidots_cleanup(UbidotsClient* client) {
  free(client);
}
