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
    // The core's MySQL backend, built into this library instead of the core. Registered while the scripts load,
    // which is before the databases are opened.
    void AddMySQLBackend()
    {
        DbBackendDriver driver;
        driver.create = &CreateMySQLBackend;
        // sql files go through the mysql program shipped next to this library (MySQLExecutable)
        driver.caps = { 0, true, true, true };
        driver.init = &MySQLLibrary::Init;
        driver.end = &MySQLLibrary::End;
        driver.version = &MySQLLibrary::Version;
        RegisterBackendDriver(DatabaseBackend::MySQL, driver);
    }
}

AC_PLUGIN(AddMySQLBackend)
