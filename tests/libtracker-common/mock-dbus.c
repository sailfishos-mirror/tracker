#include <glib-object.h>
#include <gobject/gvaluecollector.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <dbus/dbus-glib-bindings.h>
#include "mock-dbus-gproxy.h"
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

static gint connection_counter = 0;

DBusGConnection * 	
dbus_g_bus_get (DBusBusType type, GError **error)
{
        connection_counter += 1;
        return (DBusGConnection *)g_strdup ("hi");
}

DBusGProxy *
dbus_g_proxy_new_for_name (DBusGConnection *conn,
                           const gchar *service,
                           const gchar *path,
                           const gchar *interface)
{
        return (DBusGProxy *)mock_dbus_gproxy_new ();
}

DBusGConnection *
dbus_g_connection_ref (DBusGConnection *conn)
{
        connection_counter += 1;
        return conn;
}

void
dbus_g_connection_unref (DBusGConnection *connection)
{
        connection_counter -= 1;
}

gchar *
dbus_g_method_get_sender (DBusGMethodInvocation *context)
{
        return g_strdup ("hardcoded sender");
}

gboolean
dbus_g_proxy_call (DBusGProxy *proxy,
                   const gchar *function_name,
                   GError  **error,
                   GType first_arg_type, ...)
{
        va_list  args;
        GType    arg_type;
        gchar   *local_error = NULL;

        va_start (args, first_arg_type);
        if (g_strcmp0 (function_name, "GetConnectionUnixProcessID") == 0) {
                /*
                 * G_TYPE_STRING, sender
                 * G_TYPE_INVALID,
                 * G_TYPE_UINT, pid
                 * G_TYPE_INVALID,
                 */
                GValue value = { 0, };

                /* Input string (ignore) */
                g_value_init (&value, G_TYPE_STRING);
                G_VALUE_COLLECT (&value, args, 0, &local_error);
                g_value_unset (&value);

                arg_type = va_arg (args, GType);
                g_assert (arg_type == G_TYPE_INVALID);

                arg_type = va_arg (args, GType);
                g_assert (arg_type == G_TYPE_UINT);

                g_value_init (&value, arg_type);
                g_value_set_uint (&value, (guint)getpid ());
                G_VALUE_LCOPY (&value, 
                               args, 0,
                               &local_error);
                g_value_unset (&value);
                                
                
        } else {
                g_critical ("dbus_g_proxy_call '%s' unsupported", function_name);
        }
        va_end (args);
        return TRUE;
}
