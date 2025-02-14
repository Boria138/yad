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
 * Copyright (C) 2008-2025, Victor Ananjevsky <victor@sanana.kiev.ua>
 */

#include "yad.h"

static void
yad_set_about_license (GtkAboutDialog *dlg)
{
  if (options.about_data.license == NULL)
    {
      gtk_about_dialog_set_license_type (dlg, GTK_LICENSE_UNKNOWN);
      return;
    }

  gtk_about_dialog_set_wrap_license (dlg, TRUE);

  /* Check for predefined licenses */
  if (g_strcmp0 (options.about_data.license, "GPL-2.0") == 0)
    {
      gtk_about_dialog_set_license_type (dlg, GTK_LICENSE_GPL_2_0);
      return;
    }
  if (g_strcmp0 (options.about_data.license, "GPL-3.0") == 0)
    {
      gtk_about_dialog_set_license_type (dlg, GTK_LICENSE_GPL_3_0);
      return;
    }
  if (g_strcmp0 (options.about_data.license, "LGPL-2.1") == 0)
    {
      gtk_about_dialog_set_license_type (dlg, GTK_LICENSE_LGPL_2_1);
      return;
    }
  if (g_strcmp0 (options.about_data.license, "LGPL-3.0") == 0)
    {
      gtk_about_dialog_set_license_type (dlg, GTK_LICENSE_LGPL_3_0);
      return;
    }
  if (g_strcmp0 (options.about_data.license, "BSD") == 0)
    {
      gtk_about_dialog_set_license_type (dlg, GTK_LICENSE_BSD);
      return;
    }
  if (g_strcmp0 (options.about_data.license, "MIT") == 0)
    {
      gtk_about_dialog_set_license_type (dlg, GTK_LICENSE_MIT_X11);
      return;
    }
  if (g_strcmp0 (options.about_data.license, "Artistic") == 0)
    {
      gtk_about_dialog_set_license_type (dlg, GTK_LICENSE_ARTISTIC);
      return;
    }

  /* User specified license */
  gtk_about_dialog_set_license_type (dlg, GTK_LICENSE_CUSTOM);
  if (g_file_test (options.about_data.license, G_FILE_TEST_EXISTS))
    {
      gchar *buf;

      if (g_file_get_contents (options.about_data.license, &buf, NULL, NULL))
        {
          gtk_about_dialog_set_license (dlg, buf);
          g_free (buf);
          return;
        }
    }

  /* Set license as is */
  gtk_about_dialog_set_license (dlg, options.about_data.license);
}

gint
yad_about (void)
{
  GtkWidget *dialog;
  GtkAboutDialog *about;

  const gchar *const authors[] = {
    "Victor Ananjevsky <victor@sanana.kiev.ua>",
    NULL
  };
  const gchar *translators = _("translator-credits");

  gchar *comments = g_strdup_printf (_("Yet Another Dialog\n"
                                       "(show dialog boxes from shell scripts)\n"
                                       "\nBased on Zenity code\n\n"
#ifdef HAVE_HTML
                                       "Built with WebKit\n"
#endif
#ifdef HAVE_SOURCEVIEW
                                       "Built with GtkSourceView\n"
#endif
#ifdef HAVE_SPELL
                                       "Built with GSpell\n"
#endif
                                       "Using GTK+ %s\n"),
                                     gtk_get_version ());

  dialog = gtk_about_dialog_new ();
  about = GTK_ABOUT_DIALOG (dialog);

  /* Set window icon */
  if (options.data.window_icon)
    gtk_window_set_icon_name (GTK_WINDOW (dialog), options.data.window_icon);
  else
    gtk_window_set_icon_name (GTK_WINDOW (dialog), "yad");

  if (options.about_data.name != NULL)
    {
      /* Custom about dialog */
      gtk_about_dialog_set_program_name (about, options.about_data.name);

      if (options.data.dialog_image)
        {
          GdkPixbuf *pixbuf = get_pixbuf (options.data.dialog_image, YAD_BIG_ICON, TRUE);
          gtk_about_dialog_set_logo (about, pixbuf);
          if (pixbuf)
            g_object_unref (pixbuf);
        }

      if (options.about_data.version)
        gtk_about_dialog_set_version (about, options.about_data.version);

      if (options.about_data.copyright)
        gtk_about_dialog_set_copyright (about, options.about_data.copyright);

      if (options.about_data.comments)
        gtk_about_dialog_set_comments (about, options.about_data.comments);

      if (options.about_data.authors)
        {
          gchar **authors_list = g_strsplit (options.about_data.authors, ",", -1);
          gtk_about_dialog_set_authors (about, (const gchar **) authors_list);
          g_strfreev (authors_list);
        }

      if (options.about_data.website)
        gtk_about_dialog_set_website (about, options.about_data.website);

      if (options.about_data.website_lbl)
        gtk_about_dialog_set_website_label (about, options.about_data.website_lbl);

      yad_set_about_license (about);
    }
  else
    {
      /* Default YAD about dialog */
      gtk_about_dialog_set_program_name (about, PACKAGE_NAME);
      gtk_about_dialog_set_version (about, PACKAGE_VERSION);
      gtk_about_dialog_set_copyright (about, "Copyright © 2008-2025, Victor Ananjevsky <victor@sanana.kiev.ua>");
      gtk_about_dialog_set_comments (about, comments);
      gtk_about_dialog_set_authors (about, authors);
      gtk_about_dialog_set_website (about, PACKAGE_URL);
      gtk_about_dialog_set_translator_credits (about, translators);
      gtk_about_dialog_set_wrap_license (about, TRUE);
      gtk_about_dialog_set_license_type (about, GTK_LICENSE_GPL_3_0);
      gtk_about_dialog_set_logo_icon_name (about, "yad");
    }

  g_free (comments);

  /* Connect response signal to close window */
  g_signal_connect_swapped (dialog, "response",
                            G_CALLBACK (gtk_widget_destroy), dialog);

  gtk_widget_show (dialog);

  return 0;
}
