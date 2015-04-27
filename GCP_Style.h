#ifndef GCP_StyleH
#define GCP_StyleH

#include "GCP_SPointer.h"
#include "GCP_Constants.h"

namespace gcp
{
	class GCP_Style;
	typedef GCP_SPointer<GCP_Style> gcp_spStyle;

	enum GCPE_StyleType
	{
		E_DEFAULT,
		E_BUTTON,
		E_BUTTONWHITE,
		E_PANELBLACK,
		E_MESSAGEBOX
	};

	class GCP_Style : public GCP_SPointerBase
	{
	public:

		GCP_Style(GCPE_StyleType sType = E_DEFAULT)
		{
			//Default options
			backgroundColor = c_white;
			backgroundMouseHoverColor = c_white;
			backgroundDisabledColor = c_dkgrey;

			drawTextBorder = false;
			drawTextBackground = false;
			drawTextBackgroundAlpha = 255;

			textColor = c_black;
			textMouseHoverColor = c_black;
			textDisabledColor = c_grey;
			borderColor = c_black;

			textFieldColor = c_orange;
			headBackground = c_black;
			headInactiveColor = c_grey;
			headButtonBackground = c_white;

			borderWidth = 2;
			roundCff = 7;
			font.size = 14;

			switch (sType)
			{
			case E_BUTTON:
				textColor = c_white;
				backgroundColor = c_black;
				textMouseHoverColor = c_black;
				backgroundMouseHoverColor = c_white;
				font.align = E_CENTER;
				break;
			case E_BUTTONWHITE:
				textColor = c_black;
				textMouseHoverColor = c_white;
				backgroundColor = c_white;
				backgroundMouseHoverColor = c_aquadark;
				font.align = E_CENTER;
				break;

			case E_PANELBLACK:
				textColor = c_white;
				backgroundColor = c_black;
				textMouseHoverColor = c_white;
				backgroundMouseHoverColor = c_black;
				font.size = 20;
				font.align = E_CENTER;
				break;

			case E_MESSAGEBOX:
				font.align = E_CENTER;
				break;

			default:
				break;
			}
		}

	public:
		GCP_Font font;
		GCP_Color backgroundColor;             //���
		GCP_Color backgroundMouseHoverColor;   //��� ��� ���������
		GCP_Color backgroundDisabledColor;     //��� ������������ ��������

		GCP_Color textColor;                   //���� ������
		GCP_Color textMouseHoverColor;         //���� ������ ��� ���������
		GCP_Color textDisabledColor;           //���� ������������ ��������
		GCP_Color borderColor;                 //���� �����

		GCP_Color textFieldColor;              //���� ������ � ����� �����
		GCP_Color headBackground;              //��� ��������� (� ������)
		GCP_Color headInactiveColor;           //��� ��������� ����������� ��������
		GCP_Color headButtonBackground;        //��� ������ ������������� � ��������� (������� �� �����)

		short borderWidth;                     //������� �����
		short roundCff;                        //����������� ���������� �����

		bool drawTextBorder;				   //�������� ����� ������ ������
		bool drawTextBackground;			   //�������� ��� ��� �������
		gcpUint8 drawTextBackgroundAlpha;      //������������ ���� ������
	};
}
#endif
