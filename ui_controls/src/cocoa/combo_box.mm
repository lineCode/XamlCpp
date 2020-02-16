#import <cocoa/XamlComboBoxDelegate.h>
#include <xaml/ui/controls/combo_box.hpp>

@implementation XamlComboBoxDelegate
- (void)comboBoxSelectionDidChange:(NSNotification*)obj
{
    xaml::combo_box* ptr = (xaml::combo_box*)self.classPointer;
    ptr->__on_changed();
}
@end

using namespace std;

namespace xaml
{
    void combo_box::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            NSComboBox* combo = [NSComboBox new];
            combo.bezeled = YES;
            XamlComboBoxDelegate* delegate = [[XamlComboBoxDelegate alloc] initWithClassPointer:this];
            combo.delegate = delegate;
            set_handle(combo);
            __set_delegate(delegate);
            draw_items();
        }
        rectangle real = region - get_margin();
        __set_rect(real);
        draw_sel();
        draw_editable();
    }

    void combo_box::draw_size()
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        NSRect frame = combo.frame;
        frame.size = to_native<NSSize>(get_size());
        combo.frame = frame;
    }

    void combo_box::draw_text()
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        if (m_text)
        {
            combo.stringValue = [NSString stringWithUTF8String:m_text->c_str()];
        }
        else
        {
            if (get_sel_id() < 0 || get_sel_id() >= get_items().size())
            {
                combo.stringValue = @"";
            }
            else
            {
                combo.stringValue = [NSString stringWithUTF8String:get_items()[get_sel_id()]->c_str()];
            }
        }
    }

    void combo_box::draw_items()
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        for (auto item : get_items())
        {
            [combo addItemWithObjectValue:[NSString stringWithUTF8String:item->c_str()]];
        }
    }

    void combo_box::draw_sel()
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        [combo selectItemAtIndex:(NSInteger)get_sel_id()];
    }

    void combo_box::draw_editable()
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        combo.drawsBackground = m_is_editable;
        combo.editable = m_is_editable;
        combo.selectable = m_is_editable;
    }

    void combo_box::__size_to_fit()
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        [combo sizeToFit];
        NSRect frame = combo.frame;
        __set_size_noevent(from_native(frame.size));
    }

    void combo_box::__on_changed()
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        set_sel_id(combo.indexOfSelectedItem);
        set_text(combo.stringValue.UTF8String);
    }

    void combo_box::insert_item(size_t index, string_t const& value)
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        [combo insertItemWithObjectValue:[NSString stringWithUTF8String:value.c_str()]
                                 atIndex:(NSInteger)index];
    }

    void combo_box::remove_item(size_t index)
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        [combo removeItemAtIndex:(NSInteger)index];
    }

    void combo_box::clear_items()
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        [combo removeAllItems];
    }

    void combo_box::replace_item(size_t index, string_t const& value)
    {
        remove_item(index);
        insert_item(index, value);
    }
}