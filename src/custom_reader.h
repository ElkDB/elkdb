
void set_sql_query(const char* query) {
    yy_scan_string(query);
}

void clear_buffer() {
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

int parse_query(const char* query) {
  set_sql_query(query);
  int status = yyparse();
  clear_buffer();
  return status;
}
