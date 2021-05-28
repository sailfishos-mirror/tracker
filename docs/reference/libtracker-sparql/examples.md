Title: Examples

This chapters shows some real examples of usage of the Tracker
SPARQL Library.

## Querying the Store

First, a [class@Tracker.SparqlConnection] object must be acquired
with [ctor@Tracker.SparqlConnection.new], [ctor@Tracker.SparqlConnection.bus_new]
or [ctor@Tracker.SparqlConnection.remote_new].

Then, a query can be launched either synchronously ([method@Tracker.SparqlConnection.query])
or asynchronously ([method@Tracker.SparqlConnection.query_async]). If launched
asynchronously, the results of the query can be obtained with
[method@Tracker.SparqlConnection.query_finish].

If the query was successful, a [class@Tracker.SparqlCursor]
will be available. You can now iterate the results of the query both
synchronously (with [method@Tracker.SparqlCursor.next]) or asynchronously
(with [method@Tracker.SparqlCursor.next_async] and
[method@Tracker.SparqlCursor.next_finish]).

The [method@Tracker.SparqlConnection.query_statement] function can be used
to obtain a [class@Tracker.SparqlStatement] object holding a prepared SPARQL
query that can then be executed with [method@Tracker.SparqlStatement.execute].
The query string can contain `~name` placeholders which can be replaced with
arbitrary values before query execution with
[method@Tracker.SparqlStatement.bind_string] and similar functions.
This allows parsing the query string only once and to execute it multiple
times with different parameters with potentially significant performance gains.

Once you end up with the query, remember to call [method@Tracker.SparqlCursor.close].
The same applies to [method@Tracker.SparqlConnection.close] when no longer needed.

The following program shows how Read-Only queries can be done to the store in a
synchronous way:

```c
{{examples/readonly-example.c}}
```

## Updating the store

In order to perform updates in the store, a new writable
[class@Tracker.SparqlConnection] object must be acquired with
[ctor@Tracker.SparqlConnection.new].

Once a proper connection object has been acquired, the update can
be launched either synchronously ([method@Tracker.SparqlConnection.update])
or asynchronously ([method@Tracker.SparqlConnection.update_async]).
If launched asynchronously, the result of the operation can be obtained with
[method@Tracker.SparqlConnection.update_finish].

Once you no longer need the connection, remember to call
[method@Tracker.SparqlConnection.close] on the [class@Tracker.SparqlConnection].

The following program shows how a synchronous update can be done to the store:

```c
{{examples/writeonly-example.c}}
```

## Updating the store with blank nodes

The majority of the work here is already described in the
[previous example](#updating-the-store) where we talk about how to write the store.

The difference with this example is that sometimes you want to
insert data and have the URNs returned which were created to
avoid re-querying for them. This is done using
the [method@Tracker.SparqlConnection.update_blank] function (or asynchronously with
[method@Tracker.SparqlConnection.update_blank_async]).
If launched asynchronously, the result of the operation can be obtained with
[method@Tracker.SparqlConnection.update_blank_finish]

The `_:foo` in the example is how a blank node is
represented in SPARQL. The `foo` part is used to generate the
unique ID that is used for the new URN. It is also used in the
`GVariant` that is returned. In the example below, we are creating
a new blank node called `foo` for every class that exists.

The format of the `GVariant` (in D-Bus terms) is `aaa{ss}` (an
array of an array of dictionaries). This is rather complex but
for a good reason. The first array represents each INSERT that
may exist in the SPARQL. The second array represents each new
node for a given WHERE clause (the example below illustrates
this), you need this to differentiate between two INSERT
statments like the one below in the same SPARQL sent to the
store. Last, we have a final array to represent each new node's
name (in this case `foo`) and the actual URN which was
created. For most updates the first two outer arrays will only
have one item in them.

The following program shows how a synchronous blank node update can be done to the store:

```c
{{examples/writeonly-with-blank-nodes-example.c}}
```