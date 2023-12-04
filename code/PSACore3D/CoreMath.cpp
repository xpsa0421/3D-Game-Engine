#include "CoreMath.h"

//-------------------------------------------------
// member functions of Vector2
Vector2::Vector2()
{
	x = y = 0.0f;
}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2::Vector2(const Vector2& v)
{
	this->x = v.x;
	this->y = v.y;
}

float Vector2::GetLengthSquared()
{
	return (x * x + y * y);
}

float Vector2::GetLength()
{
	return sqrt(GetLengthSquared());
}

void Vector2::Normalise()
{
	float inverseLength = 1.0f / GetLength();
	x = x * inverseLength;
	y = y * inverseLength;
}

Vector2 Vector2::operator + (Vector2& v)
{
	return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator - (Vector2& v)
{
	return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator * (float scala)
{
	return Vector2(x * scala, y * scala);
}

Vector2 Vector2::operator / (float scala)
{
	return Vector2(x / scala, y / scala);
}

Vector2 Vector2::operator += (Vector2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2 Vector2::operator -= (Vector2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2 Vector2::operator *= (float scala)
{
	x *= scala;
	y *= scala;
	return *this;
}

bool Vector2::operator == (Vector2& v)
{
	if ((fabs(x - v.x) < EPSILON)
		&& (fabs(y - v.y) < EPSILON))
	{
		return true;
	}
	return false;
}

bool Vector2::operator <= (Vector2& v)
{
	if ((x <= v.x) && (y <= v.y)) return true;
	return false;
}

bool Vector2::operator >= (Vector2& v)
{
	if ((x >= v.x) && (y >= v.y)) return true;
	return false;
}


//-------------------------------------------------
// member functions of Vector
Vector::Vector()
{
	x = y = z = 0.0f;
}

Vector::Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector(const Vector& v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}

float Vector::GetLengthSquared()
{
	return (x * x + y * y + z * z);
}

float Vector::GetLength()
{
	return sqrt(GetLengthSquared());
}

float Vector::GetAngle(Vector& v)
{
	float dotProduct = *this | v;
	dotProduct = dotProduct / (GetLength() * v.GetLength());
	float radAngle = acos(dotProduct);
	float degAngle = RadianToDegree(radAngle);
	return degAngle;
}

void Vector::Normalise()
{
	float inverseLength = 1.0f / GetLength();
	x = x * inverseLength;
	y = y * inverseLength;
	z = z * inverseLength;
}

Vector Vector::GetNormal()
{
	Vector v = *this;
	float fInvertLength = 1.0f / GetLength();
	v.x *= fInvertLength;
	v.y *= fInvertLength;
	v.z *= fInvertLength;
	return v;
}

Vector Vector::cross(Vector& v)
{
	return *this ^ v;
}

float Vector::dot(Vector& v)
{
	return *this | v;
}

Vector Vector::XMVector3Normalise(Vector& v)
{
	Vector ret;
	DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&v);
	V = DirectX::XMVector3Normalize(V);
	DirectX::XMStoreFloat3(&ret, V);
	return ret;
}

Vector Vector::XMVector3TransformCoord(Vector& v, Matrix& m)
{
	DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&v);
	DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&m);
	DirectX::XMVECTOR RET = DirectX::XMVector3TransformCoord(V, M);

	Vector ret;
	DirectX::XMStoreFloat3(&ret, RET);
	return ret;
}

/**
 * Performs a linear interpolation between two vectors.
 *
 * @param	v1 the vector to interpolate from
 * @param	v2 the vector to interpolate to
 * @param	t the interpolation control factor
 * @return	the vector containing the interpolation
 */
Vector Vector::XMVectorLerp(Vector& v1, Vector& v2, float t)
{
	DirectX::XMVECTOR V1 = DirectX::XMLoadFloat3(&v1);
	DirectX::XMVECTOR V2 = DirectX::XMLoadFloat3(&v2);
	DirectX::XMVECTOR RET = DirectX::XMVectorLerp(V1, V2, t);
		
	Vector ret;
	DirectX::XMStoreFloat3(&ret, RET);
	return ret;
}

float Vector::operator [] (int i)
{
	if (i == 0) return x;
	if (i == 1) return y;
	if (i == 2) return z;
	return 0.0f;
}

Vector Vector::operator + (Vector& v)
{
	return Vector(x + v.x, y + v.y, z + v.z);
}

Vector Vector::operator - (Vector& v)
{
	return Vector(x - v.x, y - v.y, z - v.z);
}

Vector Vector::operator * (float scala)
{
	return Vector(x * scala, y * scala, z * scala);
}

Vector Vector::operator / (float scala)
{
	return Vector(x / scala, y / scala, z / scala);
}

Vector Vector::operator += (Vector v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector Vector::operator -= (Vector& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector Vector::operator *= (float scala)
{
	x *= scala;
	y *= scala;
	z *= scala;
	return *this;
}

bool Vector::operator == (Vector& v)
{
	if ((fabs(x - v.x) < EPSILON)
		&& (fabs(y - v.y) < EPSILON)
		&& (fabs(z - v.z) < EPSILON))
	{
		return true;
	}
	return false;
}

bool Vector::operator <= (Vector& v)
{
	if ((x <= v.x) && (y <= v.y) && (z <= v.z)) return true;
	return false;
}

bool Vector::operator >= (Vector& v)
{
	if ((x >= v.x) && (y >= v.y) && (z >= v.z)) return true;
	return false;
}

Vector Vector::operator * (Matrix& m)
{
	Vector v;
	v.x = x * m._11 + y * m._21 + z * m._31 + 1.0f * m._41;
	v.y = x * m._12 + y * m._22 + z * m._32 + 1.0f * m._42;
	v.z = x * m._13 + y * m._23 + z * m._33 + 1.0f * m._43;
	return v;
}

Vector Vector::operator ^ (Vector const& v)
{
	Vector productV(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
	return productV;
}

float Vector::operator | (Vector const& v)
{
	return x * v.x + y * v.y + z * v.z;
}

//-------------------------------------------------
// member functions of Vector4
Vector4::Vector4()
{
	x = y = z = w = 0.0f;
}

Vector4::Vector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4::Vector4(const Vector4& v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = v.w;
}

float Vector4::GetLengthSquared()
{
	return (x * x + y * y + z * z + w * w);
}

float Vector4::GetLength()
{
	return sqrt(GetLengthSquared());
}

void Vector4::Normalise()
{
	float inverseLength = 1.0f / GetLength();
	x = x * inverseLength;
	y = y * inverseLength;
	z = z * inverseLength;
	w = w * inverseLength;
}

Vector4 Vector4::operator + (Vector4& v)
{
	return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4 Vector4::operator - (Vector4& v)
{
	return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4 Vector4::operator * (Vector4& v)
{
	return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector4 Vector4::operator * (float scala)
{
	return Vector4(x * scala, y * scala, z * scala, w * scala);
}

Vector4 Vector4::operator / (float scala)
{
	return Vector4(x / scala, y / scala, z / scala, w / scala);
}

Vector4 Vector4::operator += (Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

Vector4 Vector4::operator -= (Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

Vector4 Vector4::operator *= (float scala)
{
	x *= scala;
	y *= scala;
	z *= scala;
	w *= scala;
	return *this;
}

bool Vector4::operator == (Vector4& v)
{
	if ((fabs(x - v.x) < EPSILON)
		&& (fabs(y - v.y) < EPSILON)
		&& (fabs(z - v.z) < EPSILON)
		&& (fabs(w - v.w) < EPSILON))
	{
		return true;
	}
	return false;
}

bool Vector4::operator <= (Vector4& v)
{
	if ((x <= v.x) && (y <= v.y) && (z <= v.z) && (w <= v.w)) return true;
	return false;
}

bool Vector4::operator >= (Vector4& v)
{
	if ((x >= v.x) && (y >= v.y) && (z >= v.z) && (w >= v.w)) return true;
	return false;
}


//-------------------------------------------------
// member functions of Matrix

Matrix::Matrix()
{
	MakeIdentity();
}

Matrix::Matrix(float4x4 contents)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = contents.m[i][j];
		}
	}
}

Matrix Matrix::GetIdentity()
{
	Matrix m;
	return m;
}

void Matrix::MakeIdentity()
{
	_12 = _13 = _14 = 0.0f;
	_21 = _23 = _24 = 0.0f;
	_31 = _32 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;
	_11 = _22 = _33 = _44 = 1.0f;
}

Matrix Matrix::XMMatrixInverse()
{
	Matrix res;
	DirectX::FXMMATRIX M = DirectX::XMLoadFloat4x4(this);
	DirectX::XMVECTOR det;
	DirectX::XMStoreFloat4x4(&res, DirectX::XMMatrixInverse(&det, M));
	return res;
}

Matrix Matrix::XMMatrixTranspose(Matrix& m)
{
	Matrix outM;
	DirectX::FXMMATRIX M = DirectX::XMLoadFloat4x4(&m);
	DirectX::XMStoreFloat4x4(&outM, DirectX::XMMatrixTranspose(M));
	return outM;
}

void Matrix::Transpose()
{
	Matrix m;
	m._11 = _11; m._12 = _21; m._13 = _31; m._14 = _41;
	m._21 = _12; m._22 = _22; m._23 = _32; m._24 = _42;
	m._31 = _13; m._32 = _23; m._33 = _33; m._34 = _43;
	m._41 = _14; m._42 = _24; m._43 = _34; m._44 = _44;
	*this = m;
}

Matrix Matrix::GetTransposed()
{
	Matrix m;
	m._11 = _11; m._12 = _21; m._13 = _31; m._14 = _41;
	m._21 = _12; m._22 = _22; m._23 = _32; m._24 = _42;
	m._31 = _13; m._32 = _23; m._33 = _33; m._34 = _43;
	m._41 = _14; m._42 = _24; m._43 = _34; m._44 = _44;
	return m;
}

void Matrix::RotateX(float rad)
{
	Matrix rot;
	float cosTheta = cos(rad);
	float sinTheta = sin(rad);
	rot._22 = cosTheta;	 rot._23 = sinTheta;
	rot._32 = -sinTheta; rot._33 = cosTheta;
	*this = *this * rot;
}

void Matrix::RotateY(float rad)
{
	Matrix rot;
	float cosTheta = cos(rad);
	float sinTheta = sin(rad);
	rot._11 = cosTheta;	 rot._13 = -sinTheta;
	rot._31 = sinTheta;	 rot._33 = cosTheta;
	*this = *this * rot;
}

void Matrix::RotateZ(float rad)
{
	Matrix rot;
	float cosTheta = cos(rad);
	float sinTheta = sin(rad);
	rot._11 = cosTheta;	 rot._12 = sinTheta;
	rot._21 = -sinTheta; rot._22 = cosTheta;
	*this = *this * rot;
}

void Matrix::Scale(float x, float y, float z)
{
	_11 = _11 * x;
	_22 = _22 * y;
	_33 = _33 * z;
}

void Matrix::Translate(float x, float y, float z)
{
	_41 = _41 + x;
	_42 = _42 + y;
	_43 = _43 + z;
}

void Matrix::RotateRollPitchYaw(float roll, float pitch, float yaw)
{
	Matrix rotX, rotY, rotZ, rot;
	rotZ.RotateZ(roll);
	rotX.RotateX(pitch);
	rotY.RotateY(yaw);
	rot = rotZ * rotX * rotY;

	*this = *this * rot;
}

Matrix Matrix::operator* (Matrix& matrix)
{
	Matrix mat;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			mat.m[i][j] =
				m[i][0] * matrix.m[0][j] +
				m[i][1] * matrix.m[1][j] +
				m[i][2] * matrix.m[2][j] +
				m[i][3] * matrix.m[3][j];
		}
	}
	return mat;
}

void Matrix::XMMatrixLookAt(const Vector& pos, const Vector& target, const Vector& up)
{
	DirectX::XMVECTOR posV = DirectX::XMLoadFloat3(&pos);
	DirectX::XMVECTOR targetV = DirectX::XMLoadFloat3(&target);
	DirectX::XMVECTOR upV = DirectX::XMLoadFloat3(&up);
	DirectX::XMStoreFloat4x4(this, DirectX::XMMatrixLookAtLH(posV, targetV, upV));
}

bool Matrix::XMMatrixDecompose(Vector& scale, Quaternion& rotation, Vector& translation, Matrix& mat)
{
	DirectX::XMVECTOR S, R, T;
	DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&mat);
	if (DirectX::XMMatrixDecompose(&S, &R, &T, M))
	{
		DirectX::XMStoreFloat3(&scale, S);
		DirectX::XMStoreFloat4(&rotation, R);
		DirectX::XMStoreFloat3(&translation, T);
		DirectX::XMStoreFloat4x4(&mat, M);
		return true;
	}
	else return false;
}

// 외적을 사용하여 뷰 행렬 생성
void Matrix::ViewLookAtCC(Vector& pos, Vector& target, Vector& up)
{
	Vector zAxis = (target - pos).GetNormal();
	Vector xAxis = (up.cross(zAxis)).GetNormal();
	Vector yAxis = (zAxis.cross(xAxis)).GetNormal();

	_11 = xAxis.x;	_12 = yAxis.x;	_13 = zAxis.x;
	_21 = xAxis.y;	_22 = yAxis.y;	_23 = zAxis.y;
	_31 = xAxis.z;	_32 = yAxis.z;	_33 = zAxis.z;

	_41 = -(pos.dot(xAxis));
	_42 = -(pos.dot(yAxis));
	_43 = -(pos.dot(zAxis));
}

// 내적을 사용하여 뷰 행렬 생성
Matrix Matrix::ViewLookAtDC(Vector& pos, Vector& target, Vector& up)
{
	Matrix m;
	Vector zAxis = (target - pos).GetNormal();
	Vector xAxis = (up.cross(zAxis)).GetNormal();
	Vector yAxis = (zAxis.cross(xAxis)).GetNormal();

	m._11 = xAxis.x;	m._12 = yAxis.x;	m._13 = zAxis.x;
	m._21 = xAxis.y;	m._22 = yAxis.y;	m._23 = zAxis.y;
	m._31 = xAxis.z;	m._32 = yAxis.z;	m._33 = zAxis.z;

	m._41 = -(pos.dot(xAxis));
	m._42 = -(pos.dot(yAxis));
	m._43 = -(pos.dot(zAxis));

	return m;
}

void Matrix::ObjectLookAt(Vector& pos, Vector& target, Vector& up)
{
	Vector zAxis = (target - pos).GetNormal();
	float d = up.dot(zAxis);
	Vector c = zAxis * d;
	Vector yAxis = (up - c).GetNormal();
	Vector xAxis = (yAxis.cross(zAxis)).GetNormal();

	_11 = xAxis.x;	_12 = xAxis.y;	_13 = xAxis.z;
	_21 = yAxis.x;	_22 = yAxis.y;	_23 = yAxis.z;
	_31 = zAxis.x;	_32 = zAxis.y;	_33 = zAxis.z;
	_41 = pos.x;	_42 = pos.y;	_43 = pos.z;
}

void Matrix::PerspectiveFovLH(float zNear, float zFar, float fovH, float aspectRatio)
{
	float H, W, Q;
	H = 1 / tan(fovH * 0.5f);  // 1/tans(x) = cot(x)
	W = H / aspectRatio;
	Q = zFar / (zFar - zNear);

	MakeIdentity();
	_11 = W;
	_22 = H;
	_33 = Q;
	_43 = -Q * zNear;
	_34 = 1;
	_44 = 0.0f;
}

void Matrix::OrthoLH(float w, float h, float n, float f)
{
	MakeIdentity();
	_11 = 2.0f / w;
	_22 = 2.0f / h;
	_33 = 1.0f / (f - n);
	_43 = -n / (f - n);
}

void Matrix::OrthoOffCenterLH(float l, float r, float b, float t, float n, float f)
{
	MakeIdentity();
	_11 = 2.0f / (r - l);
	_22 = 2.0f / (t - b);
	_33 = 1.0f / (f - n);
	_43 = -n / (f - n);
	_41 = (l + r) / (l - r);
	_42 = (t + b) / (b - t);
}

/**
 * Builds the identity matrix.
 *
 * @return	the identity matrix
 */
Matrix Matrix::XMMatrixIdentity(Matrix& m)
{
	DirectX::XMStoreFloat4x4(&m, DirectX::XMMatrixIdentity());
	return m;
}

/*
Builds and returns a matrix multiplication of (mA, mB)
*/
Matrix Matrix::XMMatrixMultiply(Matrix& mA, Matrix& mB)
{
	DirectX::XMMATRIX MA = DirectX::XMLoadFloat4x4(&mA);
	DirectX::XMMATRIX MB = DirectX::XMLoadFloat4x4(&mB);
	DirectX::XMMATRIX MOUT = DirectX::XMMatrixMultiply(MA, MB);

	Matrix outM;
	DirectX::XMStoreFloat4x4(&outM, MOUT);
	return outM;
}

/*
Builds and returns a matrix which scales by (sx, sy, sz)
*/
Matrix Matrix::XMMatrixScaling(float sx, float sy, float sz)
{
	Matrix outM;
	DirectX::XMStoreFloat4x4(&outM, DirectX::XMMatrixScaling(sx, sy, sz));
	return outM;
}

Matrix Matrix::XMMatrixRotationX(float radians)
{
	Matrix outM;
	DirectX::XMStoreFloat4x4(&outM, DirectX::XMMatrixRotationX(radians));
	return outM;
}

Matrix Matrix::XMMatrixRotationY(float radians)
{
	Matrix outM;
	DirectX::XMStoreFloat4x4(&outM, DirectX::XMMatrixRotationY(radians));
	return outM;
}

Matrix Matrix::XMMatrixRotationZ(float radians)
{
	Matrix outM;
	DirectX::XMStoreFloat4x4(&outM, DirectX::XMMatrixRotationZ(radians));
	return outM;
}

/**
 * Builds a rotation matrix from a quaternion.
 *
 * @param	q the quaternion defining the rotation
 * @return	converted rotation matrix
 */
Matrix Matrix::XMMatrixRotationQuaternion(Quaternion& q)
{
	Matrix outM;
	DirectX::XMVECTOR Q = DirectX::XMLoadFloat4(&q);
	DirectX::XMMATRIX OUTM = DirectX::XMMatrixRotationQuaternion(Q);
	DirectX::XMStoreFloat4x4(&outM, OUTM);
	return outM;
}

/*
Returns the affine transformation matrix,
built from the scaling, rotation, and translation information
*/
Matrix Matrix::XMMatrixAffineTransformation(
	CONST Vector scaling,
	CONST Vector* rotationCentre, CONST Quaternion* rotationQuat,
	CONST Vector* translation)
{
	DirectX::XMVECTOR s, rc, r, t;

	s = DirectX::XMLoadFloat3(&scaling);
	if (rotationCentre == NULL)
		rc = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	else
		rc = DirectX::XMLoadFloat3(rotationCentre);
	r = DirectX::XMLoadFloat4(rotationQuat);
	t = DirectX::XMLoadFloat3(translation);

	Matrix outM;
	DirectX::XMMATRIX XMMat = DirectX::XMMatrixAffineTransformation(s, rc, r, t);
	DirectX::XMStoreFloat4x4(&outM, XMMat);

	return outM;
}

Matrix Matrix::XMMatrixRotateRollPitchYaw(float roll, float pitch, float yaw)
{
	Matrix outM;
	DirectX::XMStoreFloat4x4(&outM, DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	return outM;
}



//-------------------------------------------------
// member functions of Matrix3

Matrix3::Matrix3()
{
	*this = GetIdentity();
}

Matrix3::Matrix3(float3x3 contents)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			m[i][j] = contents.m[i][j];
		}
	}
}

Matrix3 Matrix3::GetIdentity()
{
	Matrix3 m;
	m._12 = m._13 = 0.0f;
	m._21 = m._23 = 0.0f;
	m._31 = m._32 = 0.0f;
	m._11 = m._22 = m._33 = 1.0f;
	return m;
}

void Matrix3::Transpose()
{
	Matrix3 m;
	m._11 = _11; m._12 = _21; m._13 = _31;
	m._21 = _12; m._22 = _22; m._23 = _32;
	m._31 = _13; m._32 = _23; m._33 = _33;
	*this = m;
}

void Matrix3::RotateX(float rad)
{
	float cosTheta = cos(rad);
	float sinTheta = sin(rad);
	_22 = cosTheta;	 _23 = sinTheta;
	_32 = -sinTheta; _33 = cosTheta;
}

void Matrix3::RotateY(float rad)
{
	float cosTheta = cos(rad);
	float sinTheta = sin(rad);
	_11 = cosTheta;	 _13 = -sinTheta;
	_31 = sinTheta;	 _33 = cosTheta;
}

void Matrix3::RotateZ(float rad)
{
	float cosTheta = cos(rad);
	float sinTheta = sin(rad);
	_11 = cosTheta;	 _12 = sinTheta;
	_21 = -sinTheta; _22 = cosTheta;
}

void Matrix3::Scale(float x, float y)
{
	_11 = x;
	_22 = y;
}

void Matrix3::Translate(float x, float y)
{
	_31 = x;
	_32 = y;
}

Matrix3 Matrix3::operator* (Matrix3& matrix)
{
	Matrix3 mat;
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			mat.m[i][j] =
				m[i][0] * matrix.m[0][j] +
				m[i][1] * matrix.m[1][j] +
				m[i][2] * matrix.m[2][j];
		}
	}
	return mat;
}

//-------------------------------------
// Quaternion
// ------------------------------------
// CONSTRUCTORS

Quaternion::Quaternion()
{
	x, y, z, w = 0.0f;
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quaternion::Quaternion(float scalar, Vector v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = scalar;
}

Quaternion::Quaternion(Vector4 v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = v.w;
}

// OVERLOADED OPERATORS


// USEFUL FUNCTIONS

// create quaternion from roll, pitch, and yaw value
void Quaternion::XMQuaternionRotationRollPitchYaw(Quaternion *q, float roll, float pitch, float yaw)
{
	DirectX::XMStoreFloat4(q, DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
}

/*
Creates quaternion from rotation matrix
*/
void Quaternion::XMQuaternionRotationMatrix(Quaternion& q, Matrix& m)
{
	DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&m);
	DirectX::XMStoreFloat4(&q, DirectX::XMQuaternionRotationMatrix(M));
}


/**
 * Interpolates between two unit quaternions, using spherical linear interpolation.
 *
 * @param	q1 the unit quaternion to interpolate from
 * @param	q2 the unit quaternion to interpolate to
 * @param	t the interpolation control factor
 * @return	interpolated quaternion 
 *		undefined if q1 and q2 are not unit quaternions
 */
Quaternion Quaternion::XMQuaternionSlerp(Quaternion& q1, Quaternion& q2, float t)
{
	DirectX::XMVECTOR Q1 = DirectX::XMLoadFloat4(&q1);
	DirectX::XMVECTOR Q2 = DirectX::XMLoadFloat4(&q2);

	Quaternion ret;
	XMStoreFloat4(&ret, DirectX::XMQuaternionSlerp(Q1, Q2, t));
	return ret;
}