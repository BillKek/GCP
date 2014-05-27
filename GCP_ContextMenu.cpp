#include "GCP_ContextMenu.h"
#include "GCP_Button.h"
#include "GCP_Vector.h"


//����������� ���� ��� ����� ������, ������� ����������� �������� �� ������ ���� �� ��������	
GCP_ContextMenu::GCP_ContextMenu(){
	bool isVisible = false;
	int iType = GCP_MENU_MVERTICAL;		//���� ����� ���� ������������ ���� ��������������
}



GCP_ContextMenu::~GCP_ContextMenu(){
	//for(int i =0; i<menu.size(); i++)
	//	delete menu.at(i);				//������ ��� ������ ������� ���� � ����. ��� ��� ��������� �� ��� �������� ������ �����
}



void GCP_ContextMenu::setLock(bool flag)
{
	_isLocked = flag;					//��������� ��������� ������� ������������� ����. (����� �������� ����� ���������� ���������)
}

	     
void GCP_ContextMenu::initTexture(SDL_Renderer* screen)
{
	unsigned int iMenuSize = menu.size();	
	for(unsigned int i=0; i<iMenuSize; i++)	
		menu.at(i)->initTexture(screen);	//����� ��� ������� �� ������ ������. �� �� �������� ������ ������������ ������������ �������� ������
}

void GCP_ContextMenu::setFont(std::string str)
{
	unsigned int iMenuSize = menu.size();
	for(unsigned int i=0; i<iMenuSize; i++){
		menu.at(i)->setFont(str);
	}
}

void GCP_ContextMenu::addButton(GCP_SPointer<GCP_Button> &button)	{
	button->setFont(getFont());				//�������� ������ ���� � ����� ������� ������ ���� ����� �� ������ �������� �������� � ���������� ����������
	menu.push_back(button);
}

void GCP_ContextMenu::addButton(string type)	{			
	_iSeparators.push_back( menu.size());	//� ���� ����� ��������� �����������. ����������� �� ������, � ������ / ���� �����
}

void GCP_ContextMenu::open(int x, int y)
{
	isVisible = true;						//������� ���� � ������� �����������
	xPos = x;
	yPos = y;
}

void GCP_ContextMenu::close(void *obj)
{
	isVisible = false;
}

	
bool GCP_ContextMenu::OnMouseGlobalLeftHoldMotion(SDL_MouseMotionEvent motion, int fx, int fy, int fw, int fh)	{
	//Context menu is not dragable
	return false;
}

gcp_formEvent GCP_ContextMenu::OnEvent( const int GCP_EVENT, sdl_events events)
{
	gcp_formEvent evt;
	evt.isEventInsideForm = false;
	evt.isEventOnFormHead = false;
	evt.isFormDragged = false;
	if(!isVisible || _isLocked)
		return evt;

	unsigned int iMenuSize = menu.size();
	for(unsigned int i=0; i<iMenuSize; i++){
			menu.at(i)->OnEvent(GCP_EVENT,events);				//���������� �������
			if(!events.isPassingOnlyGlobalEvent)
			if(menu.at(i)->checkCollisionBox(events.mousex, events.mousey)){
				menu.at(i)->OnEvent(GCP_EVENT+1,events);		//��������� �������
			}
		}

	basicOnEvent(GCP_EVENT,events);
	return evt;
}

		

void GCP_ContextMenu::OnDraw(SDL_Renderer* screen,int w, int h, int formx, int formy, int formw, int formh)
{
	if(!isVisible) 
		return;

	//����� ������ �� �����
	//Calculate Size For OutOfSurface Correlation
	int maxMenuWidth = 0;							//������������ ������ ����������� � ����
	int maxMenuHeight = 0;							//������������ ������ ����������� � ����
	int stackWidth=0, stackHeight=0;				//��������� ������ � ������ ������ ����
	unsigned int iMenuSize = menu.size();
	for(unsigned int i=0; i<iMenuSize; i++)	{
		if (menu.at(i)->height>maxMenuHeight)
			maxMenuHeight = menu.at(i)->height;
		if (menu.at(i)->height>maxMenuWidth)
			maxMenuWidth = menu.at(i)->width;

		if(iType==GCP_MENU_MVERTICAL){
			stackHeight += menu.at(i)->getPosition().height;
			stackWidth = maxMenuWidth;
		}

		if(iType==GCP_MENU_MHORIZONTAL){
			stackWidth += menu.at(i)->getPosition().width;
			stackHeight = maxMenuHeight;
		}
	}

	//������� ���� ���� ��� ������� �� ����� / �����
	GCP_Point normPos = GCP_Math::normalizeRectInRect(GCP_Point(formx,formy),
						GCP_Point(formx+formw,formy+formh),xPos,yPos,stackWidth+iMenuSize,stackHeight+iMenuSize,1);
	xPos = (int)normPos.X; 
	yPos = (int)normPos.Y;


	//������ ����� �� ��� ��������� ��� ���� ��������� ��� ���������� ��� � ������ ���� ��� �������� ���� ����
	//!!! ����������� �� ����������� � ����� ����!!! 
	stackWidth=0;   //recalc for calculationg drawing coordinates
	stackHeight=0;  
	int currentSeparator=0;
	for(unsigned int i=0; i<iMenuSize; i++)	{

		if(iType==GCP_MENU_MVERTICAL){
			if(currentSeparator < _iSeparators.size())
			if(i==_iSeparators.at(currentSeparator))	{
				stackHeight += 5; currentSeparator++;
			}
			menu.at(i)->setPosition(xPos, yPos + stackHeight);
			stackHeight += menu.at(i)->getPosition().height+1;
			stackWidth = maxMenuWidth;
		}

		if(iType==GCP_MENU_MHORIZONTAL){
			if(currentSeparator < _iSeparators.size())
			if(i==_iSeparators.at(currentSeparator)){
				stackWidth += 5;currentSeparator++;
			}
			menu.at(i)->setPosition(xPos + stackWidth, yPos);
			stackWidth += menu.at(i)->getPosition().width+1;
			stackHeight =  maxMenuHeight;
		}			
	}

	//if(!isContextMenuBlocking)
		//_isContextMenuOpened = false;

	//������ ��� ���� ����������
	for(unsigned int i=0; i<iMenuSize; i++){
		menu.at(i)->setWidthHeight(maxMenuWidth,maxMenuHeight);
		//menu.at(i)->setColor(cBackColor, cTextColor, cBackColorHover, cTextColorHover);
		//menu.at(i)->iRoundCff = iRoundCff;
		menu.at(i)->OnDraw(screen, w,h,formx,  formy,  formw,  formh);
		//if(!isContextMenuBlocking)
		//_isContextMenuOpened = (menu.at(i)->_isContextMenuOpened || _isContextMenuOpened);
	}

	//����������� ��������� �������� ������ ���������
	for(unsigned int i=0; i<iMenuSize; i++)
		menu.at(i)->OnDrawInfo(screen, formx,  formy,  formw,  formh);

			
	GCP_Draw::Draw_Round(screen,xPos, yPos, stackWidth, stackHeight, getStyle()->iRoundCff, c_black); 
			
}//OnDraw
