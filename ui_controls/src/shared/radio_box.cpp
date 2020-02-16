#include <xaml/ui/container.hpp>
#include <xaml/ui/controls/radio_box.hpp>

using namespace std;

namespace xaml
{
    radio_box::radio_box() : button()
    {
        add_is_checked_changed([this](radio_box const&, bool) {
            if (get_handle())
            {
                draw_checked();
                draw_group();
            }
        });
#ifdef XAML_UI_COCOA
        add_click([this](button const&) { if(get_handle()) __on_state_changed(); });
#endif // XAML_UI_COCOA
    }

    radio_box::~radio_box() {}

#ifndef XAML_UI_GTK3
    void radio_box::draw_group()
    {
        if (get_parent())
        {
            if (get_is_checked())
            {
                if (auto multic = dynamic_pointer_cast<multicontainer>(get_parent()))
                {
                    for (auto& c : multic->get_children())
                    {
                        if (auto rc = dynamic_pointer_cast<radio_box>(c))
                        {
                            if (c != shared_from_this() && rc->get_group() == get_group())
                            {
                                rc->set_is_checked(false);
                            }
                        }
                    }
                }
            }
            else
            {
                if (auto multic = dynamic_pointer_cast<multicontainer>(get_parent()))
                {
                    bool etcckd = false;
                    for (auto& c : multic->get_children())
                    {
                        if (auto rc = dynamic_pointer_cast<radio_box>(c))
                        {
                            if (c != shared_from_this() && rc->get_group() == get_group())
                            {
                                etcckd |= rc->get_is_checked();
                            }
                        }
                    }
                    if (!etcckd)
                    {
                        set_is_checked(true);
                    }
                }
            }
        }
    }
#endif // !XAML_UI_GTK3
} // namespace xaml