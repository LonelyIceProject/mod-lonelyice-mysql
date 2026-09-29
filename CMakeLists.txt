# Plugin build (the core's AC_PLUGIN_SOURCE_DIRS): the core's MySQL backend as a plugin, for a core built without
# MySQL (-DWITH_MYSQL=OFF). The MySQL client library and the mysql program (which the core applies sql files
# with) are laid out next to the plugin library.
set(LONELYICE_MYSQL_DIR "" CACHE PATH "A MySQL 8 distribution (include/, lib/, bin/) to build and ship the plugin with")

find_path(MYSQL_PLUGIN_INCLUDE_DIR mysql.h HINTS "${LONELYICE_MYSQL_DIR}/include" PATH_SUFFIXES mysql)
find_library(MYSQL_PLUGIN_LIBRARY NAMES libmysql mysqlclient HINTS "${LONELYICE_MYSQL_DIR}/lib")
find_program(MYSQL_PLUGIN_CLIENT NAMES mysql HINTS "${LONELYICE_MYSQL_DIR}/bin" NO_DEFAULT_PATH)
if (NOT MYSQL_PLUGIN_INCLUDE_DIR OR NOT MYSQL_PLUGIN_LIBRARY OR NOT MYSQL_PLUGIN_CLIENT)
  message(STATUS "mod-lonelyice-mysql: MySQL 8 not found (set LONELYICE_MYSQL_DIR), the plugin is not built")
  return()
endif()

set(MYSQL_BACKEND_DIR "${AC_SOURCE_DIR}/src/server/database/Database/Backends/MySQL")
AddPlugin(mod-lonelyice-mysql
  SOURCES
    plugin/plugin.cpp
    "${MYSQL_BACKEND_DIR}/MySQLBackend.cpp"
    "${MYSQL_BACKEND_DIR}/MySQLStatement.cpp"
  LINK
    database
    "${MYSQL_PLUGIN_LIBRARY}")
target_include_directories(mod-lonelyice-mysql PRIVATE "${MYSQL_BACKEND_DIR}" "${MYSQL_PLUGIN_INCLUDE_DIR}")

# Runtime files: the client library, the mysql program and what they load.
get_filename_component(MYSQL_PLUGIN_BIN "${MYSQL_PLUGIN_CLIENT}" DIRECTORY)
if (WIN32)
  get_filename_component(MYSQL_PLUGIN_LIB_DIR "${MYSQL_PLUGIN_LIBRARY}" DIRECTORY)
  set(MYSQL_PLUGIN_RUNTIME
    "${MYSQL_PLUGIN_LIB_DIR}/libmysql.dll"
    "${MYSQL_PLUGIN_CLIENT}"
    "${MYSQL_PLUGIN_BIN}/libssl-3-x64.dll"
    "${MYSQL_PLUGIN_BIN}/libcrypto-3-x64.dll")
else()
  set(MYSQL_PLUGIN_RUNTIME "${MYSQL_PLUGIN_CLIENT}")
endif()
add_custom_command(TARGET mod-lonelyice-mysql POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MYSQL_PLUGIN_RUNTIME} "$<TARGET_FILE_DIR:mod-lonelyice-mysql>"
  VERBATIM)
