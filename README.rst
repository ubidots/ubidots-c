Ubidots-C
=========

A Quick Look
------------
The is the pure C client library for Ubidots. It depends on libjansson and libcurl. An quick example of how to use the library follows:

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

Collections, a feature of Ubidots which allows saving of multiple values with a single request, are supported. A quick example of how to use collections follows:

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


API Reference
-------------


ubidots_init()
``````````````
.. code-block:: c

    UbidotsClient* ubidots_init(char *api_key)

=====  ========  =================================
Type   Argument  Description
=====  ========  =================================
char*  api_key   Your API key for the Ubidots API
=====  ========  =================================

Initialize a Ubidots session. This is most likely the first Ubidots library function you will call. If an error occurs, this function returns NULL.
