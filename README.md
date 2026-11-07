# ElkDB

ElkDB is an experimental, in-memory key-value document store written in C and
C++. It stores JSON documents inside named **buckets** and provides a small
SQL-like command language through an interactive terminal.

The project is a learning prototype that explores how a database shell can be
built from a lexer, parser, in-memory data manager, and JSON serializer. It is
not intended for production use.

## How it works

ElkDB uses a simple two-level data model:

- A **bucket** is a named collection, similar to a table.
- A **document** is a key-value pair inside a bucket. The key is a string
  document ID and the value must be valid JSON.

For example, the command below stores a JSON document under the key `C20` in
the `customers` bucket:

```sql
CREATE BUCKET customers;
INSERT INTO customers VALUES (
  'C20',
  '{"name":"John Doe","city":"Fort Wayne"}'
);
SELECT * FROM customers;
```

`SELECT *` returns the bucket as a JSON object keyed by document ID:

```json
{
    "customers": {
        "C20": {
            "name": "John Doe",
            "city": "Fort Wayne"
        }
    }
}
```

All buckets and documents live only in process memory. ElkDB does not write
data to disk, so all stored data is lost when the program exits.

## Implemented commands

Commands and keywords are case-insensitive. SQL-like statements must end with
a semicolon.

| Command | Purpose |
| --- | --- |
| `CREATE BUCKET <name>;` | Create an empty bucket. |
| `INSERT INTO <bucket> VALUES ('<id>', '<json>');` | Insert a JSON document with a unique ID. |
| `SELECT * FROM <bucket>;` | Return every document in a bucket as JSON. |
| `\show buckets` | List buckets and their document counts. |
| `\set indent <n>` | Set JSON indentation to an even number from 2 through 16. |
| `\exit` | Leave the interactive shell. |

The parser contains an `UPDATE` rule, but it currently prints the parsed
values and does not modify a document. `UPSERT` and `DELETE` are not
implemented.

## Architecture

The prototype is split into a few small components:

- **Flex lexer** (`src/sql.l`) tokenizes commands.
- **Bison parser** (`src/sql.y`) defines the SQL-like grammar and dispatches
  operations.
- **Bucket manager** (`src/datamanager.cpp`) keeps buckets and documents in
  C++ `unordered_map` objects and validates inserted JSON.
- **Output manager** (`src/outputmanager.cpp`) formats responses and documents
  with RapidJSON.
- **Readline shell** (`src/main.c`) provides the prompt, command history, and
  optional command-file execution.
- **RapidJSON** is vendored under `include/`.

## Running command files

The CLI is designed to accept a file containing one complete command per line:

```text
\set indent 4
CREATE BUCKET customers;
INSERT INTO customers VALUES ('C20', '{"name":"John Doe"}');
\show buckets
SELECT * FROM customers;
```

It can then be passed to the executable with:

```sh
./elkdb --file commands.sql
```

After processing the file, ElkDB continues into the interactive shell.

## Build status

The intended toolchain is:

- a C++17 compiler
- GNU Make
- Flex
- GNU Bison
- GNU Readline development headers and library

The intended build command is:

```sh
cd src
make
```

However, the current source snapshot does **not** build as checked in. The
Bison grammar contains syntax that is rejected by current Bison versions, and
`src/main.c` includes a missing `datahandler.cpp` file. These issues need to be
fixed before the CLI can be compiled and run.

## Current limitations

- No disk persistence, durability, transactions, or concurrency control.
- No networking or client/server mode; ElkDB is a local CLI process.
- No document deletion or working update/upsert operation.
- `SELECT` currently returns the full bucket; field projection and aliases are
  parsed but are not applied to the result.
- Document IDs must be unique within a bucket.
- The implementation is an alpha-stage educational prototype and has limited
  error handling and test coverage.

## Project background

The repository includes
[`ElkDBKey-value document store Presentation.pdf`](./ElkDBKey-value%20document%20store%20Presentation.pdf),
which describes the original design, architecture, and goals. Some operations
shown there—such as upsert and delete—represent planned behavior rather than
functionality present in this source snapshot.

## License

ElkDB is available under the terms in [LICENSE](./LICENSE).
