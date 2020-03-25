#include <vector>
#include <wil/result_macros.h>
#include <xaml/ui/msgbox.hpp>
#include <xaml/ui/native_control.hpp>

#include <CommCtrl.h>

using namespace std;

namespace xaml
{
    msgbox_result msgbox(shared_ptr<window> parent, string_view_t message, string_view_t title, string_view_t instruction, msgbox_style style, array_view<msgbox_button> buttons)
    {
        TASKDIALOGCONFIG config{};
        config.cbSize = sizeof(TASKDIALOGCONFIG);
        if (parent) config.hwndParent = parent->get_handle()->handle;
        config.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION | TDF_SIZE_TO_CONTENT;
        config.pszWindowTitle = title.data();
        config.pszMainInstruction = instruction.data();
        config.pszContent = message.data();
        TASKDIALOG_COMMON_BUTTON_FLAGS flags = 0;
        vector<TASKDIALOG_BUTTON> cbs;
        for (auto& button : buttons)
        {
            switch (button.index())
            {
            case 0:
                flags |= (TASKDIALOG_COMMON_BUTTON_FLAGS)get<msgbox_common_button>(button);
                break;
            case 1:
            {
                msgbox_custom_button const& b = get<msgbox_custom_button>(button);
                cbs.push_back({ (int)b.result, b.text.data() });
                break;
            }
            }
        }
        config.dwCommonButtons = flags;
        config.cButtons = (UINT)cbs.size();
        config.pButtons = cbs.data();
        switch (style)
        {
        case msgbox_style::info:
        case msgbox_style::question:
            config.pszMainIcon = TD_INFORMATION_ICON;
            break;
        case msgbox_style::warning:
            config.pszMainIcon = TD_WARNING_ICON;
            break;
        case msgbox_style::error:
            config.pszMainIcon = TD_ERROR_ICON;
            break;
        }
        int result;
        THROW_IF_FAILED(TaskDialogIndirect(&config, &result, nullptr, nullptr));
        return (msgbox_result)result;
    }
} // namespace xaml
