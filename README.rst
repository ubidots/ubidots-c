Ubidots-C
=========

The is the pure C client library for Ubidots, which depends on libjansson and libcurl. An example of how to use the library follows.

.. code-block:: c

    #include <ubidots.h>

    int main() {
      UbidotsClient *client = ubidots_init(API_KEY);

      while ( keepGoing() ) {
        double value = getValue();
        ubidots_save_value(client, VARIABLE_ID, value, TIMESTAMP_NOW);
      }
   
      ubidots_cleanup(client);

      return 0;
    }

An example of how using collections (saving multiple values in a single request) follows.

.. code-block:: c

    #include <ubidots.h>

    int main() {
      UbidotsClient *client = ubidots_init(API_KEY);

      while ( keepGoing() ) {
        double firstValue = getFirstValue();
        double secondValue = getSecondValue();
        double thirdValue = getThirdValue();

        UbidotsCollection *coll = ubidots_collection(3);
        ubidots_collection_add(coll, FIRST_VARIABLE_ID, firstValue);
        ubidots_collection_add(coll, SECOND_VARIABLE_ID, secondValue);
        ubidots_collection_add(coll, THIRD_VARIABLE_ID, thirdValue);
        ubidots_collection_save(client, coll);
        ubidots_collection_cleanup(coll);
      }

      ubidots_cleanup(client);

      return 0;
    }
