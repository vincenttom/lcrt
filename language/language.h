#ifndef __LCRT_LANGUAGE_H__
#define __LCRT_LANGUAGE_H__

#include "mkconfig.h"
#define LCRT_SUPPORT_LANGUAGE 3
#define LCRT_LANGUAGE_NUMBER 17

struct __lcrt_language_config {
    char name[LCRT_CONFIG_NAME_LEN];
    char value[LCRT_CONFIG_VALUE_LEN];
};

struct lcrt_language_config {
    char *table_name;
    int members;
    struct __lcrt_language_config config[64];
};
struct lcrt_language {
    char *language_name;
    char *db_name;
    struct lcrt_language_config *language[LCRT_LANGUAGE_NUMBER];
};
struct lcrt_support_language {
    char *language_name;
    char *db_name;
};
#endif
