<p align="center"><img src="logo.png" width="128" alt="logo"></p>

# mod-lonelyice-mysql

MySQL 8 as a plugin: the server keeps its databases on a MySQL server, on the same machine or another one,
instead of the built-in (SQLite) files.

## Use with LonelyIce

Install the plugin from the catalog (or have it installed before the first run). "MySQL server" then appears
next to the hard drive wherever LonelyIce asks where to keep the data: in the setup wizard and in Settings →
Storage (the gear of the plugin opens it). Fill in the server, port, user, password and database prefix;
LonelyIce checks the server right away. On Apply it creates the databases that are missing there
(`<prefix>auth`, `<prefix>characters`, `<prefix>world`, `<prefix>playerbots`; the user needs the right to create
databases), brings existing ones up to date and unpacks the client's tables into the world database. The
built-in databases stay as they are; choosing the hard drive again returns to them.

The plugin declares this in `plugin.json` (`storage`, see LonelyIce's `docs/plugin-format.md`).

## Without LonelyIce

The plugin works in a plain server of [LonelyIceProject/azerothcore-wotlk](https://github.com/LonelyIceProject/azerothcore-wotlk)
too: put the folder into `PluginsDir` (`plugins` next to the programs by default). worldserver, authserver and
dbimport load it (`server.apps`) and then accept MySQL connection strings (`host;port;user;password;database`,
optionally with a `mysql:` prefix) in `LoginDatabaseInfo`, `WorldDatabaseInfo` and `CharacterDatabaseInfo`.

## What it is

The MySQL database backend of AzerothCore, kept out of the core: the core has no MySQL code and no MySQL build
dependency, and this plugin registers the `mysql` backend (`RegisterBackendDriver`) as soon as its library
loads, before the databases open. SQL files are applied over the backend's own connection
(`MySQLScriptTarget`), so no `mysql` program is started and nothing opens a console window. The MySQL client
library is shipped next to the plugin library.

## Build

Add this folder to the core's `AC_PLUGIN_SOURCE_DIRS` and point `LONELYICE_MYSQL_DIR` at a MySQL 8
distribution (`include/`, `lib/`, `bin/`; on Linux and macOS the system's `libmysqlclient` is found without it):

```
cmake -S <core> -B build -DAC_PLUGIN_SOURCE_DIRS=<this folder> -DLONELYICE_MYSQL_DIR=C:/mysql-8.4
cmake --build build --target mod-lonelyice-mysql
```

With the core built as shared libraries (`-DWITH_DYNAMIC_LINKING=ON`) this gives the plugin folder under
`bin/<config>/plugins/lonelyice.mysql/`. Without shared libraries the backend is built into worldserver,
authserver and dbimport, and the client library is copied next to them. Without MySQL 8 the plugin is skipped.

## Support

LonelyIce is free, with no ads and no paid features. If it is useful to you, you can
[buy me a coffee](https://buymeacoffee.com/darthgelum): it pays for the server, code signing and development time.

<a href="https://buymeacoffee.com/darthgelum"><img src=".github/buy-me-a-coffee.png" alt="Buy me a coffee" width="303"></a>

## License

GPL-2.0-or-later, see [LICENSE](LICENSE). MySQL client components are GPL-2.0 (Oracle's FOSS exception applies).

### Blizzard Entertainment

World of Warcraft®, Warcraft®, Wrath of the Lich King® and Blizzard Entertainment® are trademarks or registered
trademarks of Blizzard Entertainment, Inc. in the U.S. and/or other countries.

The game and everything in it belong to Blizzard Entertainment, Inc.: the game client and its program files, data
files and archives, maps and terrain, models, textures, art, animations, interface, music, sounds, voices, texts,
names, lore, characters, creatures, spells, items, quests and every other part of the game. All of it remains
Blizzard's property wherever it appears, including the data a server extracts from your client on your own computer
(game tables, maps, collision and navigation data).

LonelyIce is an unofficial, non-commercial fan project. It is not affiliated with, endorsed, sponsored, approved or
supported by Blizzard Entertainment, Inc. Blizzard's names are used only to say which game client the project works
with.

This repository contains no files from the game client, and LonelyIce neither distributes nor downloads any. It works
only with a copy of the game you already own. Keep the data extracted from your client to yourself: it is Blizzard's
property and is not ours or yours to share.

The license above covers only the code and files of this project and the works it is based on. It grants no rights
to anything that belongs to Blizzard Entertainment, Inc. All other trademarks belong to their respective owners.
