/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2009, Nokia (urho.konttori@nokia.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

#ifndef __LIBTRACKERMINER_MONITOR_H__
#define __LIBTRACKERMINER_MONITOR_H__

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define TRACKER_TYPE_MONITOR            (tracker_monitor_get_type ())
#define TRACKER_MONITOR(object)                 (G_TYPE_CHECK_INSTANCE_CAST ((object), TRACKER_TYPE_MONITOR, TrackerMonitor))
#define TRACKER_MONITOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TRACKER_TYPE_MONITOR, TrackerMonitorClass))
#define TRACKER_IS_MONITOR(object)      (G_TYPE_CHECK_INSTANCE_TYPE ((object), TRACKER_TYPE_MONITOR))
#define TRACKER_IS_MONITOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TRACKER_TYPE_MONITOR))
#define TRACKER_MONITOR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TRACKER_TYPE_MONITOR, TrackerMonitorClass))

typedef struct TrackerMonitor         TrackerMonitor;
typedef struct TrackerMonitorClass    TrackerMonitorClass;
typedef struct TrackerMonitorPrivate  TrackerMonitorPrivate;

struct TrackerMonitor {
	GObject         parent;
	TrackerMonitorPrivate *private;
};

struct TrackerMonitorClass {
	GObjectClass parent;
};

GType           tracker_monitor_get_type             (void);
TrackerMonitor *tracker_monitor_new                  (void);

gboolean        tracker_monitor_get_enabled          (TrackerMonitor *monitor);
gint            tracker_monitor_get_scan_timeout     (TrackerMonitor *monitor);
gint            tracker_monitor_get_cache_timeout    (TrackerMonitor *monitor);

void            tracker_monitor_set_enabled          (TrackerMonitor *monitor,
                                                      gboolean        enabled);
void            tracker_monitor_set_scan_timeout     (TrackerMonitor *monitor,
                                                      gint            value);
void            tracker_monitor_set_cache_timeout    (TrackerMonitor *monitor,
                                                      gint            value);
gboolean        tracker_monitor_add                  (TrackerMonitor *monitor,
                                                      GFile          *file);
gboolean        tracker_monitor_remove               (TrackerMonitor *monitor,
                                                      GFile          *file);
gboolean        tracker_monitor_remove_recursively   (TrackerMonitor *monitor,
                                                      GFile          *file);
gboolean        tracker_monitor_is_watched           (TrackerMonitor *monitor,
                                                      GFile          *file);
gboolean        tracker_monitor_is_watched_by_string (TrackerMonitor *monitor,
                                                      const gchar    *path);
guint           tracker_monitor_get_count            (TrackerMonitor *monitor);
guint           tracker_monitor_get_ignored          (TrackerMonitor *monitor);

G_END_DECLS

#endif /* __LIBTRACKERMINER_MONITOR_H__ */
