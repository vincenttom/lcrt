#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <libgen.h>
#include <sys/types.h>
#include "iwindow.h"
#include "istatus.h"
#include "cstatus.h"
#include "mkconfig.h"
#include "debug.h"

int lcrt_status_init_config(struct lcrt_status *lstatus);
int lcrt_status_load_config(struct lcrt_status *lstatus);
int lcrt_status_create_config(struct lcrt_status *lstatus);

struct lcrt_status *lcrt_create_status(struct lcrt_window *parent)
{
    struct lcrt_status *lstatus;
    GdkPixbuf *pixpic;
    GtkStatusIcon *status_icon;  

    lstatus = (struct lcrt_status *)calloc(1, sizeof(struct lcrt_status));
    
    assert(lstatus);
    lstatus->parent = parent;

    lcrt_status_init_config(lstatus);
    lcrt_status_load_config(lstatus);

    pixpic = gtk_window_get_icon(GTK_WINDOW(parent->window));
    
    status_icon = gtk_status_icon_new_from_pixbuf(pixpic);

    lstatus->status = status_icon;
    gtk_status_icon_set_tooltip(status_icon, lstatus->config.value[LCRT_S_TOOLTIP]);

    g_signal_connect((gpointer)status_icon, "activate", 
                     G_CALLBACK(lcrt_status_on_activate), 
                     lstatus);

    g_signal_connect((gpointer)status_icon, "button-press-event", 
                     G_CALLBACK(lcrt_status_on_button_press_event), 
                     lstatus);
    return lstatus;
}
void lcrt_destroy_status(struct lcrt_status *lstatus)
{
    debug_where();
    if (lstatus == NULL)
        free(lstatus);
}
GtkWidget* lcrt_status_create_popup_menu(struct lcrt_status *lstatus)
{
  GtkWidget *menu_status_icon;
  GtkWidget *status_show_window;
  GtkWidget *image2;
  GtkWidget *status_quit;
  GtkWidget *image3;

  menu_status_icon = gtk_menu_new ();

  status_show_window = gtk_image_menu_item_new_with_mnemonic (lstatus->config.value[LCRT_S_SHOW_WINDOW]);
  gtk_widget_show (status_show_window);
  gtk_container_add (GTK_CONTAINER (menu_status_icon), status_show_window);

  image2 = gtk_image_new_from_stock ("gtk-home", GTK_ICON_SIZE_MENU);
  gtk_widget_show (image2);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (status_show_window), image2);

  status_quit = gtk_image_menu_item_new_with_mnemonic (lstatus->config.value[LCRT_S_QUIT]);
  gtk_widget_show (status_quit);
  gtk_container_add (GTK_CONTAINER (menu_status_icon), status_quit);

  image3 = gtk_image_new_from_stock ("gtk-quit", GTK_ICON_SIZE_MENU);
  gtk_widget_show (image3);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (status_quit), image3);

  g_signal_connect ((gpointer) status_show_window, "activate",
                    G_CALLBACK (lcrt_status_on_show_window_active),
                    lstatus);
  g_signal_connect ((gpointer) status_quit, "activate",
                    G_CALLBACK (lcrt_status_on_quit_activate),
                    lstatus);

  return menu_status_icon;
}
static const char *lcrt_status_get_db_name(struct lcrt_status *lstatus)
{
    return lstatus == NULL ? NULL : lstatus->config.db.db_name;
}
static const char *lcrt_status_get_tb_name(struct lcrt_status *lstatus)
{
    return lstatus == NULL ? NULL : lstatus->config.db.db_table;
}

int lcrt_status_init_config(struct lcrt_status *lstatus)
{
    char db_name[256];
    int i;
    static char *name[LCRT_S_NUMBER] = {LCRT_S_NAME};
    static char *value[LCRT_S_NUMBER] = {LCRT_S_VALUE};

    assert(lstatus);

    memset(&lstatus->config, 0, sizeof(struct lcrtc_status));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&lstatus->config.db, db_name, LCRT_ISTATUS_TABLE);
    for (i = 0; i < LCRT_S_NUMBER; i++) {
        lstatus->config.name[i] = name[i];
        strncpy(lstatus->config.value[i], value[i], sizeof(lstatus->config.value[i]));
    }
    lstatus->get_db = lcrt_status_get_db_name;
    lstatus->get_tb = lcrt_status_get_tb_name;

    return 0;
}
int lcrt_status_load_config(struct lcrt_status *lstatus)
{
    int rv, i;

    assert(lstatus);

    rv = lstatus->config.db.select(&lstatus->config.db, "SELECT * FROM %s", lstatus->get_tb(lstatus));
    debug_where();
    if (rv == LCRTE_NO_TABLE) {
        //lcrt_status_create_config(lstatus);
        return LCRTE_NO_CONFIG;
    }
    debug_where();
    for (i = 0; i < LCRT_S_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(lstatus->config.value[i], lstatus->config.db.get_text_col(&lstatus->config.db, 1), 
                sizeof(lstatus->config.value[i]));
        rv = lstatus->config.db.get_row(&lstatus->config.db);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s]\n",
                basename((char *)lstatus->get_db(lstatus)),
                lstatus->get_tb(lstatus),
                lstatus->config.name[i],
                lstatus->config.value[i]);
    }

    return rv;
}
int lcrt_status_create_config(struct lcrt_status *lstatus)
{
    int i;

    assert(lstatus);
    lstatus->config.db.exec(&lstatus->config.db, 
                         "CREATE TABLE %s( \
                          name VARCHAR(64) PRIMARY KEY, \
                          value VARCHAR(255)",
                         lstatus->get_tb(lstatus));
    for (i = 0; i < LCRT_S_NUMBER; i++) {
        lstatus->config.db.exec(&lstatus->config.db,
                             "INSERT INTO %s VALUES('%s', '%s')",
                             lstatus->get_tb(lstatus),
                             lstatus->config.name[i], 
                             lstatus->config.value[i]);
    }
    lstatus->config.db.close(&lstatus->config.db);
    return 0;
}
