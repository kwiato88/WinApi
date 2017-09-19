//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <Windowsx.h>

#include "ControlCheckBox.hpp"

namespace WinApi
{
namespace Control
{

bool CheckBox::isChecked() const
{
	return Button_GetCheck(self) == BST_CHECKED;
}
void CheckBox::check()
{
	Button_SetCheck(self, BST_CHECKED);
}
void CheckBox::uncheck()
{
	Button_SetCheck(self, BST_UNCHECKED);
}
void CheckBox::enable()
{
	Button_Enable(self, TRUE);
}
void CheckBox::disable()
{
	Button_Enable(self, FALSE);
}

}
}
