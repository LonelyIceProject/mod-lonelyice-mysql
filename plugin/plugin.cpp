/*
 * This file is part of mod-lonelyice-mysql. Copyright (C) LonelyIceProject.
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 */
#include "IDbConnectionBackend.h"
#include "MySQLBackend.h"
#include "PluginApi.h"

namespace
{
    // Registered as soon as the library loads, in every program that opens the databases (server.apps).
    void AddMySQLBackend()
    {
        DbBackendDriver driver;
        driver.create = &CreateMySQLBackend;
        driver.caps = { 0, true, true };
        driver.init = &MySQLLibrary::Init;
        driver.end = &MySQLLibrary::End;
        driver.version = &MySQLLibrary::Version;
        RegisterBackendDriver(DatabaseBackend::MySQL, driver);
    }
}

AC_PLUGIN_ON_LOAD(AddMySQLBackend)
