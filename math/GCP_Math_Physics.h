struct reflectinfo
{
	double reflectedAngleOX;
	double reflectedAngle;
	double refractedAngleOX;
	double refractedAngle;
	bool refractionError;
	double N1, N2;
};

//����������� � ��������� line1 �� line2
template<class T>
static void lineRefractReflect(GCP_Line<T> const& line1, GCP_Line<T> const& line2, reflectinfo& info)
{
	///���������
	double alpha = lineAngle(line2);	// ���� line2 ������������ OX
	double Nalpha = alpha + 90;			// ���� ������� � line2 OX

	// ���� line1 ������������ �X
	double alpha2 = pointDirection(0, 0, line1.pointB.X - line1.pointA.X, line1.pointB.Y - line1.pointA.Y);

	// ���� ��������� line1 �� line2
	info.reflectedAngleOX = alpha2 - 2 * (alpha2 - Nalpha);

	//���� ������� �� line2 ������������ �������������� � ���
	info.reflectedAngle = std::abs(norm_dir_dif(alpha2, Nalpha));
	if (info.reflectedAngle > 90) info.reflectedAngle = 180 - info.reflectedAngle;

	///�����������
	if (alpha < 90)						// ����� ����� ������� ������
		alpha += 180;
	Nalpha = alpha - 90;				// ���� ������� � line2
	double alpha3 = norm_dir_dif(alpha2, Nalpha);	// ���� ������� ���� ������������ �����

	double x = (info.N1 * ssin(alpha3)) / info.N2;
	if (x < -1 || x > 1)
	{
		info.refractedAngle = 0;
		info.refractedAngleOX = 0;
		info.refractionError = true;
		return;
	}

	// ���� �����������
	double refractedAngle = GCP_RADTODEG*asin(x);
	bool isBelowLine = isPointBelowLine(line1.pointA, line2);
	if (isBelowLine)
		info.refractedAngleOX = Nalpha + 180 - refractedAngle;
	else info.refractedAngleOX = Nalpha + refractedAngle;

	info.refractedAngleOX = normAngle(info.refractedAngleOX);
	info.refractedAngle = std::abs(refractedAngle);
	info.refractionError = false;
}

//���� ����������� ray(p11,p12) ������ line(p21,p22) �� N1 � N2
//refractedAngleOX - ���� ������������ OX
//refractedAngle   - ���� ������������ line
//error = 1        - ����������� ����������
template<class T>
static void lineRefractionAngle(GCP_Point<T> const& p11, GCP_Point<T> const& p12, GCP_Point<T> const& p21, GCP_Point<T> const& p22, double N1, double N2, double *refractedAngleOX, double *refractedAngle, int *error)
{
	double alpha = lineAngle(p21, p22);								 // ���� ������ ������������ �X
	if (alpha < 90) alpha += 180;									 // ����� ����� ������� ������
	double Nalpha = alpha - 90;                                      // ���� ������� � ���� ������
	double alpha2 = pointDirection(0, 0, p12.X - p11.X, p12.Y - p11.Y);   // ���� ��������� ���� ������������ �X
	double alpha3 = norm_dir_dif(alpha2, Nalpha);					 // ���� ������� ���� ������������ �����

	double x = (N1 * ssin(alpha3)) / N2;
	if (x < -1 || x > 1)	{		//wrong asin
		*refractedAngleOX = 0;
		*refractedAngle = 0;
		*error = -1;
		return;
	}

	double reflectedAngle = GCP_RADTODEG*asin(x);	 // ���� �����������
	bool isBelowLine = isPointBelowLine(p11, GCP_Line<T>(p21, p22));
	if (isBelowLine)
		*refractedAngleOX = Nalpha + 180 - reflectedAngle;
	else *refractedAngleOX = Nalpha + reflectedAngle;

	*refractedAngle = std::abs(reflectedAngle);
	*error = 1;
}

template<class T>
static void lineRefractionAngle(const GCP_Line<T> &L1, const GCP_Line<T> &L2, double N1, double N2, double *refractedAngleOX, double *refractedAngle, int *error)
{
	lineRefractionAngle(L1.pointA, L1.pointB, L2.pointA, L2.pointB, N1, N2, refractedAngleOX, refractedAngle, error);
}


//���� ��������� ray(p11, p12) �� line (p21, p22)
//reflectedAngle - ������������ line
template<class T>
static void lineReflectionAngle(GCP_Point<T> const& p11, GCP_Point<T> const& p12, GCP_Point<T> const& p21, GCP_Point<T> const& p22, double *reflectedAngleOX, double *reflectedAngle)
{
	double alpha = lineAngle(p21, p22);
	double Nalpha = alpha + 90;										// ���� ������� � ���� ������ OX
	double alpha2 = pointDirection(0, 0, p12.X - p11.X, p12.Y - p11.Y);  // ���� ��������� ���� ������������ �X
	*reflectedAngleOX = alpha2 - 2 * (alpha2 - Nalpha);				// ���� ��������� ��������� ���� �� ������

	//���� ������� �� ������ ������������ �������������� � ���
	*reflectedAngle = std::abs(norm_dir_dif(alpha2, Nalpha));
	if (*reflectedAngle > 90) *reflectedAngle = 180 - *reflectedAngle;
}
template<class T>
static void lineReflectionAngle(const GCP_Line<T> &A, const GCP_Line<T> &B, double *d, double *d2)
{
	lineReflectionAngle(A.pointA, A.pointB, B.pointA, B.pointB, d, d2);
}
template<class T>
static double lineReflectionAngle(const GCP_Line<T> &A, const GCP_Line<T> &B)
{
	double d, d2;
	lineReflectionAngle(A.pointA, A.pointB, B.pointA, B.pointB, &d, &d2);
	return d;
}