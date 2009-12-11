/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2009, Nokia (urho.konttori@nokia.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

#include "config.h"

#include <string.h>

#include <glib.h>
#include <gio/gio.h>

#include <libtracker-common/tracker-common.h>
#include <libtracker-db/tracker-db.h>

#include <libtracker-data/tracker-data-manager.h>
#include <libtracker-data/tracker-data-query.h>
#include <libtracker-data/tracker-data-update.h>
#include <libtracker-data/tracker-sparql-query.h>

typedef struct _TestInfo TestInfo;

struct _TestInfo {
	const gchar *test_name;
	const gchar *data;
	gboolean expect_error;
};

const TestInfo tests[] = {
	{ "aggregates/aggregate-1", "aggregates/data-1", FALSE },
	{ "aggregates/aggregate-distinct-1", "aggregates/data-1", FALSE },
	{ "aggregates/aggregate-group-1", "aggregates/data-1", FALSE },
	{ "algebra/two-nested-opt", "algebra/two-nested-opt", FALSE },
	{ "algebra/two-nested-opt-alt", "algebra/two-nested-opt", FALSE },
	{ "algebra/opt-filter-3", "algebra/opt-filter-3", FALSE },
	{ "algebra/filter-placement-1", "algebra/data-2", FALSE },
	{ "algebra/filter-placement-2", "algebra/data-2", FALSE },
	{ "algebra/filter-placement-3", "algebra/data-2", FALSE },
	{ "algebra/filter-nested-1", "algebra/data-1", FALSE },
	{ "algebra/filter-nested-2", "algebra/data-1", FALSE },
	{ "algebra/filter-scope-1", "algebra/data-2", FALSE },
	{ "algebra/var-scope-join-1", "algebra/var-scope-join-1", FALSE },
	{ "bnode-coreference/query", "bnode-coreference/data", FALSE },
	{ "bound/bound1", "bound/data", FALSE },
	{ "expr-ops/query-ge-1", "expr-ops/data", FALSE },
	{ "expr-ops/query-le-1", "expr-ops/data", FALSE },
	{ "expr-ops/query-minus-1", "expr-ops/data", FALSE },
	{ "expr-ops/query-mul-1", "expr-ops/data", FALSE },
	{ "expr-ops/query-plus-1", "expr-ops/data", FALSE },
	{ "expr-ops/query-unminus-1", "expr-ops/data", FALSE },
	{ "expr-ops/query-unplus-1", "expr-ops/data", FALSE },
	{ "optional/q-opt-complex-1", "optional/complex-data-1", FALSE },
	{ "regex/regex-query-001", "regex/regex-data-01", FALSE },
	{ "regex/regex-query-002", "regex/regex-data-01", FALSE },
	{ "sort/query-sort-1", "sort/data-sort-1", FALSE },
	{ "sort/query-sort-2", "sort/data-sort-1", FALSE },
	{ "sort/query-sort-3", "sort/data-sort-3", FALSE },
	{ "sort/query-sort-4", "sort/data-sort-4", FALSE },
	{ "sort/query-sort-5", "sort/data-sort-4", FALSE }, 
	{ "subqueries/subqueries-1", "subqueries/data-1", FALSE },
	/* Bracket error after WHERE */
	{ "error/query-error-1", "error/query-error-1", TRUE },
	/* Unknown property */
	{ "error/query-error-2", "error/query-error-2", TRUE },

	{ "turtle/turtle-query-001", "turtle/turtle-data-001", FALSE },
	{ "turtle/turtle-query-002", "turtle/turtle-data-002", FALSE },
	{ NULL }
};

static int
strstr_i (const char *a, const char *b)
{
	return strstr (a, b) != NULL ? 1 : 0;
}

static void
test_sparql_query (gconstpointer test_data)
{
	int (*comparer) (const char *a, const char *b);
	TrackerDBResultSet *result_set;
	const TestInfo *test_info;
	GError *error;
	GString *test_results;
	gchar *data_filename;
	gchar *query, *query_filename;
	gchar *results, *results_filename;
	gchar *prefix, *data_prefix, *test_prefix;

	error = NULL;
	test_info = test_data;

	/* initialization */
	prefix = g_build_path (G_DIR_SEPARATOR_S, TOP_SRCDIR, "tests", "libtracker-data", NULL);
	data_prefix = g_build_filename (prefix, test_info->data, NULL);
	test_prefix = g_build_filename (prefix, test_info->test_name, NULL);
	g_free (prefix);

	tracker_data_manager_init (TRACKER_DB_MANAGER_FORCE_REINDEX,
	                           data_prefix,
	                           NULL, NULL);

	/* data_path = g_build_path (G_DIR_SEPARATOR_S, TOP_SRCDIR, "tests", "libtracker-data", NULL); */

	/* load data set */
	data_filename = g_strconcat (data_prefix, ".ttl", NULL);
	tracker_data_begin_transaction ();
	tracker_turtle_reader_load (data_filename, &error);
	tracker_data_commit_transaction ();
	g_assert (error == NULL);

	query_filename = g_strconcat (test_prefix, ".rq", NULL);
	g_file_get_contents (query_filename, &query, NULL, &error);
	g_assert (error == NULL);

	results_filename = g_strconcat (test_prefix, ".out", NULL);
	g_file_get_contents (results_filename, &results, NULL, &error);
	g_assert (error == NULL);

	g_free (data_prefix);
	g_free (test_prefix);

	/* perform actual query */

	result_set = tracker_data_query_sparql (query, &error);
	if (test_info->expect_error) {
		comparer = strstr_i;
		g_assert (error != NULL);
	} else {
		comparer = strcmp;
		g_assert (error == NULL);
	}

	/* compare results with reference output */

	test_results = g_string_new ("");

	if (result_set) {
		gboolean valid = TRUE;
		guint col_count;
		gint col;

		col_count = tracker_db_result_set_get_n_columns (result_set);

		while (valid) {
			for (col = 0; col < col_count; col++) {
				GValue value = { 0 };

				_tracker_db_result_set_get_value (result_set, col, &value);

				switch (G_VALUE_TYPE (&value)) {
				case G_TYPE_INT:
					g_string_append_printf (test_results, "\"%d\"", g_value_get_int (&value));
					break;
				case G_TYPE_DOUBLE:
					g_string_append_printf (test_results, "\"%f\"", g_value_get_double (&value));
					break;
				case G_TYPE_STRING:
					g_string_append_printf (test_results, "\"%s\"", g_value_get_string (&value));
					break;
				default:
					/* unbound variable */
					break;
				}

				if (col < col_count - 1) {
					g_string_append (test_results, "\t");
				}
			}

			g_string_append (test_results, "\n");

			valid = tracker_db_result_set_iter_next (result_set);
		}

		g_object_unref (result_set);
	} else if (test_info->expect_error) {
		g_string_append (test_results, error->message);
		g_clear_error (&error);
	}

	if (comparer (results, test_results->str)) {
		/* print result difference */
		gchar *quoted_results;
		gchar *command_line;
		gchar *quoted_command_line;
		gchar *shell;
		gchar *diff;

		quoted_results = g_shell_quote (test_results->str);
		command_line = g_strdup_printf ("echo -n %s | diff -u %s -", quoted_results, results_filename);
		quoted_command_line = g_shell_quote (command_line);
		shell = g_strdup_printf ("sh -c %s", quoted_command_line);
		g_spawn_command_line_sync (shell, &diff, NULL, NULL, &error);
		g_assert (error == NULL);

		g_error ("%s", diff);

		g_free (quoted_results);
		g_free (command_line);
		g_free (quoted_command_line);
		g_free (shell);
		g_free (diff);
	}

	/* cleanup */

	g_free (data_filename);
	g_free (query_filename);
	g_free (query);
	g_free (results_filename);
	g_free (results);
	g_string_free (test_results, TRUE);

	tracker_data_manager_shutdown ();
}

int
main (int argc, char **argv)
{
	gint result;
	gint i;
	gchar *current_dir;

	g_type_init ();

	if (!g_thread_supported ()) {
		g_thread_init (NULL);
	}

	g_test_init (&argc, &argv, NULL);

	current_dir = g_get_current_dir ();

	g_setenv ("XDG_DATA_HOME", current_dir, TRUE);
	g_setenv ("XDG_CACHE_HOME", current_dir, TRUE);
	g_setenv ("TRACKER_DB_SQL_DIR", TOP_SRCDIR "/data/db/", TRUE);
	g_setenv ("TRACKER_DB_ONTOLOGIES_DIR", TOP_SRCDIR "/data/ontologies/", TRUE);

	g_free (current_dir);

	/* add test cases */
	for (i = 0; tests[i].test_name; i++) {
		gchar *testpath;

		testpath = g_strconcat ("/libtracker-data/sparql/", tests[i].test_name, NULL);
		g_test_add_data_func (testpath, &tests[i], test_sparql_query);
		g_free (testpath);
	}

	/* run tests */
	result = g_test_run ();

	/* clean up */
	g_print ("Removing temporary data\n");
	g_spawn_command_line_async ("rm -R tracker/", NULL);

	return result;
}
