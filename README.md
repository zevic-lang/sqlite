# Sqlite

SQLite for [Zevic](https://github.com/zevic-lang/zevic). The public API uses only Zevic types and returns `Result` for anything that can fail; the C types stay inside the package.

## Install

Needs the SQLite library and its development files (`apt install libsqlite3-dev`; macOS has them). The tests run on Linux and macOS in CI. Windows is not tested yet.

```
zevpm get zevic-lang/sqlite@v0.1.0
```

or in `zevic.toml`:

```
[deps]
Sqlite = { git = "https://github.com/zevic-lang/sqlite", tag = "v0.1.0" }
```

## Use

```
module Main

import Zevic.Console
import Sqlite

fn Run() -> Result<Unit, Error> {
    mut db: Sqlite.Db = Sqlite.Db.Open("app.db")?
    defer db.Close()

    db.Exec("CREATE TABLE IF NOT EXISTS people (id INTEGER PRIMARY KEY, name TEXT)")?

    mut ins: Sqlite.Stmt = db.Prepare("INSERT INTO people (name) VALUES (?1)")?
    defer ins.Close()
    ins.BindText(1, "Ann")?
    ins.Step()?

    mut q: Sqlite.Stmt = db.Prepare("SELECT id, name FROM people")?
    defer q.Close()
    while (q.Step()?) {
        Console.Println("${q.I64(0)}: ${q.Text(1)}")
    }
    Ok(())
}

fn Main() {
    match (Run()) {
        Ok(_) => {},
        Err(e) => Console.Eprintln(e.ToString()),
    }
}
```

## API

| | |
| --- | --- |
| `Sqlite.Version()` | the linked SQLite version |
| `Db.Open(path)` | open or create a database (`":memory:"` for a private one) |
| `db.Exec(sql)` | run statements that return no rows |
| `db.Prepare(sql)` | compile a statement (`?1`, `?2`, ... are parameters) |
| `db.Changes()`, `db.LastInsertId()` | rows changed, last rowid |
| `db.Close()` | close (twice is harmless; close the `Stmt`s first) |
| `st.BindText(i, v)` `BindI64` `BindF64` `BindNull` | bind a parameter (the first is 1) |
| `st.Step()` | `Ok(true)` row, `Ok(false)` done |
| `st.Reset()` | run again |
| `st.ColumnCount()` `ColumnName(i)` `IsNull(i)` `Text(i)` `I64(i)` `F64(i)` | read the current row (the first column is 0) |
| `st.Close()` | release the statement |

`Db` and `Stmt` hold C pointers, so keep them in a `mut` variable and release them with `defer`.

## Build and test

```
zevpm test
```

## Releases

A push to `main` that passes the tests in CI is released by the workflow when `version` in `zevic.toml` has no tag yet: it creates the tag `vX.Y.Z` and a GitHub release whose notes are the `## X.Y.Z` section of `CHANGELOG.md`. To release, raise `version`, add that section, and push to `main`.

## License

Apache-2.0
