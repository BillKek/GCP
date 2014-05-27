#include "GCP_FormComponent.h"
#include "GCP_Button.h"

GCP_Button::GCP_Button(){
	iType = GCP_BUTTON_SHARPRECT;		//������� - ����� � ������ ����
	checked = false;					//��������� ������ ���� ������
	setStyle(&defStyles.defaultbuttonStyle);
}

void GCP_Button::OnDraw(SDL_Renderer* screen,int w, int h, int formx, int formy, int formw, int formh)
{
	if(!isVisible)
		return;

	GCP_Color cTColor=getStyle()->cTextColor, cBColor=getStyle()->cBackColor;

	//����� ���� ���� � ��������� �������
	if(_isMouseOver){
			cBColor = getStyle()->cBackColorHover;
			cTColor = getStyle()->cTextColorHover;					
		}

	if(checked)
		cBColor = getStyle()->cBackColorHover;

	//���� ������ �������� ������ � ������� 
	if(icon != "")			{
		//������ ������
		if(iType==GCP_BUTTON_ROUNDRECT)
			GCP_Draw::Draw_FillRound(screen,xPos,yPos,width,height,getStyle()->iRoundCff,cBColor);
		if(iType==GCP_BUTTON_SHARPRECT)
			GCP_Draw::Draw_FillRect(screen,xPos,yPos,width,height,cBColor);

		SDL_Surface *bmp = SDL_LoadBMP(icon.c_str());	
		if(bmp != NULL){
			//SDL_Rect recta = {(Sint16)xPos+2,(Sint16)yPos+2,width,height}; 
			//SDL_BlitSurface( hello, NULL, screen, &recta );
			//������ �������� � �������� � ������� �� �� �����
			SDL_Texture *tex = SDL_CreateTextureFromSurface(screen, bmp);
			GCP_Draw::applySurface(xPos+2,yPos+2,tex,screen);
			SDL_FreeSurface( bmp );
			SDL_DestroyTexture(tex); //Create texture should be once !!!!!!!!! not dynamic!!!!!
			//!!!!!!!  �������� �������� � �������� �������� ������ ��� ��� ��������
			//!!!!!!!  ���� ������ ��� ���� ���, � ����� ������ �������� �������� �� ������
		}
		else
		{
			//�������� �� �����������
		}
	}
	else			{
		//������ ������� �������
		string sCaption = getCaption();
		if(iType==GCP_BUTTON_ROUNDRECT)
		{
			GCP_Draw::Draw_FillRound(screen,xPos,yPos,width,height,getStyle()->iRoundCff,cBColor);
			GCP_Draw::Draw_Round(screen,xPos,yPos,width,height,getStyle()->iRoundCff, getStyle()->cBorderColor);
		}
		if(iType==GCP_BUTTON_SHARPRECT)
		{
			GCP_Draw::Draw_FillRect(screen,xPos,yPos,width,height,cBColor);	
			GCP_Draw::Draw_Rect(screen,xPos,yPos,width,height,getStyle()->cBorderColor);
		}
		string font = getFont();
		if(font != "")
		GCP_Draw::renderText(sCaption,xPos+3,yPos+3,screen,&drawdata,cTColor,font,14);
		//TTF_CloseFont(font);
	}

	//������� ����� �� ���� ��� ���� ��������
	basicOnDraw(screen, formx, formy, formw, formh);
}	

