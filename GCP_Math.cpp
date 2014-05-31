#include <cstring>
#include <sstream>
#include <cmath>
#include <cstdio>
#include <string>
#include "GCP_Vector.h"
#include "GCP_Math.h"


/////////////////////////////////////////////////////////////////////////
		 bool GCP_Math::isPointBelowLine(GCP_Point P, GCP_Line L)
		{
			// ���������� ��������� �� ����� ����� �� ����� ��� ������
			bool isBelow = ((long double)(L.pointB.X - L.pointA.X)*(P.Y - L.pointA.Y) - (long double)(L.pointB.Y - L.pointA.Y)*(P.X - L.pointA.X) > 0);
			if (lineAngle(L)>90) return  isBelow;
			return !isBelow;
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////����������
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
////////////////////Is In Radius
/////////////////////////////////////////////////////////////////////////
		 bool GCP_Math::isInRadius(double x1,double y1, double x2, double y2, double r)		{
			//���������� �� ����� (x1,y1) �� ����� (x2,y2)
			return (sqrt(pow(x2-x1,2)+pow(y2-y1,2)) < r);
		}
/////////////////////////////////////////////////////////////////////////
////////////////////Is In Rect
/////////////////////////////////////////////////////////////////////////
		 bool GCP_Math::isInRect(double pointX, double pointY, double rectTopx, double rectTopy, double rectWidth, double rectHeight)		{
			if(GCP_Math::compareDouble(pointX, rectTopx) && GCP_Math::compareDouble( rectTopx+rectWidth, pointX) && GCP_Math::compareDouble(pointY, rectTopy) && GCP_Math::compareDouble(rectTopy + rectHeight,pointY))
				return true;
			return false;
		}
		 bool GCP_Math::isPointInRect(GCP_Point TopLeft, GCP_Point BottomRight, GCP_Point point)
		{
			return isInRect(point.X,point.Y,TopLeft.X,TopLeft.Y,BottomRight.X-TopLeft.X,BottomRight.Y-TopLeft.Y);
		}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 double GCP_Math::lineAngle(GCP_Point p11, GCP_Point p12)
		{
			double N1 = (double)(p12.X - p11.X);										//����������� ����� ������������ �������
			double N2 = (double)(p12.Y - p11.Y);
			return pointDirection(0,0,N1,N2);
		}
/////////////////////////////////////////////////////////////////////////
////////////////////Normalze Rect Inside Rect
/////////////////////////////////////////////////////////////////////////
		 GCP_Point GCP_Math::normalizeRectInRect(GCP_Point TopLeft, GCP_Point BottomRight, double x, double y, int width, int height, int offset){
			GCP_Point drawPoint = GCP_Point(x,y);
			if(drawPoint.X+width+offset>BottomRight.X)
				drawPoint.X -= drawPoint.X+width+offset - BottomRight.X;
			if(drawPoint.Y+height+offset>BottomRight.Y)
				drawPoint.Y -= drawPoint.Y+height+offset - BottomRight.Y;
			if(drawPoint.X	<	TopLeft.X+offset)
				drawPoint.X = TopLeft.X+offset;
			if(drawPoint.Y	<	TopLeft.Y+offset)
				drawPoint.Y = TopLeft.Y+offset;
			return drawPoint;
		}
/////////////////////////////////////////////////////////////////////////
////////////////////Normalze Point Inside Rect
////////////////////////////////////////////////////////////////////////////////////////////
		 GCP_Point GCP_Math::normalizePointInRect(GCP_Point point, GCP_Point pmin, GCP_Point pmax)
		{
			GCP_Point normPoint;
			normPoint.X = max(min(point.X,(double)pmax.X),(double)pmin.X);
			normPoint.Y = max(min(point.Y,(double)pmax.Y),(double)pmin.Y);
			return normPoint;
		}
/////////////////////////////////////////////////////////////////////////
////////////////////Round Double Value
/////////////////////////////////////////////////////////////////////////
		 double GCP_Math::round(double value)
		{
			if(GCP_Math::compareDouble(value-floor(value),0.5))
				return floor(value);
			return floor(value)+1;
		}
/////////////////////////////////////////////////////////////////////////
////////////////////LineIntersection
/////////////////////////////////////////////////////////////////////////
		 bool GCP_Math::lineIntersection(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double *Px, double *Py, bool round)
		{
			//����������� ���� ����� L1(x1,y1,x2,y2) L2(x3,y3,x4,y4) � ����� (Px,Py)
			long double fDevide = (long double)((x1-x2)*(y3-y4)) - (long double)((y1-y2)*(x3-x4));
			if (fDevide != 0)
			{
				long double statmentx = (x3-x4)*((long double)x1*y2 - (long double)y1*x2)
									   -(x1-x2)*((long double)x3*y4 - (long double)y3*x4);
				*Px =  statmentx / fDevide;
				long double statmenty = (y3-y4)*((long double)x1*y2 - (long double)y1*x2)
									   -(y1-y2)*((long double)x3*y4 - (long double)y3*x4);
				*Py = statmenty / fDevide;

				if(round)		{
					*Px = floor(*Px);
					*Py = floor(*Py);
				}
				return true;
			}
			else {
				*Px = 0; *Py = 0;
				return false;
			}
		}
		 bool GCP_Math::lineIntersection(GCP_Point p11, GCP_Point p12, GCP_Point p21, GCP_Point p22, GCP_Point *R, bool round)
		{
			double xx, yy;
			bool collision = lineIntersection(p11.X, p11.Y,p12.X,p12.Y,p21.X,p21.Y,p22.X,p22.Y,&xx,&yy,round);
			*R = GCP_Point(xx,yy);
			return collision;
		}
		 bool GCP_Math::lineIntersection(GCP_Line A, GCP_Line B, GCP_Point *R,bool round)
		{
			return lineIntersection(A.pointA,A.pointB,B.pointA,B.pointB, R,round);
		}
/////////////////////////////////////////////////////////////////////////
////////////////////Round Double Value
/////////////////////////////////////////////////////////////////////////
		 bool GCP_Math::lineRectIntersection(GCP_Point TopLeft, GCP_Point TopRight, GCP_Point BottomLeft, GCP_Point BottomRight, GCP_Line L, GCP_Vector<GCP_Point> *R)
		{
			//����� ���� ����������� ����� ������� �������������� � ������ L (*������ ������, �� �����)
			GCP_Point intersection;
			bool isIntersect;
			L = GCP_Line(GCP_Point(round(L.pointA.X),round(L.pointA.Y)),
						 GCP_Point(round(L.pointB.X),round(L.pointB.Y)));

			isIntersect = lineIntersection(GCP_Line(TopLeft,TopRight),L,&intersection,true);
			if (isIntersect)	{
				//�������� ��� ����������� ������������� �� �����, � �� ���-�� �� ����
				if(isPointInLine(GCP_Line(TopLeft,TopRight),intersection)&&isPointInLine(L,intersection))
					R->push_back(intersection);
			}

			isIntersect = GCP_Math::lineIntersection(GCP_Line(TopLeft,BottomLeft),L,&intersection,true);
			if (isIntersect)	{
				if(isPointInLine(GCP_Line(TopLeft,BottomLeft),intersection)&&isPointInLine(L,intersection))
					R->push_back(intersection);
			}

			isIntersect = GCP_Math::lineIntersection(GCP_Line(BottomLeft,BottomRight),L,&intersection,true);
			if (isIntersect)	{
				if(isPointInLine(GCP_Line(BottomLeft,BottomRight),intersection)&&isPointInLine(L,intersection))
					R->push_back(intersection);
			}

			isIntersect = GCP_Math::lineIntersection(GCP_Line(TopRight,BottomRight),L,&intersection,true);
			if (isIntersect)	{
				if(isPointInLine(GCP_Line(TopRight,BottomRight),intersection)&&isPointInLine(L,intersection))
					R->push_back(intersection);
			}

			return (R->size()>0);
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 bool GCP_Math::compareDouble(double x, double y)		{
			//x > y ?
			double eps = 1e-5;
			if( x-y > -eps)
				return true;
			return false;
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 double GCP_Math::rnd()
		{
			//return (double)rand(RAND_MAX)/RAND_MAX;   //��������� ����� �� 0 �� 1 ���������� ��������������
			return (double)rand()/RAND_MAX;
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 double GCP_Math::rnd(const char *dist, double a, double b)
		{
			if (strcmp(dist,"norm")==0)
			{
				//������������ ����������� ������������� - ���� �� �� ������ � �� a � ���� 1.
				while(1)
				{
                    double v1,v2,s;
					v1=(double)(2.*rnd())-1.;
					v2=(double)(2.*rnd())-1.;
					s=v1*v1+v2*v2;
					if((s<=1.)&&(s>0.))
					{
                      //double r;
					  //r=v1*sqrt((-2.)*log(s)/s); //�� ���� ��� �������� 2 ��
					  return a+(v2*sqrt((-2.)*log(s)/s));
					}
				}
			}
			return -1;
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 double GCP_Math::pointDirection(double x1, double y1, double x2, double y2)
		{
			//����������� �� ����� (x1,y1) � ����� (x2,y2)
			double d;
			double yd = y1 - y2;
			double xd = x2 - x1;
			double atan = atan2(yd, xd);
			d = (double)(GCP_RADTODEG*atan);			//� ��������
			return normAngle(d);
		}
		double GCP_Math::pointDirection(GCP_Point A, GCP_Point B)
		{
			return pointDirection(A.X,A.Y,B.X,B.Y);
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 void GCP_Math::lineRefractionAngle(GCP_Point p11, GCP_Point p12, GCP_Point p21, GCP_Point p22, double N1, double N2, double *refractedAngleOX, double *refractedAngle, int *error)
		{
			double alpha = lineAngle(p21,p22);								 // ���� ������ ������������ �X
			if (alpha < 90) alpha += 180;									 // ����� ����� ������� ������
			double Nalpha = alpha - 90;                                      // ���� ������� � ���� ������
			double alpha2 = pointDirection(0,0,p12.X-p11.X,p12.Y - p11.Y);   // ���� ��������� ���� ������������ �X
			double alpha3 = norm_dir_dif(alpha2,Nalpha);					 // ���� ������� ���� ������������ �����


			double x = (N1 * ssin(alpha3))/N2;
			if(x < -1 || x > 1)	{		//wrong asin
				*refractedAngleOX = 0;
				*refractedAngle = 0;
				*error =-1;
				return;
			}

			double reflectedAngle = GCP_RADTODEG*asin(x);	 // ���� �����������
			bool isBelowLine = isPointBelowLine(p11,GCP_Line(p21,p22));
			if(isBelowLine)
				*refractedAngleOX = Nalpha+180-reflectedAngle;
			else *refractedAngleOX = Nalpha+reflectedAngle;

			*refractedAngle = abs(reflectedAngle);
			*error = 1;
		}
		 void GCP_Math::lineReflectionAngle(GCP_Point p11, GCP_Point p12, GCP_Point p21, GCP_Point p22, double *reflectedAngleOX, double *reflectedAngle)
		{
			double alpha = lineAngle(p21,p22);
			double Nalpha = alpha + 90;										// ���� ������� � ���� ������ OX
			double alpha2 = pointDirection(0,0,p12.X-p11.X,p12.Y - p11.Y);  // ���� ��������� ���� ������������ �X
			*reflectedAngleOX = alpha2 - 2*(alpha2 - Nalpha);				// ���� ��������� ��������� ���� �� ������

			//���� ������� �� ������ ������������ �������������� � ���
			*reflectedAngle = abs(norm_dir_dif(alpha2,Nalpha));
			if(*reflectedAngle > 90) *reflectedAngle = 180 - *reflectedAngle;
		}
		 void GCP_Math::lineReflectionAngle(GCP_Line A, GCP_Line B, double *d, double *d2)
		{
			lineReflectionAngle(A.pointA, A.pointB, B.pointA, B.pointB, d, d2);
		}
		 double GCP_Math::lineReflectionAngle(GCP_Line A, GCP_Line B)
		{
			double d,d2;
			lineReflectionAngle(A.pointA, A.pointB, B.pointA, B.pointB, &d, &d2);
			return d;
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 double GCP_Math::pointDistance(double x1, double y1, double x2, double y2)
		{
			return sqrt(pow(x2-x1,2)+pow(y2-y1,2));	//���������� �� ����� (x1,y1) �� ����� (x2,y2)
		}
		 double GCP_Math::pointDistance(GCP_Point X, GCP_Point Y)
		{
			return pointDistance(X.X,X.Y,Y.X,Y.Y);
		}
		////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 double GCP_Math::lineAngle(GCP_Line L)
		{
			return lineAngle(L.pointA,L.pointB);
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 void GCP_Math::lineRefractionAngle(GCP_Line L1, GCP_Line L2, double N1, double N2, double *refractedAngleOX, double *refractedAngle, int *error)
		{
			lineRefractionAngle(L1.pointA, L1.pointB, L2.pointA, L2.pointB, N1, N2, refractedAngleOX, refractedAngle, error);
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
		// template< class A>
		// void GCP_Math::quickSort(GCP_Vector<A> *a, int l, int r, int criteria)
		//{
		//	A x = a->at(l + (r - l) / 2);
		//	//������ ������������ (l+r)/2,
		//	//�� �� �������� ������������ �� ������� ������
		//	int i = l;
		//	int j = r;
		//	//��� � while ������ ������� � ��������� particle
		//	while(i <= j)
		//	{
		//		while((a->at(i))->compare(x,1,criteria)) i++;
		//		while((a->at(j))->compare(x,2,criteria)) j--;
		//		if(i <= j)
		//		{
		//			a->swap(i,j);
		//			i++;
		//			j--;
		//		}
		//	}
		//	if (i<r)
		//				quickSort<A>(a,i, r, criteria);
		//
		//	if (l<j)
		//		quickSort<A>(a,l, j, criteria);
		//}
		//
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 double GCP_Math::ccos(double value)
		{
			//GCP_FastTG.InitFSinCosTable();
			//return GCP_FastTG.fcosined(value);
			return cos(GCP_DEGTORAD*value);						//������� �� �������� � ��������
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 double GCP_Math::ssin(double value)
		{
			//GCP_FastTG.InitFSinCosTable();
			//return GCP_FastTG.fsined(value);
			return sin((double)(GCP_DEGTORAD*value));			//����� �� �������� � ��������
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 double GCP_Math::arcsin(double value)
		{
			return asin(value);
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 bool GCP_Math::isPointInLine(GCP_Line line, GCP_Point checkPoint)
		{
			GCP_Point TopLeft(min(line.pointA.X,line.pointB.X),min(line.pointA.Y,line.pointB.Y));
			GCP_Point BottomRight(max(line.pointA.X,line.pointB.X),max(line.pointA.Y,line.pointB.Y));
			return isPointInRect(TopLeft,BottomRight,checkPoint);
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 bool GCP_Math::isPointNear(GCP_Point pointA, GCP_Point pointB, int dist)
		{
			return isPointInRect(GCP_Point(pointB.X-dist/2,pointB.Y-dist/2),GCP_Point(pointB.X+dist/2,pointB.Y+dist/2),pointA);
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 double GCP_Math::normAngle(double value)
		{
			value = fmod(value,360);		//���������� ���� � ������� [0,360)
			//try{
				if (value<0) value+=360;
		//	}
			/*catch(System::ArithmeticException ^e)
			{
				value = 0;
			}*/
			return value;
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 string GCP_Math::normAngleS(double value)
		{
			int degree = (int)floor(value);
			double tempvalue = 60*(value-degree);
			int minutes = (int)floor(tempvalue);
			int seconds = (int)floor(60 * (tempvalue-minutes));

			return intToString(degree)+"'"+intToString(minutes)+"'"+intToString(seconds);
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 double GCP_Math::norm_dir_dif(double dir1, double dir2)
		{
			double dif  = (double)dir1-dir2;											//���������� ���������� �� ����������� dir1 � ����������� dir2
			if (dif > 180) {return(dif - 360);}											//������: �� ����������� 90 � ������������ 300/-60 =  -150
			if (dif < -180) {return(dif + 360);}										//������: �� ����������� 90 � ������������ 240/-120 =  150
			return dif; //if (dif<=180 && dif>=-180) {return dif;}
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 /* reverse:  �������������� ������ s �� ����� */
 void gcp_reverse(char s[])
 {
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }
  /* itoa:  ������������ n � ������� � s */
 void gcp_itoa(int n, char s[])
 {
     int i, sign;

     if ((sign = n) < 0)  /* ���������� ���� */
         n = -n;          /* ������ n ������������� ������ */
     i = 0;
     do {       /* ���������� ����� � �������� ������� */
         s[i++] = n % 10 + '0';   /* ����� ��������� ����� */
     } while ((n /= 10) > 0);     /* ������� */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     gcp_reverse(s);
 }


		 string GCP_Math::intToString(int value)
		{
			 char buff[256];
			 //_itoa_s(value,buff,10) ;
			 gcp_itoa(value,buff);
			 return  string(buff);
		}
		 string GCP_Math::doubleToString(double value)
		{
			char buffer[32];
            sprintf (buffer, "%g", value);
            //gcp_itoa(value,buffer);
			/*#ifdef __WIN32__
				sprintf_s(buffer,32, "%g", value);
			#else
				//snprintf(buffer, sizeof(buffer), "%.10g", value);
				//sprintf_s(buffer,32, "%g", value);
				sprintf (buffer, "%g", 8, value);
				gcp_itoa(value,buffer);
			#endif*/

			string str(buffer);
			return str;
		}
		int GCP_Math::stringToInt(char* str)
		{
			return atoi(str);
		}
		double GCP_Math::stringToDouble(char *str)
		{
			return atof(str);
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		int GCP_Math::findStringInVector(string sTextIn, GCP_Vector<string> *vector)
		{
			for(unsigned int i=0; i<vector->size(); i++)
			{
				if(vector->at(i) == sTextIn)
					return i;
			}
			return -1;
		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		int GCP_Math::readIntegerArray(char* strline, char separator, int* arr)
		{
			//GCP_Vector<int>* result = new GCP_Vector<int>();
			char xx[50]; int i=0, j=0, k=0;
			bool isExpectE = false;
			int iPointJ = 0;

			while(strline[i] != '\0'){
				while(strline[i]!= separator ){
					if((strline[i]>='0' && strline[i]<='9')
                        || strline[i]=='-')
                    {
						xx[j] = strline[i];
						j++;
					}
					i++;

					//��������� �����
					if(strline[i]=='.')
					{
						i++;					//�����������, �� ��������� ������ ���� 1.24317e+006
						isExpectE = true;
						iPointJ = j;
					}

					//��������� �
					if(strline[i]=='e')
					{
						i++; //+
						i++; //firestDigit after e+
						char eNum[10];
						int i2=0;
						while(strline[i]>='0' && strline[i]<='9' )
						{
							eNum[i2]=strline[i];
							i++; i2++;
						}

						eNum[i2] = '\0';
						int eParam = GCP_Math::stringToInt(eNum);

						//������� � ����� ����������� ���������� ������ ����� �����, ������� ��� e
						int eXtend = eParam-(j-iPointJ);
						for(int j2=0; j2<eXtend; j2++)
						{
							xx[j]='0'; j++;
						}
						isExpectE = false;
					}
				}

				//�� ������ ���� �� ��������� �����, �� �� ��������� ����� e
				//�������� � ��������� �� ����� (��������� ������ ����� �����)
				if(isExpectE == true)
					j = iPointJ;

				xx[j] = '\0'; j=0; i++; isExpectE = false;	iPointJ = 0;
				if(xx[0] != 0)
					arr[k] = GCP_Math::stringToInt(xx);
				k++;
			}
			return k;

		}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
		 double GCP_Math::uniqueValue()
		{
			return rnd()*rnd();
		}
/////////////////////////////////////////////////////////////////////////
////////////////////Calculatuions To Draw Russian Text As English Font Using Special Font Type
/////////////////////////////////////////////////////////////////////////
		 size_t GCP_Math::utf8_length(unsigned char c)
		{
			c = (unsigned char)(0xff & c);
			if (c < 0x80)
				return 1;
			else if ((c >> 5) ==0x6)
				return 2;
			else if ((c >> 4) == 0xe)
				return 3;
			else if ((c >> 3) == 0x1e)
				return 4;
			else
				return 0;
		}

		 char *GCP_Math::utf8_next(char *p)
		{
			size_t len = utf8_length(*p);
			size_t i = 0;
			if (!len)
				return 0;

			for (; i < len; ++i)
			{
				++p;
				if (!*p)
					return 0;
			}
			return p;
		}
		 char *GCP_Math::utf8_advance(char *p, size_t distance)
		{
			size_t i = 0;
			for (; i < distance && p; ++i)
			{
				p = utf8_next(p);
			}
			return p;
		}
		string GCP_Math::Font_StrToEng(string text)
		{
			char *buffer = new char[text.size()+2];
			Font_StrToEng(text,buffer,false);
			string str = buffer;
			delete []buffer;
			buffer = NULL;
			return str;
		}
		 void GCP_Math::Font_StrToEng(string sTextIn, char* sTextOut,bool special_symbols)
		{
			string *RusToEng =	GCP_Math::DefineRusToEng();
			unsigned int i,j;
			for(i=0,j=0; i<sTextIn.size(); i++,j++)
			{
				if ((sTextIn[j]=='\\')&&(sTextIn[j+1]=='n'))
				{
					sTextOut[i] = '\\';
					sTextOut[i+1] = 'n';
					i++;
					j++;
				}
				else
				{
					//&quot;
					if ((sTextIn[j]=='&')&&(sTextIn[j+1]=='q')&&(sTextIn[j+2]=='u')&&(sTextIn[j+3]=='o')&&(sTextIn[j+4]=='t')&&(sTextIn[j+5]==';'))
					{
					sTextOut[i] = '&';
					sTextOut[i+1] = 'q';
					sTextOut[i+2] = 'u';
					sTextOut[i+3] = 'o';
					sTextOut[i+4] = 't';
					sTextOut[i+5] = ';';
					i+=5; j+=5;
					}
					else
					{
						string chr = "&";// OR ELSE DEDUCE
						if ((special_symbols)&&(RusToEng[300+sTextIn[j]]==chr))//'&'))
						{
							chr = RusToEng[300+sTextIn[j]];
							sTextOut[i] = (char) chr[0];
							sTextOut[i+1] = 'a';
							sTextOut[i+2] = 'm';
							sTextOut[i+3] = 'p';
							sTextOut[i+4] = ';';
							i+=4;					 //&amp;
						}
						else
						{
							chr = RusToEng[300+sTextIn[j]];
							sTextOut[i] = (char) chr[0];
						}
					}
				}
				   if (sTextIn[j] == 0){break;}
			}
			sTextOut[i]='\0';
			delete[] RusToEng;
			RusToEng = NULL;
		}



		string GCP_Math::RUSCP1251_TO_WCHAR(const wchar_t* str)
		{
			/*�����Ũ��������������������������*/
			/*"��������������������������������1234567890"*/
			/*"!@#$%^&*()_+{}[]\":';\\.�?"*/

			wstring s = str;
			int value;
			char buffer[10000];
			for(int i=0; i<10000; i++)
				buffer[i] = '1';
			string out=buffer;


			int k=0;
			int shift = 1152;
			char a = -48;
			for(unsigned int i=0; i< s.size(); i++)
			{
				value = str[i];

				if((value>1103 && value != 1105 && value != 8470) || value < 1040 ){
					out[k] = value; k++;
				}
				else
				{
					a = -48;
					shift = 1152;
					char b = value-shift;
					if(value >= 1088){
						a = -47;
						shift = 1216;
						if(value==1105) //"�"
							b = -111;
						b = value-shift;
					}

					if(value==8470)//"�"
					{
						a = -30;
						b = -124;
						char c = -106;
						out[k]= a; k++;
						out[k]= b; k++;
						out[k]= c; k++;
					}
					else {
					out[k]= a; k++;
					out[k]= b; k++;
					}
				}
			}

			out[k] = '\0';
			return out;
		}
		string GCP_Math::RUSCP1251_TO_WCHAR(const char* str)
		{
			/*�����Ũ��������������������������*/
			/*"��������������������������������1234567890"*/
			/*"`~!@#$%^&*()_+{}[]\":';\\.�?"*/

			string s = str;
			int value;
			char *buffer;
			buffer = new char[s.size()*3+1];

			string out;


			int k=0;
			int shift = 1152;
			char a = -48;
			for(unsigned int i=0; i< s.size(); i++)
			{
				value = str[i];

				if(value>=0){
					buffer[k] = value; k++;
				}
				else
				{
					a = -48;
					shift = -a;
					if(value >= -16)
					{
						a = -47;
						shift = 112;
					}
					char b = value-shift;
					if(value < -64 && value != -88){
						a = -47;
						shift = -a;


						b = value-shift;
						if(value == -72) //"�"
							b = -111;

					}



					if(value == -88) //"�"
						b = -127;



					if(value==-71)//"�"
					{
						a = -30;
						b = -124;
						char c = -106;
						buffer[k]= a; k++;
						buffer[k]= b; k++;
						buffer[k]= c; k++;
					}
					else {
					buffer[k]= a; k++;
					buffer[k]= b; k++;
					}
				}
			}

			buffer[k] = '\0';
			out = buffer;

			delete []buffer;
			//out[k] = '\0';
			return out;
		}
		string* GCP_Math::DefineRusToEng()
		{
			string *RusToEng = new string[1000];
			RusToEng[300+'�']= 'A';	RusToEng[300+'�']= 'a';
			RusToEng[300+'�']= 'B';	RusToEng[300+'�']= 'b';
			RusToEng[300+'�']= 'C';	RusToEng[300+'�']= 'c';
			RusToEng[300+'�']= 'D';	RusToEng[300+'�']= 'd';
			RusToEng[300+'�']= 'E';	RusToEng[300+'�']= 'e';
			RusToEng[300+'�']= 'F';	RusToEng[300+'�']= 'f';
			RusToEng[300+'�']= 'G';	RusToEng[300+'�']= 'g';
			RusToEng[300+'�']= 'H';	RusToEng[300+'�']= 'h';
			RusToEng[300+'�']= 'I';	RusToEng[300+'�']= 'i';
			RusToEng[300+'�']= 'J';	RusToEng[300+'�']= 'j';
			RusToEng[300+'�']= 'K';	RusToEng[300+'�']= 'k';
			RusToEng[300+'�']= 'L';	RusToEng[300+'�']= 'l';
			RusToEng[300+'�']= 'M';	RusToEng[300+'�']= 'm';
			RusToEng[300+'�']= 'N';	RusToEng[300+'�']= '#';
			RusToEng[300+'�']= 'O';	RusToEng[300+'�']= 'o';
			RusToEng[300+'�']= 'P';	RusToEng[300+'�']= 'p';
			RusToEng[300+'�']= 'Q';	RusToEng[300+'�']= 'q';
			RusToEng[300+'�']= 'R';	RusToEng[300+'�']= 'r';
			RusToEng[300+'�']= 'S';	RusToEng[300+'�']= 's';
			RusToEng[300+'�']= 'T';	RusToEng[300+'�']= 't';
			RusToEng[300+'�']= 'U';	RusToEng[300+'�']= 'u';
			RusToEng[300+'�']= 'V';	RusToEng[300+'�']= 'v';
			RusToEng[300+'�']= 'W';	RusToEng[300+'�']= 'w';
			RusToEng[300+'�']= 'X';	RusToEng[300+'�']= 'x';
			RusToEng[300+'�']= 'Y';	RusToEng[300+'�']= 'y';
			RusToEng[300+'�']= '�';	RusToEng[300+'�']= 'z';

			RusToEng[300+'�']= '�';	RusToEng[300+'�']= '&';
			RusToEng[300+'�']= '�';	RusToEng[300+'�']= '~';
			RusToEng[300+'�']= '�';	RusToEng[300+'�']= '}';
			RusToEng[300+'�']= '^';	RusToEng[300+'�']= 'Z';
			RusToEng[300+'�']= '+';	RusToEng[300+'�']= '_';
			RusToEng[300+'�']= 'n';	RusToEng[300+'�']= '|';
			RusToEng[300+'�']= '{';	RusToEng[300+'�']= '`';

			RusToEng[300+'0']= '0';
			RusToEng[300+'1']= '1';
			RusToEng[300+'2']= '2';
			RusToEng[300+'3']= '3';
			RusToEng[300+'4']= '4';
			RusToEng[300+'5']= '5';
			RusToEng[300+'6']= '6';
			RusToEng[300+'7']= '7';
			RusToEng[300+'8']= '8';
			RusToEng[300+'9']= '9';
			RusToEng[300+'!']= '!';
			RusToEng[300+'?']= '?';
			RusToEng[300+',']= ',';
			RusToEng[300+'.']= '.';
			RusToEng[300+'(']= '(';
			RusToEng[300+')']= ')';
			RusToEng[300+'$']= '$';
			RusToEng[300+':']= ':';
			RusToEng[300+';']= ';';
			RusToEng[300+'-']= '-';
			RusToEng[300+' ']= ' ';
			RusToEng[300+'"']= '"';
			RusToEng[300+'*']= '*';
			RusToEng[300+'\'']= '\'';
			RusToEng[300+'/']= '/';
			RusToEng[300+'\\']= '\\';
			return RusToEng;
		}


//////////////////////////////////////////////////////////////////////////////////////////////


















