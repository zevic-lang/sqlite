# Changelog

## 0.6.0

- No C code any more: `import C "sqlite3.h"` reads the header, and the 64 lines of `native/zv_sqlite.c` are gone. Needs Zevic 0.6 (`import C`). The public API is the same.

## 0.5.0

- Needs Zevic 0.5 or later: the fields are written with `let` and `mut` (no change in what the package does).

## 0.1.0

First version: `Db` (`Open`, `Close`, `Exec`, `Prepare`, `Changes`, `LastInsertId`), `Stmt` (`Bind*`, `Step`, `Reset`, reading columns) and `Version`.
