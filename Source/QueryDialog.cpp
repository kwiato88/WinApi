//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include "QueryDialog.hpp"
#include "QueryDialogDef.h"
#include "DialogMsgMatchers.hpp"

namespace WinApi
{

QueryDialog::QueryDialog(InstanceHandle p_hInstance, Handle p_parrent)
	: Dialog(p_hInstance, p_parrent, ResourceId(ID_QUERY_DIALOG))
{
	registerHandler(MsgMatchers::ButtonClick(IDOK), std::bind(&QueryDialog::onOkClick, this));
	registerHandler(MsgMatchers::ButtonClick(IDCANCEL), std::bind(&QueryDialog::onCancelClick, this));
}

void QueryDialog::onInit()
{
	questionControl.init(getItem(ResourceId(ID_QUESTION_TXT)));
	questionControl.setContent(question);
	responseControl.init(getItem(ResourceId(ID_RESPONSE_TXT)));
	responseControl.setContent(initialResponse);
	responseControl.setFocus();
	responseControl.setSelection(0, responseControl.getLineLength(0));
}

void QueryDialog::onOkClick()
{
	response = responseControl.getContent();
	close(Dialog::RESULT_OK);
}

void QueryDialog::onCancelClick()
{
	response.clear();
	close(Dialog::RESULT_CANCEL);
}

void QueryDialog::setQuestion(const std::string& p_txt)
{
	question = p_txt;
}

void QueryDialog::setInitialResponse(const std::string& p_txt)
{
	initialResponse = p_txt;
}

std::string QueryDialog::getResponse()
{
	return response;
}

}
