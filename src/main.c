
#include "sql.tab.c"
#include "lex.yy.c"
#include "custom_reader.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

static char* query = (char*) NULL;

char* get_query()
{
    // Free the memory if the query is already allocated.
    if (query)
    {
        free(query);
        query = (char *)NULL;
    }

    // Get the query.
    query = readline ("> ");

    // Add the query to the history.
    if (query && *query) {
        add_history (query);
    }

  return query;
}
int main(void) {
    char* q;
    q = get_query();

    while(strlen(q) >= 5 && strncmp(q, "\\exit", 5) != 0) {
        parse_query(q);

        q = get_query();
    }
}