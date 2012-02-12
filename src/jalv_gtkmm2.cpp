/*
  Copyright 2007-2012 David Robillard <http://drobilla.net>

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <gtkmm.h>

#include "jalv_internal.h"

Gtk::Main* jalv_gtk_main = NULL;

int
jalv_init(int* argc, char*** argv, JalvOptions* opts)
{
	jalv_gtk_main = new Gtk::Main(*argc, *argv);
	return 0;
}

LilvNode*
jalv_native_ui_type(Jalv* jalv)
{
	return lilv_new_uri(jalv->world,
	                    "http://lv2plug.in/ns/extensions/ui#GtkUI");
}

int
jalv_ui_resize(Jalv* jalv, int width, int height)
{
	if (jalv->ui_instance) {
		GtkWidget* widget = GTK_WIDGET(
			suil_instance_get_widget(jalv->ui_instance));
		if (widget) {
			gtk_widget_set_size_request(GTK_WIDGET(widget), width, height);
		}
	}
	return 0;
}

int
jalv_open_ui(Jalv*         jalv,
             SuilInstance* instance)
{
	Gtk::Window* window = new Gtk::Window();

	if (instance) {
		GtkWidget*   widget   = (GtkWidget*)suil_instance_get_widget(instance);
		Gtk::Widget* widgetmm = Glib::wrap(widget);
		window->add(*Gtk::manage(widgetmm));
		widgetmm->show_all();

		g_timeout_add(1000 / JALV_UI_UPDATE_HZ,
		              (GSourceFunc)jalv_emit_ui_events, jalv);
	} else {
		Gtk::Button* button = Gtk::manage(new Gtk::Button("Close"));
		window->add(*Gtk::manage(button));
	}

	// TODO: Check UI properties for resizable
	window->set_resizable(false);
	window->show_all();

	Gtk::Main::run(*window);

	delete window;
	delete jalv_gtk_main;
	zix_sem_post(jalv->done);

	return 0;
}