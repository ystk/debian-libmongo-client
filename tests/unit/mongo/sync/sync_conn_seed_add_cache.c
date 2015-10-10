#include "test.h"
#include "mongo.h"

void
test_mongo_sync_connection_cache_seed_add (void)
{
  mongo_sync_conn_recovery_cache *cache;

  cache = mongo_sync_conn_recovery_cache_new ();

  ok (mongo_sync_conn_recovery_cache_seed_add (cache,
                                               "localhost",
                                               27017) == TRUE,
      "mongo_sync_connection_cache_seed_add() works");

  ok (mongo_sync_conn_recovery_cache_seed_add (cache,
                                               NULL,
                                               27017) == FALSE,
      "mongo_sync_connection_cache_seed_add() should fail with a NULL host");

  mongo_sync_conn_recovery_cache_discard (cache);

  ok (mongo_sync_conn_recovery_cache_seed_add (cache,
                                               "localhost",
                                               27017) == TRUE,
      "mongo_sync_connection_cache_seed_add() works");
      
  mongo_sync_conn_recovery_cache_free (cache);
}

RUN_TEST (3, mongo_sync_connection_cache_seed_add);
