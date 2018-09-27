#include <windows.h> // for FLOAT definition
#include <DirectXMath.h>
#include <iostream>
using namespace std;
using namespace DirectX;
 
// Overload the "<<" operators so that we can use cout to
// output XMVECTOR and XMMATRIX objects.
ostream& operator<<(ostream& os, FXMVECTOR v) {
	XMFLOAT4 dest;
	XMStoreFloat4(&dest, v);
	os << "(" << dest.x << ", " << dest.y << ", "
		<< dest.z << ", " << dest.w << ")";
	return os;
}
ostream& operator<<(ostream& os, CXMMATRIX m) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j)
			os << m(i, j) << "\t";
		os << endl;
	}
	return os;
}
int main1() {
	cout.setf(ios_base::boolalpha);
	// Check support for SSE2 (Pentium4, AMD K8, and above).
	if (!XMVerifyCPUSupport()) {
		cout << "xna math not supported" << endl;
		return 0;
	}
	XMVECTOR n = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
	XMVECTOR v = XMVectorSet(-2.0f, 1.0f, -3.0f, 0.0f);
	XMVECTOR w = XMVectorSet(0.707f, 0.707f, 0.0f, 0.0f);
	// Vector addition: XMVECTOR operator +
	XMVECTOR a = u + v;
	// Vector subtraction: XMVECTOR operator -
	XMVECTOR b = u - v;
	// Scalar multiplication: XMVECTOR operator *
	XMVECTOR c = 10.0f*u;
	// ||u||
	XMVECTOR L = XMVector3Length(u);
	// d = u / ||u||
	XMVECTOR d = XMVector3Normalize(u);
	// s = u dot v
	XMVECTOR s = XMVector3Dot(u, v);
	// e = u x v
	XMVECTOR e = XMVector3Cross(u, v);
	// Find proj_n(w) and perp_n(w)
	XMVECTOR projW;
	XMVECTOR perpW;
	XMVector3ComponentsFromNormal(&projW, &perpW, w, n);
	// Does projW + perpW == w?
	bool equal = XMVector3Equal(projW + perpW, w) != 0;
	bool notEqual = XMVector3NotEqual(projW + perpW, w) != 0;
	// The angle between projW and perpW should be 90 degrees.
	XMVECTOR angleVec = XMVector3AngleBetweenVectors(projW, perpW);
	float angleRadians = XMVectorGetX(angleVec);
	float angleDegrees = XMConvertToDegrees(angleRadians);
	cout << "u = " << u << endl;
	cout << "v = " << v << endl;
	cout << "w = " << w << endl;
	cout << "n = " << n << endl;
	cout << "a = u + v = " << a << endl;
	cout << "b = u - v = " << b << endl;
	cout << "c = 10 * u = " << c << endl;
	cout << "d = u / ||u|| = " << d << endl;
	cout << "e = u x v = " << e << endl;
	cout << "L = ||u|| = " << L << endl;
	cout << "s = u.v = " << s << endl;
	cout << "projW = " << projW << endl;
	cout << "perpW = " << perpW << endl;
	cout << "projW + perpW == w = " << equal << endl;
	cout << "projW + perpW != w = " << notEqual << endl;
	cout << "angle = " << angleDegrees << endl;
	return 0;
}

int main2() {
	cout.precision(8);
	// Check support for SSE2 (Pentium4, AMD K8, and above).
	if (!XMVerifyCPUSupport()) {
		cout << "xna math not supported" << endl;
		return 0;
	}
	XMVECTOR u = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	XMVECTOR n = XMVector3Normalize(u);
	float LU = XMVectorGetX(XMVector3Length(n));
	float LU2 = XMVectorGetY(XMVector3Length(n));
	float LU3 = XMVectorGetZ(XMVector3Length(n));
	float LU4 = XMVectorGetW(XMVector3Length(n));
	std::cout << "LU=" << LU << "\nLU2=" << LU2 << "\nLU3=" << LU3 << "\nLU4=" << LU4 << endl;
	// Mathematically, the length should be 1. Is it numerically?
	if (LU == 1.0f)
		cout << "Length 1" << endl;
	else
		cout << "Length not 1" << endl;
	// Raising 1 to any power should still be 1. Is it?
	float powLU = powf(LU, 1.0e6f);
	cout << "LU^(10^6) = " << powLU << endl;
	return 0;
}


int main3() {
	cout.setf(ios_base::boolalpha);
	// Check support for SSE2 (Pentium4, AMD K8, and above).
	if (!XMVerifyCPUSupport()) {
		cout << "xna math not supported" << endl;
		return 0;
	}
	XMVECTOR p = XMVectorSet(2.0f, 2.0f, 1.0f, 0.0f);
	XMVECTOR q = XMVectorSet(2.0f, -0.5f, 0.5f, 0.1f);
	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 4.0f, 8.0f);
	XMVECTOR v = XMVectorSet(-2.0f, 1.0f, -3.0f, 2.5f);
	XMVECTOR w = XMVectorSet(0.0f, XM_PIDIV4, XM_PIDIV2, XM_PI);
	cout << "XMVectorAbs(v) = " << XMVectorAbs(v) << endl;
	cout << "XMVectorCos(w) = " << XMVectorCos(w) << endl;
	cout << "XMVectorLog(u) = " << XMVectorLog(u) << endl;
	cout << "XMVectorExp(p) = " << XMVectorExp(p) << endl;
	cout << "XMVectorPow(u, p) = " << XMVectorPow(u, p) << endl;
	cout << "XMVectorSqrt(u) = " << XMVectorSqrt(u) << endl;
	cout << "XMVectorSwizzle(u, 2, 2, 1, 3) = "
		<< XMVectorSwizzle(u, 2, 2, 1, 3) << endl;
	cout << "XMVectorSwizzle(u, 2, 1, 0, 3) = "
		<< XMVectorSwizzle(u, 2, 1, 0, 3) << endl;
	cout << "XMVectorMultiply(u, v) = "
		<< XMVectorMultiply(u, v) << endl;
	cout << "XMVectorSaturate(q) = "
		<< XMVectorSaturate(q) << endl;
	cout << "XMVectorMin(p, v = " << XMVectorMin(p, v) << endl;
	cout << "XMVectorMax(p, v) = " << XMVectorMax(p, v) << endl;
	return 0;
}



int main() {
	// Check support for SSE2 (Pentium4, AMD K8, and above).
	if (!XMVerifyCPUSupport()) {
		cout << "xna math not supported" << endl;
		return 0;
	}
	XMMATRIX A(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 4.0f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f);
	XMMATRIX B = XMMatrixIdentity();
	XMMATRIX C = A * B;
	XMMATRIX D = XMMatrixTranspose(A);
	XMVECTOR det = XMMatrixDeterminant(A);
	XMMATRIX E = XMMatrixInverse(&det, A);
	XMMATRIX F = A * E;
	cout << "A = " << endl << A << endl;
	cout << "B = " << endl << B << endl;
	cout << "C = A*B = " << endl << C << endl;
	cout << "D = transpose(A) = " << endl << D << endl;
	cout << "det = determinant(A) = " << det << endl << endl;
	cout << "E = inverse(A) = " << endl << E << endl;
	cout << "F = A*E = " << endl << F << endl;
	return 0;
}