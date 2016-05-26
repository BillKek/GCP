
///* reverse:  �������������� ������ s �� ����� */
static void gcp_reverse(char s[])
{
	int i, j;
	char c;

	for (i = 0, j = strlen(s) - 1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

///* itoa:  ������������ n � ������� � s */
static void gcp_itoa(int n, char s[])
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

static std::string intToString(int value)
{
	char buff[256];
	//_itoa_s(value,buff,10) ;
	gcp_itoa(value, buff);
	return  string(buff);
}

static std::string doubleToString(double value)
{
	char buffer[32];
	sprintf(buffer, "%g", value);
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
static int stringToInt(char* str) { return atoi(str); }
static double stringToDouble(char* str) { return atof(str); }

static size_t utf8_length(unsigned char c)
{
	c = (unsigned char)(0xff & c);
	if (c < 0x80)
		return 1;
	else if ((c >> 5) == 0x6)
		return 2;
	else if ((c >> 4) == 0xe)
		return 3;
	else if ((c >> 3) == 0x1e)
		return 4;
	else
		return 0;
}
static char *utf8_next(char *p)
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
static char *utf8_advance(char *p, size_t distance)
{
	size_t i = 0;
	for (; i < distance && p; ++i)
	{
		p = utf8_next(p);
	}
	return p;
}
static std::string Font_StrToEng(const std::string &text)
{
	char *buffer = new char[text.size() + 2];
	Font_StrToEng(text, buffer, false);
	string str = buffer;
	delete[]buffer;
	buffer = NULL;
	return str;
}

static void Font_StrToEng(const std::string &sTextIn, char* sTextOut, bool special_symbols)
{
	string RusToEng = GCP_Math::DefineRusToEng();

	unsigned int i, j;
	for (i = 0, j = 0; i<sTextIn.size(); i++, j++)
	{
		if ((sTextIn[j] == '\\') && (sTextIn[j + 1] == 'n'))
		{
			sTextOut[i] = '\\';
			sTextOut[i + 1] = 'n';
			i++;
			j++;
		}
		else
		{
			//&quot;
			if ((sTextIn[j] == '&') && (sTextIn[j + 1] == 'q') && (sTextIn[j + 2] == 'u') && (sTextIn[j + 3] == 'o') && (sTextIn[j + 4] == 't') && (sTextIn[j + 5] == ';'))
			{
				sTextOut[i] = '&';
				sTextOut[i + 1] = 'q';
				sTextOut[i + 2] = 'u';
				sTextOut[i + 3] = 'o';
				sTextOut[i + 4] = 't';
				sTextOut[i + 5] = ';';
				i += 5; j += 5;
			}
			else
			{
				string chr = "&";// OR ELSE DEDUCE
				if ((special_symbols) && (RusToEng[300 + sTextIn[j]] == '&'))//'&'))
				{
					chr = RusToEng[300 + sTextIn[j]];
					sTextOut[i] = (char)chr[0];
					sTextOut[i + 1] = 'a';
					sTextOut[i + 2] = 'm';
					sTextOut[i + 3] = 'p';
					sTextOut[i + 4] = ';';
					i += 4;					 //&amp;
				}
				else
				{
					chr = RusToEng[300 + sTextIn[j]];
					sTextOut[i] = (char)chr[0];
				}
			}
		}
		if (sTextIn[j] == 0){ break; }
	}
	sTextOut[i] = '\0';
	//delete[] RusToEng;
	//RusToEng = NULL;
}

static std::string& DefineRusToEng()
{
	static string RusToEng;// = new string[1000];
	if (RusToEng.size() > 0)
		return RusToEng;

	RusToEng[300 + '�'] = 'A';	RusToEng[300 + '�'] = 'a';
	RusToEng[300 + '�'] = 'B';	RusToEng[300 + '�'] = 'b';
	RusToEng[300 + '�'] = 'C';	RusToEng[300 + '�'] = 'c';
	RusToEng[300 + '�'] = 'D';	RusToEng[300 + '�'] = 'd';
	RusToEng[300 + '�'] = 'E';	RusToEng[300 + '�'] = 'e';
	RusToEng[300 + '�'] = 'F';	RusToEng[300 + '�'] = 'f';
	RusToEng[300 + '�'] = 'G';	RusToEng[300 + '�'] = 'g';
	RusToEng[300 + '�'] = 'H';	RusToEng[300 + '�'] = 'h';
	RusToEng[300 + '�'] = 'I';	RusToEng[300 + '�'] = 'i';
	RusToEng[300 + '�'] = 'J';	RusToEng[300 + '�'] = 'j';
	RusToEng[300 + '�'] = 'K';	RusToEng[300 + '�'] = 'k';
	RusToEng[300 + '�'] = 'L';	RusToEng[300 + '�'] = 'l';
	RusToEng[300 + '�'] = 'M';	RusToEng[300 + '�'] = 'm';
	RusToEng[300 + '�'] = 'N';	RusToEng[300 + '�'] = '#';
	RusToEng[300 + '�'] = 'O';	RusToEng[300 + '�'] = 'o';
	RusToEng[300 + '�'] = 'P';	RusToEng[300 + '�'] = 'p';
	RusToEng[300 + '�'] = 'Q';	RusToEng[300 + '�'] = 'q';
	RusToEng[300 + '�'] = 'R';	RusToEng[300 + '�'] = 'r';
	RusToEng[300 + '�'] = 'S';	RusToEng[300 + '�'] = 's';
	RusToEng[300 + '�'] = 'T';	RusToEng[300 + '�'] = 't';
	RusToEng[300 + '�'] = 'U';	RusToEng[300 + '�'] = 'u';
	RusToEng[300 + '�'] = 'V';	RusToEng[300 + '�'] = 'v';
	RusToEng[300 + '�'] = 'W';	RusToEng[300 + '�'] = 'w';
	RusToEng[300 + '�'] = 'X';	RusToEng[300 + '�'] = 'x';
	RusToEng[300 + '�'] = 'Y';	RusToEng[300 + '�'] = 'y';
	RusToEng[300 + '�'] = '�';	RusToEng[300 + '�'] = 'z';

	RusToEng[300 + '�'] = '�';	RusToEng[300 + '�'] = '&';
	RusToEng[300 + '�'] = '�';	RusToEng[300 + '�'] = '~';
	RusToEng[300 + '�'] = '�';	RusToEng[300 + '�'] = '}';
	RusToEng[300 + '�'] = '^';	RusToEng[300 + '�'] = 'Z';
	RusToEng[300 + '�'] = '+';	RusToEng[300 + '�'] = '_';
	RusToEng[300 + '�'] = 'n';	RusToEng[300 + '�'] = '|';
	RusToEng[300 + '�'] = '{';	RusToEng[300 + '�'] = '`';

	RusToEng[300 + '0'] = '0';
	RusToEng[300 + '1'] = '1';
	RusToEng[300 + '2'] = '2';
	RusToEng[300 + '3'] = '3';
	RusToEng[300 + '4'] = '4';
	RusToEng[300 + '5'] = '5';
	RusToEng[300 + '6'] = '6';
	RusToEng[300 + '7'] = '7';
	RusToEng[300 + '8'] = '8';
	RusToEng[300 + '9'] = '9';
	RusToEng[300 + '!'] = '!';
	RusToEng[300 + '?'] = '?';
	RusToEng[300 + ','] = ',';
	RusToEng[300 + '.'] = '.';
	RusToEng[300 + '('] = '(';
	RusToEng[300 + ')'] = ')';
	RusToEng[300 + '$'] = '$';
	RusToEng[300 + ':'] = ':';
	RusToEng[300 + ';'] = ';';
	RusToEng[300 + '-'] = '-';
	RusToEng[300 + ' '] = ' ';
	RusToEng[300 + '"'] = '"';
	RusToEng[300 + '*'] = '*';
	RusToEng[300 + '\''] = '\'';
	RusToEng[300 + '/'] = '/';
	RusToEng[300 + '\\'] = '\\';
	return RusToEng;
}

static std::string RUSCP1251_TO_WCHAR(const wchar_t* str)
{
	/*�����Ũ��������������������������*/
	/*"��������������������������������1234567890"*/
	/*"!@#$%^&*()_+{}[]\":';\\.�?"*/

	wstring s = str;
	int value;
	char buffer[10000];
	for (int i = 0; i<10000; i++)
		buffer[i] = '1';
	string out = buffer;


	int k = 0;
	int shift = 1152;
	char a = -48;
	for (unsigned int i = 0; i< s.size(); i++)
	{
		value = str[i];

		if ((value>1103 && value != 1105 && value != 8470) || value < 1040){
			out[k] = value; k++;
		}
		else
		{
			a = -48;
			shift = 1152;
			char b = value - shift;
			if (value >= 1088){
				a = -47;
				shift = 1216;
				if (value == 1105) //"�"
					b = -111;
				b = value - shift;
			}

			if (value == 8470)//"�"
			{
				a = -30;
				b = -124;
				char c = -106;
				out[k] = a; k++;
				out[k] = b; k++;
				out[k] = c; k++;
			}
			else {
				out[k] = a; k++;
				out[k] = b; k++;
			}
		}
	}

	out[k] = '\0';
	return out;
}

static std::string RUSCP1251_TO_WCHAR(const char* str)
{
	/*�����Ũ��������������������������*/
	/*"��������������������������������1234567890"*/
	/*"`~!@#$%^&*()_+{}[]\":';\\.�?"*/

	string s = str;
	int value;
	char *buffer;
	buffer = new char[s.size() * 3 + 1];

	string out;


	int k = 0;
	int shift = 1152;
	char a = -48;
	for (unsigned int i = 0; i< s.size(); i++)
	{
		value = str[i];

		if (value >= 0){
			buffer[k] = value; k++;
		}
		else
		{
			a = -48;
			shift = -a;
			if (value >= -16)
			{
				a = -47;
				shift = 112;
			}
			char b = value - shift;
			if (value < -64 && value != -88){
				a = -47;
				shift = -a;


				b = value - shift;
				if (value == -72) //"�"
					b = -111;

			}



			if (value == -88) //"�"
				b = -127;



			if (value == -71)//"�"
			{
				a = -30;
				b = -124;
				char c = -106;
				buffer[k] = a; k++;
				buffer[k] = b; k++;
				buffer[k] = c; k++;
			}
			else {
				buffer[k] = a; k++;
				buffer[k] = b; k++;
			}
		}
	}

	buffer[k] = '\0';
	out = buffer;

	delete[]buffer;
	//out[k] = '\0';
	return out;
}

static bool isCharInStr(std::string const& str, const char* ch)
{
	const char * chars = str.c_str();
	for (size_t i = 0; i < str.size(); i++)
	{
		if (chars[i] == *ch)
			return true;
	}
	return false;
}

static bool isOneByteChar(const char s)
{
	static std::string engAlphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static std::string symAlphabet = ".+_-/*!@#$%^&()�;:?{}[]|'\\~\"0123456789";

	return (isCharInStr(engAlphabet, &s) || isCharInStr(symAlphabet, &s));
}

static bool isNum(char *s)
{
	int i = 0, flag = 1;
	static std::string engAlphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static std::string rusAlphabet = "�������������������������������������Ũ��������������������������";
	static std::string symAlphabet = "+_-/*!@#$%^&()�;:?{}[]|'\\~\"";

	while (s[i])
	{
		//if there is a letter in a string then string is not a number
		//if (isalpha(s[i]))//Does not work on russina characters

		if (strstr(engAlphabet.c_str(), s) != NULL || strstr(rusAlphabet.c_str(), s) != NULL
			|| strstr(symAlphabet.c_str(), s) != NULL
			)
		{
			flag = 0;
			break;
		}
		/*else
		{
		flag = 1;
		if (strstr("+_-/*!@#$%^&()�;:?{}[]|'\\~\"", s) != NULL)
		flag = 0;

		}*/

		i++;
	}
	if (flag == 1) return true;
	else return false;
}

/**
* C++ version 0.4 char* style "itoa":
* Written by Luk?s Chmela
* Released under GPLv3.
*/
char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
	} while (value);

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

static GCP_Vector<std::string> *strToLines(const std::string &Text)
{
	char str;
	unsigned int i = 0;
	std::string line;
	GCP_Vector<std::string> *strings = new GCP_Vector<std::string>();
	while (i < Text.size())
	{
		line = "";
		str = Text.at(i);
		while (str != '\n')
		{
			line += str;
			i++;
			if (i >= Text.size())
				break;
			str = Text.at(i);
		}
		strings->push_back(line);
		i++;
	}
	return strings;
}

template <class T>
static int readIntegerArray(char* strline, char separator, GCP_Vector<T>& _arr, bool _append = false, int _defaultLineSize = 50)
{
	char* xx = new char[_defaultLineSize];
	if (!_append)
		_arr.clear();
	int i = 0, j = 0;
	bool isExpectE = false;
	int iPointJ = 0;
	bool isMinusBeforeDigit = false;

	while (strline[i] != '\0')
	{
		while (strline[i] != separator && strline[i] != '\0')
		{
			if ((strline[i] >= '0' && strline[i] <= '9') || strline[i] == '-')
			{
				if (strline[i] == '-')
					isMinusBeforeDigit = true;
				else
					isMinusBeforeDigit = false;
				xx[j] = strline[i];
				j++;
			}
			else
			{
				//������ ���� � ������� ��� �� �����. �������.
				if (isMinusBeforeDigit == true)
				{
					isMinusBeforeDigit = false;
					xx[j - 1] = 0;
					j--;
				}
			}
			i++;

			//��������� �����
			if (strline[i] == '.')
			{
				i++;					//�����������, �� ��������� ������ ���� 1.24317e+006
				isExpectE = true;
				iPointJ = j;
			}

			//��������� �
			if (strline[i] == 'e')
			{
				i++; //+
				i++; //firestDigit after e+
				char eNum[10];
				int i2 = 0;
				while (strline[i] >= '0' && strline[i] <= '9')
				{
					eNum[i2] = strline[i];
					i++; i2++;
				}

				eNum[i2] = '\0';
				int eParam = GCP_Math::stringToInt(eNum);

				//������� � ����� ����������� ���������� ������ ����� �����, ������� ��� e
				int eXtend = eParam - (j - iPointJ);
				for (int j2 = 0; j2<eXtend; j2++)
				{
					xx[j] = '0'; j++;
				}
				isExpectE = false;
			}
		}

		//�� ������ ���� �� ��������� �����, �� �� ��������� ����� e
		bool isDouble = false;
		if (isExpectE == true)
		{
			isDouble = true;
			char moveChar = xx[iPointJ];
			xx[iPointJ] = '.';
			for (int i = iPointJ + 1; i <= j + 1; i++)
			{
				char tmp = xx[i];
				xx[i] = moveChar;
				moveChar = tmp;
			}
			j++;
		}


		xx[j] = '\0';
		j = 0;
		i++;
		isExpectE = false;
		iPointJ = 0;
		if (xx[0] != 0)
			if (isDouble)
				_arr.push_back(GCP_Math::stringToDouble(xx));
			else
				_arr.push_back(GCP_Math::stringToInt(xx));
	}

	delete xx;
	return _arr.size();
}

static int findStringInVector(const std::string &sTextIn, GCP_Vector<std::string> *vector)
{
	for (unsigned int i = 0; i<vector->size(); i++)
	{
		if (vector->at(i) == sTextIn)
			return i;
	}
	return -1;
}