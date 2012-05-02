/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : NiuTao<niutao0602@gmail.com>
 * Created Time: Sun 20 Feb 2011 01:58:51 PM CST
 *
 * Description:  
 */
//#define __LCRT_DEBUG__
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include "language.h"
#include "mkconfig.h"
#include "de_DE.h"
#include "en_US.h"
#include "zh_CN.h"

#include "debug.h"
#define LANGUAGE_DB_PATH "../etc/language"
#define LANG_C "../src/lang.c"

static struct lcrt_language *languages[LCRT_SUPPORT_LANGUAGE] = {
    &en_US,
    &zh_CN,
    &de_DE,
};

int main(int args, char *argv[])
{
    int i, j, k;
    struct lcrt_config db;
    char db_name[256];
    struct lcrt_language_config **language;
    FILE *fp;

    if ((fp = fopen(LANG_C, "w")) == NULL) {
        perror(LANG_C);
        return -1;
    }
    fprintf(fp, "#include <language.h>\n\n");
    fprintf(fp, "static struct lcrt_support_language lcrt_support_languages[LCRT_SUPPORT_LANGUAGE] = {\n");

    for (i = 0; i < LCRT_SUPPORT_LANGUAGE; i++) {
        language = (struct lcrt_language_config **)languages[i]->language;
        printf("make %s\n", languages[i]->db_name);
        fprintf(fp, "    {\"%s\", \"%s\"},\n",languages[i]->language_name, languages[i]->db_name);
        snprintf(db_name, sizeof(db_name), "%s/%s", LANGUAGE_DB_PATH, languages[i]->db_name);
        lcrt_config_init(&db, db_name, NULL);
        for (j = 0; j < LCRT_LANGUAGE_NUMBER; j++) {
            db.exec(&db, 
                    "CREATE TABLE %s( \
                    name VARCHAR(%d) PRIMARY KEY, \
                    value VARCHAR(%d))",
                    language[j]->table_name,
                    LCRT_CONFIG_NAME_LEN,
                    LCRT_CONFIG_VALUE_LEN);
            for (k = 0; k < language[j]->members; k++) {
                debug_print("[%-10s|%-10s]: name = [%-32s] value = [%s]\n",
                            basename((char *)languages[i]->db_name),
                            language[j]->table_name,
                            language[j]->config[k].name,
                            language[j]->config[k].value);
#if 0
                db.exec(&db,
                        "UPDATE %s SET value='%s' WHERE name='%s'",
                         language[j]->table_name,
                         language[j]->config[k].value,
                         language[j]->config[k].name);
                if (db.changes(&db) == 0)
#endif
                {
                    db.exec(&db,
                        "INSERT INTO %s VALUES('%s', '%s')",
                         language[j]->table_name,
                         language[j]->config[k].name,
                         language[j]->config[k].value);
                }
            }
        }
        db.close(&db);
    }

    fprintf(fp, "};\n");
    fprintf(fp, "const struct lcrt_support_language *lcrt_get_languages()\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    return (const struct lcrt_support_language *)lcrt_support_languages;\n");
    fprintf(fp, "}\n");
    fclose(fp);

    return 0;
}

