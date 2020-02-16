#include <xaml/ui/controls/combo_box.hpp>

using namespace std;

namespace xaml
{
    void combo_box::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            draw_editable();
            draw_items();
        }
        rectangle real = region - get_margin();
        __set_size_noevent({ real.width, real.height });
        draw_sel();
        draw_text();
    }

    void combo_box::draw_size()
    {
        auto [rw, rh] = to_native<tuple<gint, gint>>(get_size());
        gtk_widget_set_size_request(get_handle(), rw, rh);
    }

    void combo_box::draw_text()
    {
        if (m_is_editable)
        {
            auto entry = gtk_bin_get_child(GTK_BIN(get_handle()));
            if (m_text)
            {
                gtk_entry_set_text(GTK_ENTRY(entry), m_text->c_str());
            }
            else
            {
                if (get_sel_id() < 0 || get_sel_id() >= get_items().size())
                {
                    gtk_entry_set_text(GTK_ENTRY(entry), "");
                }
                else
                {
                    gtk_entry_set_text(GTK_ENTRY(entry), get_items()[get_sel_id()]->c_str());
                }
            }
        }
    }

    void combo_box::draw_items()
    {
        for (auto item : get_items())
        {
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(get_handle()), item->c_str());
        }
    }

    void combo_box::draw_sel()
    {
        gtk_combo_box_set_active(GTK_COMBO_BOX(get_handle()), (gint)get_sel_id());
    }

    void combo_box::draw_editable()
    {
        if (m_is_editable)
        {
            set_handle(gtk_combo_box_text_new_with_entry());
        }
        else
        {
            set_handle(gtk_combo_box_text_new());
        }
        g_signal_connect(G_OBJECT(get_handle()), "changed", G_CALLBACK(combo_box::on_changed), this);
    }

    void combo_box::__size_to_fit()
    {
        int width = gtk_widget_get_allocated_width(get_handle());
        int height = gtk_widget_get_allocated_height(get_handle());
        __set_size_noevent({ (double)width, (double)height });
    }

    void combo_box::on_changed(GtkComboBox* widget, gpointer data)
    {
        combo_box* self = (combo_box*)data;
        self->set_sel_id(gtk_combo_box_get_active(widget));
        if (self->get_is_editable())
        {
            auto entry = gtk_bin_get_child(GTK_BIN(self->get_handle()));
            self->set_text(gtk_entry_get_text(GTK_ENTRY(entry)));
        }
        else
        {
            self->set_text(self->get_items()[self->get_sel_id()]);
        }
    }

    void combo_box::insert_item(size_t index, string_t const& value)
    {
        gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(get_handle()), (gint)index, value.c_str());
    }

    void combo_box::remove_item(size_t index)
    {
        gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(get_handle()), (gint)index);
    }

    void combo_box::clear_items()
    {
        gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(get_handle()));
    }

    void combo_box::replace_item(size_t index, string_t const& value)
    {
        remove_item(index);
        insert_item(index, value);
    }
} // namespace xaml