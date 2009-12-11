/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2008, Nokia
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 * Authors: Philip Van Hoof <philip@codeminded.be>
 */

#ifndef _TRACKER_TOPANALYZER_H_
#define _TRACKER_TOPANALYZER_H_

#include <glib.h>

G_BEGIN_DECLS

void tracker_topanalyzer_init     (void);
void tracker_topanalyzer_extract  (const gchar *uri,
                                   TrackerSparqlBuilder *metadata,
                                   gchar **content_type);
void tracker_topanalyzer_shutdown (void);

G_END_DECLS

#endif
