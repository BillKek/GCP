#include "GCP_Edit.h"
#include "GCP_FormComponent.h"
#include "GCP_Math.h"
using namespace gcp;

GCP_Edit::GCP_Edit()
{
	_isEditingText = false;			//����������� ��������� �����. (��������� ������ ���� ���)
	_sTextInputDraw = "";			//����� ������� ������
	_iDrawDash = 0;					//���������� ��� ������� ��� ������ ������
	_iTextDrawIndex = 0;			//� ������ ������� ������� ����� (���� �� ������� �������)
	inputType = ALL;//������ ����� ������
	setStyle(GCP_ButtonWhiteStyle);
}

void GCP_Edit::OnDraw(const GCP_Event &event)
{
	if(!isVisible())
		return;

	//������� ����� � �������
	GCP_Draw::Render()->Draw_FillRound(_position, 1, getStyle()->backgroundColor);
	GCP_Draw::Render()->Draw_Round(_position, 1, getStyle()->borderColor);

	int iBorderWidth = getStyle()->borderWidth;
	GCP_Draw::Render()->Draw_FillRound(_position.x() + iBorderWidth, _position.y() + iBorderWidth, _position.width() - iBorderWidth * 2, _position.height() - iBorderWidth * 2, 1, getStyle()->textFieldColor);
	//TTF_Font* font = TTF_OpenFont(getFont().c_str(),14);

	SDL_Rect rect = {_position.x()+iBorderWidth,_position.y()+iBorderWidth*2,_position.width()-iBorderWidth*2,_position.height()-iBorderWidth*2 };
	SDL_SetTextInputRect(&rect); //�� ������ ����� � ��� ��� ����
	GCP_Rect<int> textPos(_position.x() + iBorderWidth * 2, _position.y() + iBorderWidth * 2, _position.width(), _position.height());
	GCP_Draw::Render()->Draw_Text(_sTextInputDraw, textPos, getStyle(), &drawdata);
	basicOnDraw(event);


	_iDrawDash++;
	if(_iDrawDash>25 && _isEditingText)
	{
		//MAKE THIS AS A GCP_DRAW FUNCTION
		int sw,sh;
		//��� ����� ���� �������� � ������������ ������� ������ � �������� ��� ������ ����������� ����� �� ������������ � ������ ��������� (1 ��� 2 �����)
		//������ ��� �������������� � ���������� ��������� �������� CP1251TOUTF �� ��� ��������� �� �� ���� �����������
		GCP_Draw::Render()->GetTextSize(_sTextInputDraw, sw, sh, getStyle());
		//������ �������������
		int iBorderWidth = getStyle()->borderWidth;
		GCP_Draw::Render()->Draw_Line(_position.x() + sw + iBorderWidth * 2, _position.y() + iBorderWidth * 2, _position.x() + sw + iBorderWidth * 2, _position.y() + iBorderWidth * 2 + sh, getStyle()->textColor);
		if(_iDrawDash>20)	//!!! ������ ��������
			_iDrawDash = 0;
	}

}

gcp_formEvent GCP_Edit::OnEvent(const GCP_Event &event)
{
	gcp_formEvent evt;
	evt.isEventInsideForm = false;
	evt.isEventOnFormHead = false;
	evt.isFormDragged = false;
	if(!isVisible())
		return evt;

	switch(event.eventType)
	{
	case GCP_ON_LEFT_CLICK:
		_isEditingText = true;
		break;
	case GCP_ON_GLEFT_CLICK:
		_isEditingText = false;
		break;
	case GCP_ON_GKEYDOWN:
		onKeyDown(event.keycode);
		break;
	case GCP_ON_GTEXTINPUT:
		onTextInput(event.sTextInput);
		break;
	}

	basicOnEvent(event);
	return evt;
}

/*bool GCP_Edit::OnMouseLeftClick(SDL_MouseButtonEvent mousebutton)	{
	if(!isVisible)
		return false;

	_isEditingText = true;


	basicOnMouseLeftClick(mousebutton);
	return true;
}*/

/*gcp_formEvent GCP_Edit::OnMouseGlobalLeftClick(SDL_MouseButtonEvent mousebutton)	{
	gcp_formEvent evt;
	evt.isEventInsideForm = false;
	evt.isEventOnFormHead = false;
	evt.isFormDragged = false;
	if(!isVisible)
		return evt;

	_isEditingText = false;

	basicOnMouseGlobalLeftClick(mousebutton);
	return evt;
}*/

void GCP_Edit::onKeyDown(int keycode)
{
	if(!isVisible() || !_isEditingText)
		return;

	switch (keycode)
	{
	case SDLK_RETURN:
		_sTextInput = "";
		break;
	case SDLK_BACKSPACE:
	{
		if(_sTextInput.size() > 0)
		{
			if (_inputSizeMap.size() < 1)
				throw;
			int letterSize = _inputSizeMap.pop_back();
			while (letterSize > 0)
			{
				if(_sTextInput.size() > 0)
					_sTextInput.erase(_sTextInput.size() - 1);
				letterSize--;
			}
		}
		corelateText();
	}
		break;
	}
}

bool GCP_Edit::onTextInput(const string &text)
{
	if(!isVisible() || !_isEditingText)
		return false;

	//unsigned int i = 0;
	//while(textevent.text[i]!=0)
	//	i++;


	///This convert two digits from unicode input to one digit that point to russian ASCI
	///Of course it is not. but some of digits
	///Find the function to convert this
	///OR PLACE IN STRING CONTAINING THE UTF SOMETHING THAT WILL INDICATE THIS
	///AND LATER USE TTF_RenderUTF8_Blended TO DRAW THIS
	///CRAP THIS WHILL REQUIRE TO PARSE _sCAption to multiple captions
	//TTF_RenderUNICODE_Solid and TTF_RenderUTF8_Blended
	/*wchar_t textR;
	char mbchar;
	mbstowcs(&textR,text.c_str(),2);
	wctomb(&mbchar,textR);*/


	/*if(i==2){
		s[0] = textevent.text[1]-textevent.text[0]; //������� ��� ������ ��� ����� ���� ���������� ������� �� 2 ���� � 1 ���� ������� ������
		s[1] = 0;
	}
	else{
		s[0] =	textevent.text[0];
		s[1] =	0;
	}*/

	//????
	//�������� � ��� ��� ������� ������� ����� ��������� ��� � ���� 2 ���� ��� � � ���� 1 �����
	//????
	char* character = (char*)text.c_str();
	switch(inputType)
	{
	case DIGITONLY:
		if (text.size() > 1 || !GCP_Math::isNum(character))
			return false;
		break;

	case DOUBLEDIGIT:
		if (text.size() > 1 || !GCP_Math::isNum(character) && text[0] != '.' && text[0] != '-' )
			return false;
		if (text[0] == '.')
		{
			for(size_t i=0; i<_sTextInput.size(); i++)
				if(_sTextInput.at(i) == '.')
					return false;
		}
		if (text[0] == '-')
		{
			for(size_t i=0; i<_sTextInput.size(); i++)
				if(_sTextInput.at(i) == '-')
					return false;
		}
		break;

	case TEXTONLY:
		//if (text.size() == 1 && GCP_Math::isNum(character) && text[0] != ' ')
		//	return false;
		break;
		//case GCP_Edit_InputType_Enum::ALL:
	}

	_inputSizeMap.push_back(text.size());
	_sTextInput.append(character);
	corelateText();

	return true;
}
void GCP_Edit::corelateText()
{
	//���� ����� ������� ������� ��� �������� ���������� ����� ��������������
	//������� ������ ��������� ������� � ������� �� ��������� � ����������
	if(getStyle().getPointer() == NULL)
		return;

	_sTextInputDraw = _sTextInput;
	_iTextDrawIndex = 0;

	
	int sw,sh;
	GCP_Draw::Render()->GetTextSize(_sTextInput, sw, sh, getStyle());

	int k = 0;
	while(sw >_position.width() - getStyle()->borderWidth*3)
	{
		_sTextInputDraw = "";
		_iTextDrawIndex += _inputSizeMap.at(k);
		k++;

		char buffer[100];
		_sTextInput.copy(buffer,_sTextInput.size()-_iTextDrawIndex,_iTextDrawIndex);
		buffer[_sTextInput.size()-_iTextDrawIndex]='\0';
		_sTextInputDraw.append(buffer);


		GCP_Draw::Render()->GetTextSize(_sTextInputDraw, sw, sh, getStyle());
	}

}
/*bool GCP_Edit::OnTextEdit(const GCP_Event &event)
{
	if(!isVisible() || !_isEditingText)
		return false;

	return true;
}*/

void GCP_Edit::setText(const std::string &str, int byteperletter)
{
	for(size_t i = 0; i < str.size(); i++)
	{
		bool isOneByte = GCP_Math::isOneByteChar(str.at(i));
		_inputSizeMap.push_back(isOneByte ? 1 : 2);
		if (!isOneByte)
			i++;
	}

	_sTextInput = str;
	corelateText();
}

string GCP_Edit::getText()
{
	return _sTextInput;
}

