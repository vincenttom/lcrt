#ifndef __LCRT_CONFIG_H__
#define __LCRT_CONFIG_H__

#include <sqlite3.h>

#define LANGUAGE_DIR "etc/language"
#define DEFAULT_LANGUAGE_DB  "en_US.db"
#define LANGUAGE_DB_NAME  "language.db"
#define LANGUAGE_DB_TABLE "language"

#define LCRT_CONFIG_NAME_LEN 64
#define LCRT_CONFIG_VALUE_LEN 256

struct lcrt_config {
    char          db_name[256];
    char          db_table[64];
    sqlite3      *db;
    sqlite3_stmt *db_res;
    int           db_rv;
    int           db_columns;
    int (*open)(struct lcrt_config *config);
    int (*exec)(struct lcrt_config *config, const char *format, ...);
    int (*select)(struct lcrt_config *config, const char *format, ...);
    int (*get_row)(struct lcrt_config *config);
    const char *(*get_text_col)(struct lcrt_config *config, int column);
    int (*get_int_col)(struct lcrt_config *config, int column);

    int (*close)(struct lcrt_config *config);
    int (*changes)(struct lcrt_config *config);
    int (*finalize)(struct lcrt_config *config);
};
int lcrt_config_load();
int lcrt_config_init(struct lcrt_config *config, const char *db_name, const char *db_table);
const char *lcrt_config_get_local_dir();
const char *lcrt_config_get_language();
int lcrt_config_save_language(char *language_name);
const struct lcrt_support_language *lcrt_get_languages();
#endif
