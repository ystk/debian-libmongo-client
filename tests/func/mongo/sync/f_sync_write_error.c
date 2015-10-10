#include "test.h"
#include <mongo.h>

#include <errno.h>
#include <sys/socket.h>

#include "libmongo-private.h"

#define INVALID_NS "test.$Uncle$.Dagobert$"

void
test_func_mongo_sync_write_error (void)
{
  mongo_sync_connection *conn;
  bson *b;
  const gchar *error_msg;

  b = bson_new ();
  bson_append_int32 (b, "f_sync_write_error", 1);
  bson_finish (b);

  conn = mongo_sync_connect (config.primary_host, config.primary_port,
                             TRUE);

  ok (mongo_sync_cmd_insert (conn, config.ns, b, NULL) == TRUE,
      "Inserting works with correct namespace when safe mode is off");

  ok (mongo_sync_cmd_insert (conn, INVALID_NS, b, NULL) == TRUE,
      "Inserting works with invalid namespace when safe mode is off");

  error_msg = mongo_sync_conn_get_last_error (conn);
  ok (error_msg == NULL,
      "When safe mode is off, there is no error msg, even if ns is invalid.");

  ok (mongo_sync_conn_set_safe_mode (conn, TRUE) == TRUE,
      "Setting safe mode works.");

  ok (mongo_sync_cmd_insert (conn, config.ns, b, NULL) == TRUE,
      "Inserting works with correct namespace when safe mode is on");

  ok (mongo_sync_cmd_insert (conn, INVALID_NS, b, NULL) == FALSE,
      "Inserting fails with invalid namespace when safe mode is on");

  error_msg = mongo_sync_conn_get_last_error (conn);

  ok (error_msg != NULL,
      "Inserting failed in safe mode, so we should have an error msg");

  mongo_sync_disconnect (conn);
}

RUN_NET_TEST (7, func_mongo_sync_write_error);
