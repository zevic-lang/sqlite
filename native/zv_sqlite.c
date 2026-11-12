// A thin layer over SQLite for the Zevic package `Sqlite`.
// Every function takes and returns only plain C types (pointers, int, long long, double, const char*),
// so the Zevic side never needs the layout of an SQLite structure.
#include <sqlite3.h>
#include <stddef.h>

const char *zv_sqlite_libversion(void) { return sqlite3_libversion(); }

// Always returns the handle (even when opening failed, so the message can be read);
// NULL only when memory ran out.
void *zv_sqlite_open(const char *path) {
    sqlite3 *db = NULL;
    sqlite3_open(path, &db);
    return db;
}

int zv_sqlite_close(void *db) { return sqlite3_close((sqlite3 *)db); }
int zv_sqlite_errcode(void *db) { return sqlite3_errcode((sqlite3 *)db); }
const char *zv_sqlite_errmsg(void *db) { return sqlite3_errmsg((sqlite3 *)db); }

int zv_sqlite_exec(void *db, const char *sql) {
    return sqlite3_exec((sqlite3 *)db, sql, NULL, NULL, NULL);
}

// NULL when the statement does not compile (read the message with zv_sqlite_errmsg).
void *zv_sqlite_prepare(void *db, const char *sql) {
    sqlite3_stmt *st = NULL;
    if (sqlite3_prepare_v2((sqlite3 *)db, sql, -1, &st, NULL) != SQLITE_OK) {
        if (st) sqlite3_finalize(st);
        return NULL;
    }
    return st;
}

int zv_sqlite_finalize(void *st) { return sqlite3_finalize((sqlite3_stmt *)st); }
int zv_sqlite_step(void *st) { return sqlite3_step((sqlite3_stmt *)st); }
int zv_sqlite_reset(void *st) { return sqlite3_reset((sqlite3_stmt *)st); }

// SQLITE_TRANSIENT: SQLite copies the text, so the Zevic string only has to live during the call.
int zv_sqlite_bind_text(void *st, int i, const char *v) {
    return sqlite3_bind_text((sqlite3_stmt *)st, i, v, -1, SQLITE_TRANSIENT);
}
int zv_sqlite_bind_i64(void *st, int i, long long v) { return sqlite3_bind_int64((sqlite3_stmt *)st, i, v); }
int zv_sqlite_bind_f64(void *st, int i, double v) { return sqlite3_bind_double((sqlite3_stmt *)st, i, v); }
int zv_sqlite_bind_null(void *st, int i) { return sqlite3_bind_null((sqlite3_stmt *)st, i); }

int zv_sqlite_column_count(void *st) { return sqlite3_column_count((sqlite3_stmt *)st); }
int zv_sqlite_column_type(void *st, int i) { return sqlite3_column_type((sqlite3_stmt *)st, i); }

const char *zv_sqlite_column_name(void *st, int i) {
    const char *s = sqlite3_column_name((sqlite3_stmt *)st, i);
    return s ? s : "";
}

// A NULL column gives "" (never a NULL pointer).
const char *zv_sqlite_column_text(void *st, int i) {
    const unsigned char *s = sqlite3_column_text((sqlite3_stmt *)st, i);
    return s ? (const char *)s : "";
}
long long zv_sqlite_column_i64(void *st, int i) { return sqlite3_column_int64((sqlite3_stmt *)st, i); }
double zv_sqlite_column_f64(void *st, int i) { return sqlite3_column_double((sqlite3_stmt *)st, i); }

int zv_sqlite_changes(void *db) { return sqlite3_changes((sqlite3 *)db); }
long long zv_sqlite_last_insert_rowid(void *db) { return sqlite3_last_insert_rowid((sqlite3 *)db); }
