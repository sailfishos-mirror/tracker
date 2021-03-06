[CCode (cprefix = "Tracker", gir_namespace = "Tracker", gir_version = "2.0", lower_case_cprefix = "tracker_")]
namespace Tracker {
        namespace Direct {
                [CCode (cheader_filename = "tracker-direct.h")]
                public class Connection : Tracker.Sparql.Connection, GLib.Initable, GLib.AsyncInitable {
                        public Connection (Tracker.Sparql.ConnectionFlags connection_flags, GLib.File loc, GLib.File? ontology) throws Tracker.Sparql.Error, GLib.IOError, GLib.DBusError;
                        public unowned Tracker.Data.Manager get_data_manager ();
                }
        }
}
