#include "GCP_Edit.h"
#include "GCP_FormComponent.h"


GCP_Edit::GCP_Edit()
{
	_isEditingText = false;			//����������� ��������� �����. (��������� ������ ���� ���)
	_sTextInputDraw = "";			//����� ������� ������
	_iDrawDash = 0;					//���������� ��� ������� ��� ������ ������
	_iTextDrawIndex = 0;			//� ������ ������� ������� ����� (���� �� ������� �������)
	inputType = GCP_EDIT_ALL;		//������ ����� ������
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
	//!! ������ ������ !!!
   GCP_Draw::Render()->Draw_Text(_sTextInputDraw, _position.x() + iBorderWidth * 2, _position.y() + iBorderWidth * 2, getStyle(), &drawdata);
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
			OnKeyDown(event);
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

void GCP_Edit::OnKeyDown(const GCP_Event &event){
	if(!isVisible() || !_isEditingText)
		return;

	/*switch (keybevent.keysym.sym)
    {
        case SDLK_RETURN:
			 _sTextInput = "";
             break;
        case SDLK_BACKSPACE:
             {
				//!!! ��� �� ����������� ������� ���� � �������. ���� �������� ������ � ������� ��������� �� ���� � ��� ������� �� ������ ������ ���������� ����
				//������ ��������� ������ ���� ����. �.�  ������� ����� ��������� �� 2 ������� ��������
				if(_sTextInput.size()>0)
					_sTextInput.erase(_sTextInput.size() - 1);
				corelateText();
             }
             break;
    }*/
}



//////////////////////////////////////////
//////////////////////////////////////////
//������ �����. �������� ��������������� ����� �� UTF � CP1251 � ������� �������� ��������� ��� � ������
//�� ���� �� ����������
union byte
{
	unsigned char chr;
	struct bit
	{
		bool b1;
		bool b2;
		bool b3;
		bool b4;
		bool b5;
		bool b6;
		bool b7;
		bool b8;
	}bits;

};
union rusdigit
{
	wchar_t letter;
	struct b
	{
		byte byte1;
		byte byte2;
	}bytes;
};
//////////////////////////////////////////
//////////////////////////////////////////


bool GCP_Edit::OnTextInput(const GCP_Event &event)
{
	if(!isVisible() || !_isEditingText)
		return false;

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
	/*wchar_t text;
	char mbchar;
	mbstowcs(&text,"gh",2);
	wctomb(&mbchar,text);*/

	//�������� � ��� ��� ������� ������� ����� ��������� ��� � ���� 2 ���� ��� � � ���� 1 �����

	/*if(i==2){
		s[0] = textevent.text[1]-textevent.text[0]; //������� ��� ������ ��� ����� ���� ���������� ������� �� 2 ���� � 1 ���� ������� ������
		s[1] = 0;
	}
	else{
		s[0] =	textevent.text[0];
		s[1] =	0;
	}*/


	/*string s =  textevent.text;
	switch(inputType)
	{
		case GCP_EDIT_DIGITONLY:
			if(!GCP_Math::isNum((char*)s.c_str()))
				return false;
			break;

		case GCP_EDIT_DOUBLEDIGIT:
			if(!GCP_Math::isNum((char*)s.c_str()) && s[0]!= '.')
				return false;
			if(s[0]== '.'){
				for(unsigned int i=0; i<_sTextInput.size(); i++)
					if(_sTextInput.at(i) == '.')
						return false;
			}
			break;

		case GCP_EDIT_TEXTONLY:
			if(GCP_Math::isNum((char*)s.c_str()) && s[0]!= ' ')
				return false;
			break;
		//case GCP_Edit_InputType_Enum::ALL:
	}
	_sTextInput.append(s.c_str());
	corelateText();
   */
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
		
	
	while(sw>_position.width()-getStyle()->borderWidth*2)
	{
		_sTextInputDraw = "";
		_iTextDrawIndex++;
		char buffer[100];
		_sTextInput.copy(buffer,_sTextInput.size()-_iTextDrawIndex,_iTextDrawIndex);
		buffer[_sTextInput.size()-_iTextDrawIndex]='\0';
		_sTextInputDraw.append(buffer);


      GCP_Draw::Render()->GetTextSize(_sTextInput, sw, sh, getStyle());
	}

}
bool GCP_Edit::OnTextEdit(const GCP_Event &event)
{
	if(!isVisible() || !_isEditingText)
		return false;

	return true;
}

void GCP_Edit::setCaption(std::string str)
{
	_sTextInput = str;
	corelateText();
}

string GCP_Edit::getCaption()
{
	return _sTextInput;
}

