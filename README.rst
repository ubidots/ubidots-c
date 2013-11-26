Ubidots-C
=========

The is the pure C client library for Ubidots depends on libjansson and libcurl. An example of how to use the library follows.

.. code-block:: c

    #include <time.h>
    #include "ubidots.h"

    int main() {

      UbidotsClient *client = ubidots_init(API_KEY);

      while ( keepGoing() ) {
        double value = getValue();
        ubidots_save_value(client, value, TIMESTAMP_NOW);
      }
   
      ubidots_cleanup(client);

      return 0;
    }
