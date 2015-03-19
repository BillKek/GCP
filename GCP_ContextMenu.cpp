#include "GCP_ContextMenu.h"
#include "GCP_Button.h"
#include "GCP_Vector.h"
using namespace gcp;

//����������� ���� ��� ����� ������, ������� ����������� �������� �� ������ ���� �� ��������
GCP_ContextMenu::GCP_ContextMenu(){
	_isVisible = false;
	iType = GCP_MENU_MVERTICAL;		//���� ����� ���� ������������ ���� ��������������
}



GCP_ContextMenu::~GCP_ContextMenu(){
	//for(int i =0; i<menu.size(); i++)
	//	delete menu.at(i);				//������ ��� ������ ������� ���� � ����. ��� ��� ��������� �� ��� �������� ������ �����
}



void GCP_ContextMenu::setLock(bool flag)
{
	_isLocked = flag;					//��������� ��������� ������� ������������� ����. (����� �������� ����� ���������� ���������)
}


/*void GCP_ContextMenu::initTexture(SDL_Renderer* screen)
{
	unsigned int iMenuSize = menu.size();
	for(unsigned int i=0; i<iMenuSize; i++)
		menu.at(i)->initTexture(screen);	//����� ��� ������� �� ������ ������. �� �� �������� ������ ������������ ������������ �������� ������
}*/

void GCP_ContextMenu::addButton(const GCP_SPointer<GCP_Button> &button)	{
	menu.push_back(button);
}

void GCP_ContextMenu::addButton(const string &type)
{
	_iSeparators.push_back( menu.size());	//� ���� ����� ��������� �����������. ����������� �� ������, � ������ / ���� �����
}

void GCP_ContextMenu::open(int x, int y)
{
	_isVisible = true;						//������� ���� � ������� �����������
	_position.topLeft.X = x;
	_position.topLeft.Y = y;
}

void GCP_ContextMenu::close(void *obj)
{
	_isVisible = false;
}


gcp_formEvent GCP_ContextMenu::OnMouseGlobalLeftHoldMotion(const GCP_Event &event)	{
	//Context menu is not dragable
	return gcp_formEvent();
}

gcp_formEvent GCP_ContextMenu::OnEvent(const GCP_Event &event)
{
	gcp_formEvent evt;
	evt.isEventInsideForm = false;
	evt.isEventOnFormHead = false;
	evt.isFormDragged = false;
	if(!isVisible() || _isLocked)
		return evt;

	unsigned int iMenuSize = menu.size();
	for(unsigned int i=0; i<iMenuSize; i++){
		menu.at(i)->OnEvent(event);				//���������� �������
		if(!event.isPassingOnlyGlobalEvent)
			if(menu.at(i)->checkCollisionBox(event.mousex, event.mousey)){
				menu.at(i)->OnEvent(MakeEventLocal(event));		//��������� �������
			}
	}

	basicOnEvent(event);
	return evt;
}



void GCP_ContextMenu::OnDraw(const GCP_Event &event)
{
	if(!isVisible())
		return;

	//����� ������ �� �����
	//Calculate Size For OutOfSurface Correlation
	int maxMenuWidth = 0;							//������������ ������ ����������� � ����
	int maxMenuHeight = 0;							//������������ ������ ����������� � ����
	int stackWidth=0, stackHeight=0;				//��������� ������ � ������ ������ ����
	unsigned int iMenuSize = menu.size();
	for(unsigned int i=0; i<iMenuSize; i++)	{
		if (menu.at(i)->getPosition().height()>maxMenuHeight)
			maxMenuHeight = menu.at(i)->getPosition().height();
		if (menu.at(i)->getPosition().height()>maxMenuWidth)
			maxMenuWidth = menu.at(i)->getPosition().width();

		if(iType==GCP_MENU_MVERTICAL){
			stackHeight += menu.at(i)->getPosition().height();
			stackWidth = maxMenuWidth;
		}

		if(iType==GCP_MENU_MHORIZONTAL){
			stackWidth += menu.at(i)->getPosition().width();
			stackHeight = maxMenuHeight;
		}
	}

	//������� ���� ���� ��� ������� �� ����� / �����
	GCP_Point<int> normPos = GCP_Math::normalizeRectInRect<int>(
				GCP_Rect<int>(_position.x(), _position.y(), stackWidth + iMenuSize, stackHeight + iMenuSize),
				event.drawRect, 1);
	_position.topLeft.X = (int)normPos.X;
	_position.topLeft.Y = (int)normPos.Y;


	//������ ����� �� ��� ��������� ��� ���� ��������� ��� ���������� ��� � ������ ���� ��� �������� ���� ����
	//!!! ����������� �� ����������� � ����� ����!!!
	stackWidth=0;   //recalc for calculationg drawing coordinates
	stackHeight=0;
	unsigned int currentSeparator=0;
	for(unsigned int i=0; i<iMenuSize; i++)	{

		if(iType==GCP_MENU_MVERTICAL){
			if(currentSeparator < _iSeparators.size())
				if(i==_iSeparators.at(currentSeparator))	{
					stackHeight += 5; currentSeparator++;
				}
			menu.at(i)->setPosition(_position.x(), _position.y() + stackHeight);
			stackHeight += menu.at(i)->getPosition().height()+1;
			stackWidth = maxMenuWidth;
		}

		if(iType==GCP_MENU_MHORIZONTAL){
			if(currentSeparator < _iSeparators.size())
				if(i==_iSeparators.at(currentSeparator)){
					stackWidth += 5;currentSeparator++;
				}
			menu.at(i)->setPosition(_position.x() + stackWidth, _position.y());
			stackWidth += menu.at(i)->getPosition().width()+1;
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
		menu.at(i)->OnDraw( event);
		//if(!isContextMenuBlocking)
		//_isContextMenuOpened = (menu.at(i)->_isContextMenuOpened || _isContextMenuOpened);
	}

	//����������� ��������� �������� ������ ���������
	for(unsigned int i=0; i<iMenuSize; i++)
		menu.at(i)->OnDrawInfo( event);


	GCP_Draw::Render()->Draw_Round(_position.x(),  _position.y(),  stackWidth, stackHeight, getStyle()->roundCff, c_black);

}//OnDraw
