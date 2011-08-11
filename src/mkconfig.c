//#define __LCRT_DEBUG__
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "debug.h"
#include "mkconfig.h"
#include "sqlite3.h"
#include "foperate.h"
#include "language.h"

static char local_config_dir[256] = {0};
static char config_language[512] = {0};

static int lcrt_config_open(struct lcrt_config *config)
{
    if (config == NULL || strlen(config->db_name) == 0)
        return EINVAL;

    if (config->db != NULL)
        return LCRTE_OK;

    debug_where();
    config->db_rv = sqlite3_open(config->db_name, &config->db);
    if (config->db_rv != SQLITE_OK) {
        debug_print("Can't open database: %s\n", sqlite3_errmsg(config->db));
        sqlite3_close(config->db);
        config->db = NULL;
        return EIO;
    }
    return LCRTE_OK;
}
static int lcrt_config_close(struct lcrt_config *config)
{
    if (config == NULL || config->db == NULL)
        return 0;
    debug_where();
    sqlite3_close(config->db);
    config->db = NULL;
    return 0;
}
static int lcrt_config_changes(struct lcrt_config *config)
{
    if (config == NULL)
        return 0;

     return sqlite3_changes(config->db);
}
static int lcrt_config_exec(struct lcrt_config *config, const char *format, ...)
{
    int rv;
    char sql[4096];
    va_list args;

    if (config == NULL || (rv = config->open(config)) != 0)
        return rv;

    debug_where();
     va_start(args, format);
     vsprintf(sql, format, args);
     va_end(args);

     config->db_rv = sqlite3_exec(config->db, sql, 0, 0, NULL);
     if (config->db_rv != SQLITE_OK)
         return EIO;
     return LCRTE_OK;
}

static int lcrt_config_select(struct lcrt_config *config, const char *format, ...)
{
    int rv;
    char sql[4096];
    va_list args;
    const char *tail;

    debug_where();
    if (config == NULL || (rv = config->open(config)) != 0)
        return rv;

    va_start(args, format);
    vsprintf(sql, format, args);
    va_end(args);

    config->db_rv = sqlite3_prepare(config->db, sql, -1, &config->db_res, &tail);
    if (config->db_rv != SQLITE_OK) {
        return LCRTE_NO_TABLE;
    }
    if (sqlite3_step(config->db_res) == SQLITE_DONE) {
        config->db_columns = 0;
        return LCRTE_NOT_FOUND;
    }
    config->db_columns = sqlite3_column_count(config->db_res);

    return LCRTE_OK;

}
static int lcrt_config_get_row(struct lcrt_config *config)
{
    if (config == NULL)
        return EINVAL;

    config->db_rv = sqlite3_step(config->db_res);

    if (config->db_rv == SQLITE_ROW)
        return LCRTE_OK;
    else
        return LCRTE_NOT_FOUND;
}
static const char *lcrt_config_get_text_column(struct lcrt_config *config, int column)
{
    if (config == NULL || column < 0) {
        return (const char *)NULL;
    }
    return (const char *)sqlite3_column_text(config->db_res, column);
}
static int lcrt_config_get_int_column(struct lcrt_config *config, int column)
{
    if (config == NULL || column < 0) {
        return EINVAL;
    }
    return sqlite3_column_int(config->db_res, column);
}
static int lcrt_config_load_language()
{
    char buf[256];
    int rv;
    struct lcrt_config config;
    char *path[] = {".", "..", LCRT_INSTALL_DIR};
    int i;

    snprintf(buf, sizeof(buf), "%s/"LANGUAGE_DIR, local_config_dir);
    lcrt_mkdir(buf);
    snprintf(buf, sizeof(buf), "%s/%s/"LANGUAGE_DB_NAME, local_config_dir, LANGUAGE_DIR);

    lcrt_config_init(&config, buf, LANGUAGE_DB_TABLE);
    if (config.open(&config) != 0) {
        strncpy(buf, DEFAULT_LANGUAGE_DB, sizeof(buf));
        goto out;
    }
    rv = config.select(&config, "SELECT * FROM %s where name='language'", config.db_table);
    if (rv == LCRTE_NOT_FOUND || rv == LCRTE_NO_TABLE) {
        strncpy(buf, DEFAULT_LANGUAGE_DB, sizeof(buf));
        if (rv == LCRTE_NO_TABLE)
            config.exec(&config, "CREATE TABLE %s(name VARCHAR(64) PRIMARY KEY, value VARCHAR(255))", config.db_table);
        config.exec(&config, "INSERT INTO %s VALUES('language', '%s')", config.db_table, buf);
        goto out1;
    } else {
        strncpy(buf, config.get_text_col(&config, 1), sizeof(buf));
        config.get_row(&config);
    }
out1:
    config.close(&config);
out:
    debug_print("CURRENT LANGUAGE = %s\n", buf);
    for (i = 0; i < 3; i++) {
        sprintf(config_language, "%s/%s/%s", path[i], LANGUAGE_DIR, buf);
        if (access(config_language, F_OK | R_OK) == 0)
            goto ret;
    }
    fprintf(stderr, "LCRT:missing language database\n");
ret:
    debug_print("LANGUAGE DIR: %s\n", config_language);
    return LCRTE_OK;
}
int lcrt_config_save_language(char *language_name)
{
    char buf[256];
    int rv;
    struct lcrt_config config;
    snprintf(buf, sizeof(buf), "%s/"LANGUAGE_DIR, local_config_dir);
    lcrt_mkdir(buf);
    snprintf(buf, sizeof(buf), "%s/%s/"LANGUAGE_DB_NAME, local_config_dir, LANGUAGE_DIR);

    lcrt_config_init(&config, buf, LANGUAGE_DB_TABLE);
    rv = config.select(&config, "SELECT * FROM %s WHERE name='language'", config.db_table);
    if (rv == LCRTE_NOT_FOUND || rv == LCRTE_NO_TABLE) {
        if (rv == LCRTE_NO_TABLE)
            config.exec(&config, "CREATE TABLE %s(name VARCHAR(64) PRIMARY KEY, value VARCHAR(255))", config.db_table);
        rv = config.exec(&config, "INSERT INTO %s VALUES('language', '%s')", config.db_table, language_name);
        debug_where();
    } else {
        strncpy(buf, config.get_text_col(&config, 1), sizeof(buf));
        config.get_row(&config);
        rv = config.exec(&config, "UPDATE %s SET value='%s' WHERE name='language'",
                         config.db_table,language_name);
    }
    config.close(&config);
    return LCRTE_OK;
}
int lcrt_config_load()
{
    char *home;
    if ((home = getenv("HOME")) == NULL) {
        home = ".";
    }
    snprintf(local_config_dir, sizeof(local_config_dir), "%s/.lcrt", home);
    lcrt_mkdir(local_config_dir);
    lcrt_config_load_language();
    return 0;
}
const char *lcrt_config_get_local_dir()
{
    return local_config_dir;
}
const char *lcrt_config_get_language()
{
    return config_language;
}
int lcrt_config_init(struct lcrt_config *config, const char *db_name, const char *db_table)
{
    if (config == NULL || db_name == NULL)
        return EINVAL;

    memset(config, 0, sizeof(struct lcrt_config));

    strncpy(config->db_name, db_name, sizeof(config->db_name));
    if (db_table)
        strncpy(config->db_table, db_table, sizeof(config->db_table));
    config->open = lcrt_config_open;
    config->close = lcrt_config_close;
    config->exec = lcrt_config_exec;
    config->select = lcrt_config_select;
    config->get_row = lcrt_config_get_row;
    config->get_text_col = lcrt_config_get_text_column;
    config->get_int_col = lcrt_config_get_int_column;
    config->changes = lcrt_config_changes;
    return 0;
}
#if 0
const struct lcrt_support_language *lcrt_get_languages()
{
    return (const struct lcrt_support_language *)lcrt_support_languages;
}
#endif
