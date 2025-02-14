/*
 * This file is part of YAD.
 *
 * YAD is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * YAD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with YAD. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2008-2024, Victor Ananjevsky <victor@sanana.kiev.ua>
 */

#include "yad.h"

static GtkWidget *app;

static void
app_activated_cb (GtkAppChooserWidget *w, GAppInfo *app_info, gpointer user_data)
{
  if (options.plug == -1)
    yad_exit (options.data.def_resp);
}

GtkWidget *
app_create_widget (GtkWidget *dlg)
{
  g_autofree gchar *ctype = NULL;
  GtkWidget *w;

  if (options.extra_data && *options.extra_data)
    ctype = g_strdup (options.extra_data[0]);
  else
    ctype = g_strdup ("text/plain");

  app = w = gtk_app_chooser_widget_new (ctype);
  gtk_widget_set_name (w, "yad-app-widget");

  gtk_app_chooser_widget_set_show_default (GTK_APP_CHOOSER_WIDGET (w), TRUE);
  gtk_app_chooser_widget_set_show_recommended (GTK_APP_CHOOSER_WIDGET (w), TRUE);
  gtk_app_chooser_widget_set_show_fallback (GTK_APP_CHOOSER_WIDGET (w), options.app_data.show_fallback);
  gtk_app_chooser_widget_set_show_other (GTK_APP_CHOOSER_WIDGET (w), options.app_data.show_other);
  gtk_app_chooser_widget_set_show_all (GTK_APP_CHOOSER_WIDGET (w), options.app_data.show_all);

  g_signal_connect (w, "application-activated", G_CALLBACK (app_activated_cb), NULL);

  return w;
}

void
app_print_result (void)
{
  GAppInfo *info = gtk_app_chooser_get_app_info (GTK_APP_CHOOSER (app));

  if (info)
    {
      if (options.app_data.extended)
        {
          g_autofree gchar *name = NULL;
          g_autofree gchar *display_name = NULL;
          g_autofree gchar *description = NULL;
          g_autofree gchar *icon_str = NULL;
          g_autofree gchar *executable = NULL;

          name = g_app_info_get_name (info);
          display_name = g_app_info_get_display_name (info);
          description = g_app_info_get_description (info);
          icon_str = g_icon_to_string (g_app_info_get_icon (info));
          executable = g_app_info_get_executable (info);

          if (options.common_data.quoted_output)
            {
              g_autofree gchar *quoted_name = g_shell_quote (name);
              g_autofree gchar *quoted_display_name = g_shell_quote (display_name);
              g_autofree gchar *quoted_description = g_shell_quote (description);
              g_autofree gchar *quoted_icon_str = g_shell_quote (icon_str);
              g_autofree gchar *quoted_executable = g_shell_quote (executable);

              g_printf ("%s%s%s%s%s%s%s%s%s%s\n",
                         quoted_name, options.common_data.separator,
                         quoted_display_name, options.common_data.separator,
                         quoted_description, options.common_data.separator,
                         quoted_icon_str, options.common_data.separator,
                         quoted_executable, options.common_data.separator);
            }
          else
            {
              g_printf ("%s%s%s%s%s%s%s%s%s%s\n",
                         name, options.common_data.separator,
                         display_name, options.common_data.separator,
                         description, options.common_data.separator,
                         icon_str, options.common_data.separator,
                         executable, options.common_data.separator);
            }
        }
      else
        {
          g_autofree gchar *executable = g_app_info_get_executable (info);

          if (options.common_data.quoted_output)
            {
              g_autofree gchar *quoted_executable = g_shell_quote (executable);
              g_printf ("%s\n", quoted_executable);
            }
          else
            {
              g_printf ("%s\n", executable);
            }
        }
    }
}
