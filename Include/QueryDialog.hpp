//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE

#include "Dialog.hpp"
#include "ControlEdit.hpp"
#include "ControlText.hpp"

namespace WinApi
{

class QueryDialog : public Dialog
{
public:
	QueryDialog(InstanceHandle p_hInstance, Handle p_parrent);

	void setQuestion(const std::string& p_txt);
	void setInitialResponse(const std::string& p_txt);
	std::string getResponse();

private:
	void onInit() override;
	void onOkClick();
	void onCancelClick();

	std::string question;
	std::string response;
	std::string initialResponse;

	Control::Text questionControl;
	Control::Edit responseControl;
};

}
