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

Initialize a Ubidots session. This is most likely the first Ubidots library function you will call. Upon success, returns a pointer to a UbidotsClient. Upon error, returns NULL.

ubidots_save_value()
````````````````````
.. code-block:: c

    int ubidots_save_value(UbidotsClient *client, char *variable_id, double value, long long timestamp)

==============  ===========  =================================
Type            Argument     Description
==============  ===========  =================================
UbidotsClient*  client       Pointer to UbidotsClient
char*           variable_id  The ID of the variable to save to
double          value        The value to save with this call
long long       timestamp    Timestamp (millesconds since epoch). Pass constant TIMESTAMP_NOW to have the timestamp automatically calculated.
==============  ===========  =================================

Save a value to Ubidots. Returns zero upon success. Returns non-zero upon error.

ubidots_cleanup()
`````````````````
.. code-block:: c

    void ubidots_cleanup(UbidotsClient *client)

==================  ===========  =================================
Type                Argument     Description
==================  ===========  =================================
UbidotsClient*      client         Pointer to the Ubidots client made by ubidots_init()
==================  ===========  =================================

Free a client after it is no longer needed.

ubidots_collection_init()
`````````````````````````
.. code-block:: c

    UbidotsCollection* ubidots_collection_init(int n)

==============  ===========  =================================
Type            Argument     Description
==============  ===========  =================================
int             n            Number of values that will be stored in this collection.
==============  ===========  =================================

Create a collection. If the number of values added to the collecion does not equal the about specified here when ubidots_save_collection() is called, undefinded behaviour will occur.

Returns a pointer to a collection.

ubidots_collection_add()
````````````````````````
.. code-block:: c

    void ubidots_collection_add(UbidotsCollection *coll, char *variable_id, double value)

==================  ===========  =================================
Type                Argument     Description
==================  ===========  =================================
UbidotsCollection*  coll         Pointer to the collection made by ubidots_collection_init()
char*               variable_id  The ID of the variable this value is associated with
double              value        The value to add to the collection
==================  ===========  =================================

Add a value to a collection. 

ubidots_collection_save()
`````````````````````````
.. code-block:: c

    int ubidots_collection_save(UbidotsClient *client, UbidotsCollection *coll)

==================  ===========  =================================
Type                Argument     Description
==================  ===========  =================================
UbidotsClient*      client       Pointer to the ubidots client made by ubidots_init()
UbidotsCollection*  coll         Pointer to the collection made by ubidots_collection_init()
==================  ===========  =================================
Save a collection. If the number of values added to this collection using ubidots_collection_add() does not equal the number specific when created with ubidots_collection_init(), undefined behaviour will occur.

Returns zero if the save was successful. If an error occurred, returns non-zero.

ubidots_collection_cleanup()
````````````````````````````
.. code-block:: c

    void ubidots_collection_cleanup(UbidotsCollection *coll)

==================  ===========  =================================
Type                Argument     Description
==================  ===========  =================================
UbidotsCollection*  coll         Pointer to the collection made by ubidots_collection_init()
==================  ===========  =================================

Free a collection after it is no longer needed.
