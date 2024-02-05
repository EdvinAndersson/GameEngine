#pragma once

#include <math.h>

#define M_PI   3.14159265358979323846264338327950288

struct Vec2 {
	union {
		struct {
			float x, y;
		};
		struct {
			float width, height;
		};
		float xy[2];
	};
};
struct Vec3 {
	union {
		struct {
			float x, y, z;
		};
		struct {
			float r, g, b;
		};
		float xyz[3];
		float rgb[3];
	};
};

struct Vec4 {
	union {
		struct {
			float x, y, z, w;
		};
		struct {
			float r, g, b, a;
		};
		struct {
			float x, y, width, height;
		};
		float xyzw[4];
		float rgba[4];
	};
};

struct Mat4 {
	float m[16];
};

Vec2 inline operator+(Vec2 v1, float v) {
	return Vec2{ v1.x + v, v1.y + v };
}
Vec3 inline operator+(Vec3 v1, float v) {
	return Vec3{ v1.x + v, v1.y + v, v1.z + v };
}
Vec4 inline operator+(Vec4 v1, float v) {
	return Vec4{ v1.x + v, v1.y + v, v1.z + v , v1.w + v };
}

Vec2 inline operator+(Vec2 v1, Vec2 v2) {
	return Vec2{ v1.x + v2.x, v1.y + v2.y };
}
Vec3 inline operator+(Vec3 v1, Vec3 v2) {
	return Vec3{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}
Vec4 inline operator+(Vec4 v1, Vec4 v2) {
	return Vec4{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z , v1.w + v2.w };
}

Vec2 inline operator-(Vec2 v1, Vec2 v2) {
	return Vec2{ v1.x - v2.x, v1.y - v2.y };
}
Vec3 inline operator-(Vec3 v1, Vec3 v2) {
	return Vec3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}
Vec4 inline operator-(Vec4 v1, Vec4 v2) {
	return Vec4{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z , v1.w - v2.w };
}
Vec2 inline operator*(Vec2 v1, float a) {
	return Vec2{ v1.x * a, v1.y * a};
}
Vec3 inline operator*(Vec3 v1, float a) {
	return Vec3{ v1.x * a, v1.y * a, v1.z * a };
}
Vec4 inline operator*(Vec4 v1, float a) {
	return Vec4{ v1.x * a, v1.y * a, v1.z * a , v1.w * a };
}
Vec2 inline operator/(Vec2 v1, float a) {
	return Vec2{ v1.x / a, v1.y / a };
}
Vec3 inline operator/(Vec3 v1, float a) {
	return Vec3{ v1.x / a, v1.y / a, v1.z / a };
}
Vec4 inline operator/(Vec4 v1, float a) {
	return Vec4{ v1.x / a, v1.y / a, v1.z / a , v1.w / a };
}

float inline Vec2DotPro(Vec2 a, Vec2 b) {
	return a.x * b.x + a.y * b.y;
}
float inline Vec3DotPro(Vec3 a, Vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
float inline Vec4DotPro(Vec4 a, Vec4 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Vec3 inline Vec3CrossPro(Vec3 a, Vec3 b) {
	return Vec3{ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}
Vec2 inline Vec2Div(Vec2 a, float b) {
	return Vec2{ a.x / b, a.y / b };
}
Vec3 inline Vec3Div(Vec3 a, float b) {
	return Vec3{ a.x / b, a.y / b, a.z / b };
}

float inline Vec2Length(Vec2 a) {
	return sqrtf(powf(a.x, 2) + powf(a.y, 2));
}
float inline Vec3Length(Vec3 a) {
	return sqrtf(powf(a.x, 2) + powf(a.y, 2) + powf(a.z, 2));
}
Vec2 inline Vec2Normalize(Vec2 a) {
	return Vec2Div(a, Vec2Length(a));
}
Vec3 inline Vec3Normalize(Vec3 a) {
	return Vec3Div(a, Vec3Length(a));
}
Vec2 inline operator-(Vec2 a) {
	return Vec2 { -a.x , -a.y };
}
Vec3 inline operator-(Vec3 a) {
	return Vec3{ -a.x , -a.y , -a.z };
}
Vec4 inline operator-(Vec4 a) {
	return Vec4{ -a.x , -a.y , -a.z , -a.w };
}

Vec2 inline Vec2Rotate(Vec2 v, float rotation_rad) {
	return Vec2 {
		cosf(rotation_rad) * v.x - sinf(rotation_rad) * v.y,
		sinf(rotation_rad) *v.x + cosf(rotation_rad) * v.y
	};
}

Mat4 inline Mat4Identity() {
	Mat4 out = { 0 };
	out.m[0] = 1;
	out.m[5] = 1;
	out.m[10] = 1;
	out.m[15] = 1;
	return out;
}

Mat4 inline operator*(Mat4 a, Mat4 b) {
	Mat4 c;
	/*c.m[0] = a.m[0] * b.m[0] + a.m[1] * b.m[4] + a.m[2] * b.m[8] + a.m[3] * b.m[12];
	c.m[1] = a.m[0] * b.m[1] + a.m[1] * b.m[5] + a.m[2] * b.m[9] + a.m[3] * b.m[13];
	c.m[2] = a.m[0] * b.m[2] + a.m[1] * b.m[6] + a.m[2] * b.m[10] + a.m[3] * b.m[14];
	c.m[3] = a.m[0] * b.m[3] + a.m[1] * b.m[7] + a.m[2] * b.m[11] + a.m[3] * b.m[15];

	c.m[4] = a.m[4] * b.m[0] + a.m[5] * b.m[4] + a.m[6] * b.m[8] + a.m[7] * b.m[12];
	c.m[5] = a.m[4] * b.m[1] + a.m[5] * b.m[5] + a.m[6] * b.m[9] + a.m[7] * b.m[13];
	c.m[6] = a.m[4] * b.m[2] + a.m[5] * b.m[6] + a.m[6] * b.m[10] + a.m[7] * b.m[14];
	c.m[7] = a.m[4] * b.m[3] + a.m[5] * b.m[7] + a.m[6] * b.m[11] + a.m[7] * b.m[15];

	c.m[8] = a.m[8] * b.m[0] + a.m[9] * b.m[4] + a.m[10] * b.m[8] + a.m[11] * b.m[12];
	c.m[9] = a.m[8] * b.m[1] + a.m[9] * b.m[5] + a.m[10] * b.m[9] + a.m[11] * b.m[13];
	c.m[10] = a.m[8] * b.m[2] + a.m[9] * b.m[6] + a.m[10] * b.m[10] + a.m[11] * b.m[14];
	c.m[11] = a.m[8] * b.m[3] + a.m[9] * b.m[7] + a.m[10] * b.m[11] + a.m[11] * b.m[15];

	c.m[12] = a.m[12] * b.m[0] + a.m[13] * b.m[4] + a.m[14] * b.m[8] + a.m[15] * b.m[12];
	c.m[13] = a.m[12] * b.m[1] + a.m[13] * b.m[5] + a.m[14] * b.m[9] + a.m[15] * b.m[13];
	c.m[14] = a.m[12] * b.m[2] + a.m[13] * b.m[6] + a.m[14] * b.m[10] + a.m[15] * b.m[14];
	c.m[15] = a.m[12] * b.m[3] + a.m[13] * b.m[7] + a.m[14] * b.m[11] + a.m[15] * b.m[15];*/

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			c.m[i*4+j] = 0;
			for (int k = 0; k < 4; k++) {
				c.m[i * 4 + j] += a.m[i*4+k] * b.m[k*4+j];
			}
		}
	}

	return c;
}
Mat4 inline operator-(Mat4 mat) {
	Mat4 out;
	for (int i = 0; i < 15; i++)
		out.m[i] = -mat.m[i];
	return out;
}
/* Dosen't work!
Vec4 inline operator*(Mat4 mat, Vec4 a) {
	return Vec4{
		mat.m[0] * a.x + mat.m[1] * a.y + mat.m[2] * a.z + mat.m[3] * a.w,
		mat.m[4] * a.x + mat.m[5] * a.y + mat.m[6] * a.z + mat.m[7] * a.w,
		mat.m[8] * a.x + mat.m[9] * a.y + mat.m[10] * a.z + mat.m[11] * a.w,
		mat.m[12] * a.x + mat.m[13] * a.y + mat.m[14] * a.z + mat.m[15] * a.w
	};
}*/
Mat4 inline Mat4TranslateV2(Mat4 mat, Vec2 a) {
	if (a.x == 0 && a.y == 0) return mat;

	Mat4 transMat = Mat4Identity();
	transMat.m[3 * 4 + 0] = a.x;
	transMat.m[3 * 4 + 1] = a.y;

	return transMat * mat;
}
Mat4 inline Mat4ScaleV2(Mat4 mat, Vec2 a) {
	if (a.x == 0 && a.y == 0) return mat;

	Mat4 scaleMat = Mat4Identity();
	scaleMat.m[0 * 4 + 0] = a.x;
	scaleMat.m[1 * 4 + 1] = a.y;

	return scaleMat * mat;
}
Mat4 inline Mat4TranslateV3(Mat4 mat, Vec3 a) {
	if (a.x == 0 && a.y == 0 && a.z == 0) return mat;

	Mat4 transMat = Mat4Identity();
	transMat.m[3 * 4 + 0] = a.x;
	transMat.m[3 * 4 + 1] = a.y;
	transMat.m[3 * 4 + 2] = a.z;

	return transMat * mat;
}
Mat4 inline Mat4ScaleV3(Mat4 mat, Vec3 a) {
	if (a.x == 0 && a.y == 0 && a.z == 0) return mat;

	Mat4 scaleMat = Mat4Identity();
	scaleMat.m[0 * 4 + 0] = a.x;
	scaleMat.m[1 * 4 + 1] = a.y;
	scaleMat.m[2 * 4 + 2] = a.z;

	return scaleMat * mat;
}

Mat4 inline Mat4LookAt(Vec3 pos, Vec3 target, Vec3 up) {
	Vec3 f, u, s;

	f = Vec3Normalize(target - pos);
	s = Vec3Normalize(Vec3CrossPro(f, up));
	u = Vec3CrossPro(s, f);

	Mat4 dest;

	dest.m[0] = s.x;
	dest.m[1] = u.x;
	dest.m[2] = -f.x;
	dest.m[3] = 0.0f;

	dest.m[4] = s.y;
	dest.m[5] = u.y;
	dest.m[6] = -f.y;
	dest.m[7] = 0.0f;

	dest.m[8] = s.z;
	dest.m[9] = u.z;
	dest.m[10] = -f.z;
	dest.m[11] = 0.0f;

	dest.m[12] = -Vec3DotPro(s, pos);
	dest.m[13] = -Vec3DotPro(u, pos);
	dest.m[14] = Vec3DotPro(f, pos);
	dest.m[15] = 1.0f;

	return dest;
}

Mat4 inline Mat4Perspective(float fov, float aspect, float nearZ, float farZ) {
	float yScale = 1.0f / tanf(fov / 2);
	float xScale = yScale / aspect;
	float nearmfar = nearZ - farZ;

	Mat4 m = { {
		xScale, 0, 0, 0,
		0, yScale, 0, 0,
		0, 0, (farZ + nearZ) / nearmfar, -1,
		0, 0, 2 * farZ * nearZ / nearmfar, 0
	}};

	return m;
}

Mat4 inline Mat4Rotate(Mat4 mat, float a, Vec3 axis) {

	if (axis.x == 0 && axis.y == 0 && axis.z == 0)
		return mat;

	Mat4 rotMat = Mat4Identity();

	float r = a;
	float s = sinf(r);
	float c = cosf(r);
	float omc = 1.0f - cosf(r);

	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	rotMat.m[0 + 0 * 4] = c + x * x * omc;
	rotMat.m[1 + 0 * 4] = x * y * omc - z * s;
	rotMat.m[2 + 0 * 4] = z * x * omc + y * s;

	rotMat.m[0 + 1 * 4] = x * y * omc + z * s;
	rotMat.m[1 + 1 * 4] = c + y * y * omc;
	rotMat.m[2 + 1 * 4] = z * y * omc - x * s;

	rotMat.m[0 + 2 * 4] = x * z * omc - y * s;
	rotMat.m[1 + 2 * 4] = y * z * omc + x * s;
	rotMat.m[2 + 2 * 4] = c + z * z * omc;

	return rotMat *  mat;
}

Mat4 inline Mat4Ortho(float left, float right, float bottom, float top, float n, float f) {
	Mat4 out = { 0 };

	out.m[0 * 4 + 0] = 2 / (right - left);
	out.m[1 * 4 + 1] = 2 / (top - bottom);
	out.m[2 * 4 + 2] = -2 / (f - n);

	out.m[3 * 4 + 0] = -(right + left) / (right - left);
	out.m[3 * 4 + 1] = -(top + bottom) / (top - bottom);
	out.m[3 * 4 + 2] = -(f + n) / (f - n);
	out.m[3 * 4 + 3] = 1;

	return out;
}

struct Quaternion {
	float x, y, z, w;
};

Quaternion inline QuatIdentity() {
	return Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
}

Quaternion inline QuatConjugate(Quaternion q) {
	Quaternion out;

	out.x = -q.x;
	out.y = -q.y;
	out.z = -q.z;
	out.w = -q.w;
	
	return out;
}

Quaternion inline QuatNormalize(Quaternion q) {
	Quaternion out;
	float n = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);

	out.x = q.z / n;
	out.y = q.y / n;
	out.z = q.z / n;
	out.w = q.w / n;

	return out;
}

Quaternion inline QuatScale(Quaternion q, float scale) {
	Quaternion out;

	out.x = q.x * scale;
	out.y = q.y * scale;
	out.z = q.z * scale;
	out.w = q.w * scale;

	return out;
}

Quaternion inline QuatMultiply(Quaternion q1, Quaternion q2) {
	Quaternion out;

	out.x = q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
	out.y = -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y;
	out.z = q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z;
	out.w = -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w;

	return out;
}

Quaternion inline QuatAdd(Quaternion q1, Quaternion q2) {
	Quaternion out;

	out.x = q1.x + q2.x;
	out.y = q1.y + q2.y;
	out.z = q1.z + q2.z;
	out.w = q1.w + q2.w;

	return out;
}

Quaternion inline QuatAngleAxis(float angle, Vec3 rotationAxis) {
	Quaternion out;

	float angle_rad = (float) ((M_PI / 180.0f) * angle);

	float a = sinf(angle_rad / 2.0f);

	out.x = rotationAxis.x * a;
	out.y = rotationAxis.y * a;
	out.z = rotationAxis.z * a;
	out.w = cosf(angle_rad / 2.0f);

	return out;
}

Vec4 inline QuatToAxisAngle(Quaternion q) {
	Vec4 out;

	if (q.w == 1.0f) return Vec4{ 1.0f, 0.0f, 0.0f, 0.0f };

	out.w = 2 * acosf(q.w);
	float v = sinf(out.w / 2.0f);

	out.x = q.x / v;
	out.y = q.y / v;
	out.z = q.z / v;

	return out;
}

Vec3 inline QuatRotateVec3(Vec3 v, Quaternion q) {
	Vec3 u = { q.x, q.y, q.z };
	float s = q.w;

	Vec3 out = u * 2.0f * Vec3DotPro(u, v)
		+ v * (s * s - Vec3DotPro(u, u))
		+ Vec3CrossPro(u, v) * s * 2.0f;

	return out;
}

Mat4 inline QuatToMat4(Quaternion q) {
	Mat4 mat = Mat4Identity();

	float sqw = q.w * q.w;
	float sqx = q.x * q.x;
	float sqy = q.y * q.y;
	float sqz = q.z * q.z;

	float invs = 1.0f / (sqx + sqy + sqz + sqw);
	mat.m[0] = (sqx - sqy - sqz + sqw) * invs; // since sqw + sqx + sqy + sqz =1/invs*invs
	mat.m[5] = (-sqx + sqy - sqz + sqw) * invs;
	mat.m[10] = (-sqx - sqy + sqz + sqw) * invs;

	float tmp1 = q.x * q.y;
	float tmp2 = q.z * q.w;
	mat.m[4] = 2.0f * (tmp1 + tmp2) * invs;
	mat.m[1] = 2.0f * (tmp1 - tmp2) * invs;

	tmp1 = q.x * q.z;
	tmp2 = q.y * q.w;
	mat.m[8] = 2.0f * (tmp1 - tmp2) * invs;
	mat.m[2] = 2.0f * (tmp1 + tmp2) * invs;
	tmp1 = q.y * q.z;
	tmp2 = q.x * q.w;
	mat.m[9] = 2.0f * (tmp1 + tmp2) * invs;
	mat.m[6] = 2.0f * (tmp1 - tmp2) * invs;

	return mat;
}

Quaternion inline QuatEuler(float roll, float pitch, float yaw);
Quaternion inline QuatEuler(Vec3 euler) {
	return QuatEuler(euler.x, euler.y, euler.z);
}
Quaternion inline QuatEuler(float roll, float pitch, float yaw) { // roll (x), pitch (Y), yaw (z)
	float angle_roll = (float)((M_PI / 180.0f) * roll);
	float angle_pitch = (float)((M_PI / 180.0f) * pitch);
	float angle_yaw = (float)((M_PI / 180.0f) * yaw);

	float cr = cosf(angle_roll * 0.5f);
	float sr = sinf(angle_roll * 0.5f);
	float cp = cosf(angle_pitch * 0.5f);
	float sp = sinf(angle_pitch * 0.5f);
	float cy = cosf(angle_yaw * 0.5f);
	float sy = sinf(angle_yaw * 0.5f);

	Quaternion q;
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;

	return q;
}

Mat4 inline Mat4Inverse(Mat4 m)
{
	Mat4 inv;
	Mat4 invOut;
	double det;
	int i;

	inv.m[0] = m.m[5] * m.m[10] * m.m[15] -
		m.m[5] * m.m[11] * m.m[14] -
		m.m[9] * m.m[6] * m.m[15] +
		m.m[9] * m.m[7] * m.m[14] +
		m.m[13] * m.m[6] * m.m[11] -
		m.m[13] * m.m[7] * m.m[10];

	inv.m[4] = -m.m[4] * m.m[10] * m.m[15] +
		m.m[4] * m.m[11] * m.m[14] +
		m.m[8] * m.m[6] * m.m[15] -
		m.m[8] * m.m[7] * m.m[14] -
		m.m[12] * m.m[6] * m.m[11] +
		m.m[12] * m.m[7] * m.m[10];

	inv.m[8] = m.m[4] * m.m[9] * m.m[15] -
		m.m[4] * m.m[11] * m.m[13] -
		m.m[8] * m.m[5] * m.m[15] +
		m.m[8] * m.m[7] * m.m[13] +
		m.m[12] * m.m[5] * m.m[11] -
		m.m[12] * m.m[7] * m.m[9];

	inv.m[12] = -m.m[4] * m.m[9] * m.m[14] +
		m.m[4] * m.m[10] * m.m[13] +
		m.m[8] * m.m[5] * m.m[14] -
		m.m[8] * m.m[6] * m.m[13] -
		m.m[12] * m.m[5] * m.m[10] +
		m.m[12] * m.m[6] * m.m[9];

	inv.m[1] = -m.m[1] * m.m[10] * m.m[15] +
		m.m[1] * m.m[11] * m.m[14] +
		m.m[9] * m.m[2] * m.m[15] -
		m.m[9] * m.m[3] * m.m[14] -
		m.m[13] * m.m[2] * m.m[11] +
		m.m[13] * m.m[3] * m.m[10];

	inv.m[5] = m.m[0] * m.m[10] * m.m[15] -
		m.m[0] * m.m[11] * m.m[14] -
		m.m[8] * m.m[2] * m.m[15] +
		m.m[8] * m.m[3] * m.m[14] +
		m.m[12] * m.m[2] * m.m[11] -
		m.m[12] * m.m[3] * m.m[10];

	inv.m[9] = -m.m[0] * m.m[9] * m.m[15] +
		m.m[0] * m.m[11] * m.m[13] +
		m.m[8] * m.m[1] * m.m[15] -
		m.m[8] * m.m[3] * m.m[13] -
		m.m[12] * m.m[1] * m.m[11] +
		m.m[12] * m.m[3] * m.m[9];

	inv.m[13] = m.m[0] * m.m[9] * m.m[14] -
		m.m[0] * m.m[10] * m.m[13] -
		m.m[8] * m.m[1] * m.m[14] +
		m.m[8] * m.m[2] * m.m[13] +
		m.m[12] * m.m[1] * m.m[10] -
		m.m[12] * m.m[2] * m.m[9];

	inv.m[2] = m.m[1] * m.m[6] * m.m[15] -
		m.m[1] * m.m[7] * m.m[14] -
		m.m[5] * m.m[2] * m.m[15] +
		m.m[5] * m.m[3] * m.m[14] +
		m.m[13] * m.m[2] * m.m[7] -
		m.m[13] * m.m[3] * m.m[6];

	inv.m[6] = -m.m[0] * m.m[6] * m.m[15] +
		m.m[0] * m.m[7] * m.m[14] +
		m.m[4] * m.m[2] * m.m[15] -
		m.m[4] * m.m[3] * m.m[14] -
		m.m[12] * m.m[2] * m.m[7] +
		m.m[12] * m.m[3] * m.m[6];

	inv.m[10] = m.m[0] * m.m[5] * m.m[15] -
		m.m[0] * m.m[7] * m.m[13] -
		m.m[4] * m.m[1] * m.m[15] +
		m.m[4] * m.m[3] * m.m[13] +
		m.m[12] * m.m[1] * m.m[7] -
		m.m[12] * m.m[3] * m.m[5];

	inv.m[14] = -m.m[0] * m.m[5] * m.m[14] +
		m.m[0] * m.m[6] * m.m[13] +
		m.m[4] * m.m[1] * m.m[14] -
		m.m[4] * m.m[2] * m.m[13] -
		m.m[12] * m.m[1] * m.m[6] +
		m.m[12] * m.m[2] * m.m[5];

	inv.m[3] = -m.m[1] * m.m[6] * m.m[11] +
		m.m[1] * m.m[7] * m.m[10] +
		m.m[5] * m.m[2] * m.m[11] -
		m.m[5] * m.m[3] * m.m[10] -
		m.m[9] * m.m[2] * m.m[7] +
		m.m[9] * m.m[3] * m.m[6];

	inv.m[7] = m.m[0] * m.m[6] * m.m[11] -
		m.m[0] * m.m[7] * m.m[10] -
		m.m[4] * m.m[2] * m.m[11] +
		m.m[4] * m.m[3] * m.m[10] +
		m.m[8] * m.m[2] * m.m[7] -
		m.m[8] * m.m[3] * m.m[6];

	inv.m[11] = -m.m[0] * m.m[5] * m.m[11] +
		m.m[0] * m.m[7] * m.m[9] +
		m.m[4] * m.m[1] * m.m[11] -
		m.m[4] * m.m[3] * m.m[9] -
		m.m[8] * m.m[1] * m.m[7] +
		m.m[8] * m.m[3] * m.m[5];

	inv.m[15] = m.m[0] * m.m[5] * m.m[10] -
		m.m[0] * m.m[6] * m.m[9] -
		m.m[4] * m.m[1] * m.m[10] +
		m.m[4] * m.m[2] * m.m[9] +
		m.m[8] * m.m[1] * m.m[6] -
		m.m[8] * m.m[2] * m.m[5];

	det = m.m[0] * inv.m[0] + m.m[1] * inv.m[4] + m.m[2] * inv.m[8] + m.m[3] * inv.m[12];

	if (det == 0)
		return Mat4Identity();

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		invOut.m[i] = (float)(inv.m[i] * det);

	return invOut;
}