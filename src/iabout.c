/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : NiuTao<niutao0602@gmail.com>
 * Created Time: Wed 02 Mar 2011 08:06:54 PM CST
 * File Name   : /home/niutao/lcrt/lcrt/iabout.c
 *
 * Description:  
 */
#include <gtk/gtk.h>
#include "support.h"
#include "ihelp.h"
#include "imenubar.h"
#include "iwindow.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#error "missing config.h"
#endif

int lcrt_abount_dialog(struct lcrt_help *parent)
{
    const char *copyright =
        "Copyright © 2010 NiuTao";
    const char *licence =
       "lcrt is free software; you can redistribute it and/or \n"
       "modify it under the terms of the GNU General Public \n"
       "License as published by the Free Software Foundation; \n"
       "either version 2 of the License, or(at your option) any\n"
       "later version.\n\n"
       "lcrt is distributed in the hope that it will be useful, \n"
       "but WITHOUT ANY WARRANTY; without even the implied \n"
       "warranty of MERCHANTABILITY or FITNESS FOR A \n"
       "PARTICULAR PURPOSE. See the GNU General Public \n"
       "License for more details.\n\n"
       "You should have received a copy of the GNU General \n"
       "Public License along with lcrt; if not, write to the \n"
       "Free Software Foundation, Inc., 51 Franklin St, Fifth \n"
       "Floor, Boston, MA 02110-1301 USA.\n";

    const char *authors[] = {
        "NiuTao <niutao0602@mail.com>",
        "Asias He <asias.hejun@gmail.com>",
        NULL
    };

    const char *documenters[] = {
        "NiuTao <niutao0602@mail.com>",
        NULL
    };
    const char *website = 
        "http://code.google.com/p/lcrt";
    gtk_show_about_dialog(GTK_WINDOW(parent->parent->parent->window),
                          "copyright", copyright,
                          "comments", "Linux Remote Login Tool",
                          "version", PACKAGE_VERSION,
                          "authors", authors,
                          "documenters", documenters,
                          "license", licence,
                          "wrap-license", TRUE,
                          "translator-credits", "translator-credits",
                          "website", website,
                          "logo", create_pixbuf("lcrt-logo.xpm", 48, 48), 
                          NULL);
    return 0;
}
