# Plugin build (the core's AC_PLUGIN_SOURCE_DIRS): the MySQL database backend. It builds against the core's shared
# library only, so worldserver, authserver and dbimport can all load it (server.apps). The MySQL client library and
# what it loads are laid out next to the plugin library (next to the programs when the core is built without shared
# libraries); sql files are applied over the connection, so the mysql program is not needed.
set(LONELYICE_MYSQL_DIR "" CACHE PATH "A MySQL 8 distribution (include/, lib/, bin/) to build and ship the plugin with")

find_path(MYSQL_PLUGIN_INCLUDE_DIR mysql.h HINTS "${LONELYICE_MYSQL_DIR}/include" PATH_SUFFIXES mysql)
find_library(MYSQL_PLUGIN_LIBRARY NAMES libmysql mysqlclient HINTS "${LONELYICE_MYSQL_DIR}/lib")
if (NOT MYSQL_PLUGIN_INCLUDE_DIR OR NOT MYSQL_PLUGIN_LIBRARY)
  message(STATUS "mod-lonelyice-mysql: MySQL 8 not found (set LONELYICE_MYSQL_DIR), the plugin is not built")
  return()
endif()

# Runtime files on Windows: the client library and the OpenSSL it loads (elsewhere the system's libmysqlclient).
set(MYSQL_PLUGIN_RUNTIME "")
if (WIN32)
  get_filename_component(MYSQL_PLUGIN_LIB_DIR "${MYSQL_PLUGIN_LIBRARY}" DIRECTORY)
  get_filename_component(MYSQL_PLUGIN_BIN "${MYSQL_PLUGIN_LIB_DIR}/../bin" ABSOLUTE)
  set(MYSQL_PLUGIN_RUNTIME
    "${MYSQL_PLUGIN_LIB_DIR}/libmysql.dll"
    "${MYSQL_PLUGIN_BIN}/libssl-3-x64.dll"
    "${MYSQL_PLUGIN_BIN}/libcrypto-3-x64.dll")
endif()

AddPlugin(mod-lonelyice-mysql
  SOURCES
    plugin/plugin.cpp
    src/MySQLBackend.cpp
    src/MySQLScriptTarget.cpp
    src/MySQLStatement.cpp
  CORE
    shared
  LINK
    "${MYSQL_PLUGIN_LIBRARY}"
  RUNTIME_FILES
    ${MYSQL_PLUGIN_RUNTIME})
target_include_directories(mod-lonelyice-mysql PRIVATE "${MYSQL_PLUGIN_INCLUDE_DIR}")
