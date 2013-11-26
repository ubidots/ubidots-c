/**
 * ubirequest.c - Provides the ubi_request() function for making calls to the
 * API easier for client-side code.
 */

typedef struct CurlRespString {
  char *ptr;
  size_t len;
} CurlRespString;


/**
 * Write Callback for Curl, saves response into a CurlRespString.
 * @arg ptr     pointer to beginning of chunks
 * @arg size_t  size of each chunk
 * @arg nmemb   number of chunks
 * @arg s       curl response string
 * @return The number of bytes actually taken care of.
 */
size_t curl_writefunc(void *ptr, size_t size, size_t nmemb, CurlRespString *s) {
  size_t new_len = s->len + size * nmemb;
  s->ptr = realloc(s->ptr, new_len + 1);

  if (s->ptr == NULL) {
    fprintf(stderr, "Call to realloc() failed.\n");
    exit(EXIT_FAILURE);
  }

  memcpy(s->ptr + s->len, ptr, size * nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size * nmemb;
}


/**
 * Initialize a Curl response string.
 * @arg s  The CRS to initialize.
 */
void crs_init(CurlRespString *s) {
  s->len = 0;
  s->ptr = malloc(1);
  
  if (s->ptr == NULL) {
    fprintf(stderr, "Call to malloc() failed.\n");
    exit(EXIT_FAILURE);
  }

  s->ptr[0] = '\0';
}


/**
 * Perform a request.
 * @arg method    GET, POST, PUT, DELETE.
 * @arg url       URL to request.
 * @arg token     Token for X-AUTH-TOKEN header.
 * @arg json_data JSON payload.
 * @arg json_resp Address to write response to. If NULL, then no action will be taken.
 * @return Zero upon success, non-zero upon error.
 */
int ubi_request(char *method, char *url, char *token, char *json_data, json_t **json_resp) {
  CURL *curl;
  CURLcode res;
  char token_header[80];
  char *content_type_header;
  struct curl_slist *slist;
  CurlRespString resp;

  if (token[0] == '/') { // hack
    sprintf(token_header, "X-UBIDOTS-APIKEY: %s", token + 1);
  } else {
    sprintf(token_header, "X-AUTH-TOKEN: %s", token);
  }
  
  content_type_header = "Content-Type: application/json";

  slist = NULL;
  slist = curl_slist_append(slist, token_header);
  slist = curl_slist_append(slist, content_type_header);

  crs_init(&resp);

  // Setup Curl
  curl = curl_easy_init();

  if (! curl)
    return 1;

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curl_writefunc);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp);

  if (! strcmp(method, "POST")) {
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)strlen(json_data));
  } else if (! strcmp(method, "PUT")) {
    // Todo
  } else if (! strcmp(method, "DELETE")) {
    // Todo
  } else if (! strcmp(method, "GET")) {
    // Todo
  }
    
  // Perform Curl request
  res = curl_easy_perform(curl);

  if (res != CURLE_OK) {
    curl_easy_cleanup(curl);
    curl_slist_free_all(slist);
    free(resp.ptr);
    return 2;
  }

  // Write to json_resp if not null
  if (json_resp != NULL) {
    json_t *j_root;
    json_error_t j_error;

    j_root = json_loads(resp.ptr, 0, &j_error);

    if (j_root == NULL) {
      curl_easy_cleanup(curl);
      curl_slist_free_all(slist);
      free(resp.ptr);
      return 3;
    }
    
    *json_resp = j_root;
  }

  /*
  printf("\n");
  printf("payld: %s\n", json_data);
  printf("resp:  %s\n", resp.ptr);
  printf("\n");
  */

  // Cleanup
  curl_easy_cleanup(curl);
  curl_slist_free_all(slist);
  free(resp.ptr);
  
  return 0;
}
