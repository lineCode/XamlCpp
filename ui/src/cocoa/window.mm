#import <Cocoa/Cocoa.h>
#include <internal/cocoa/drawing.hpp>
#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    window::window() : container(), m_resizable(true)
    {
        add_title_changed([this](window const&, string_view_t) { if (get_handle()) draw_title(); });
        add_location_changed([this](window const&, point) { if (get_handle() && !resizing) __draw({}); });
        add_size_changed([this](control const&, size) { if (get_handle() && !resizing) __draw({}); });
        add_resizable_changed([this](control const&, bool) { if(get_handle()) draw_resizable(); });
    }

    window::~window()
    {
        [(NSWindow*)get_handle() close];
    }

    void window::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            NSRect frame = NSMakeRect(0, 0, get_width(), get_height());
            NSWindow* window = [[NSWindow alloc]
                initWithContentRect:frame
                          styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable
                            backing:NSBackingStoreBuffered
                              defer:false];
            set_handle(window);
            application::current()->wnd_num++;
        }
        NSWindow* window = (NSWindow*)get_handle();
        if (!resizing)
        {
            CGFloat fw = (CGFloat)get_width();
            CGFloat fh = (CGFloat)get_height();
            NSRect frame = [window frame];
            frame.size = { fw, fh };
            frame.origin.x = (CGFloat)get_x();
            frame.origin.y = -frame.size.height - (CGFloat)get_y();
            [window setFrame:frame display:YES];
        }
        draw_title();
        if (get_child())
        {
            draw_child();
        }
        draw_resizable();
        [window makeKeyAndOrderFront:nil];
    }

    void window::draw_title()
    {
        NSWindow* window = (NSWindow*)get_handle();
        NSString* nstitle = [[NSString stringWithUTF8String:m_title.data()] autorelease];
        [window setTitle:nstitle];
    }

    void window::draw_child()
    {
        get_child()->__draw(get_client_region());
    }

    void window::draw_resizable()
    {
        NSWindow* window = (NSWindow*)get_handle();
        if (m_resizable)
            window.styleMask |= NSWindowStyleMaskResizable;
        else
            window.styleMask &= ~NSWindowStyleMaskResizable;
    }

    void window::show()
    {
        __draw({});
    }

    rectangle window::get_client_region() const
    {
        NSWindow* window = (NSWindow*)get_handle();
        NSRect frame = [[window contentView] frame];
        return get_rect(frame);
    }
}