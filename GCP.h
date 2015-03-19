////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////������� SDL GUI CHECK POINT 
/////////////////////////////////////��������� ��� SDL
////////////////////////////////////////////////////////////////////////////////////////////
//////������ �������� �� ��������� ������� SDL � �������� �� ������ ����������: _mainForm
//////���������� ������ ����� ������������ ������ � ������ ������ ����� ���������� ������������ �����
//////��� ������ ����������� ���� ������� ������� handleEvents � draw � ����� ���������
//////������:
//////=====ON CREATE
//////GCP_Controller *Interface = new GCP_Controller(sdlRenderer,SCREEN_WIDTH,SCREEN_HEIGHT);
//////Interface->setFont(font_path.str());								// ���� � ����� ������� 
//////GCP_Form *MainInterfaceForm = Interface->createForm(sdlRenderer); // ������� ����� ����������
//////=====MAIN WHILE
//////SDL_RenderClear(sdlRenderer);								
//////while( SDL_PollEvent( &event ) )		//���������� ���� ������� �� SDL
//////{ 		
//////	Interface->handleEvents(event);		//������������ ���������	
//////	if( event.type == SDL_QUIT ) 
//////		return -1;
//////} 	
//////draw();									//������ ���� ������
//////Interface->draw();						//������ ���������	
//////SDL_RenderPresent(sdlRenderer);			//������� �� �����
//////================
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
#ifndef GCP_MainH
#define GCP_MainH
#define GCP_VERSION "1.01c"
#include "GCP_Form.h"
#include "GCP_Button.h"
#include "SDL.h"

namespace gcp
{
	class GCP_Controller : public GCP_SPointerBase
	{
	protected:
		int _width, _height;				      //������ ������ ������. ��������� ������� ������� ���� �������� ���������� �� �����. GCP ����� ��������� ���.
		GCP_SPointer<GCP_Form> _mainForm;	//������� ����� ����������
		bool _isLeftHold;					      //������������� ��� ������� ���� �� ����
	public:
		GCP_Controller(SDL_Renderer *sdlRenderer, int width, int height);
		~GCP_Controller();
		void setFont(const string &directory_path_string);
		GCP_SPointer<GCP_Form> createForm();
		void handleEvents(const SDL_Event &event);
		void draw();
		void deInit()
		{
			GCP_Draw::DeInit();
		}
	};
}
#endif
