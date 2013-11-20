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
UbidotsClient* ubidots_init2(char *api_key, char *base_url);
void ubidots_cleanup(UbidotsClient *client);
