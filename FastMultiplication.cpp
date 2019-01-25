#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<map>
#include<queue>
#include<complex>
#pragma warning(disable : 4996)

using namespace std;

const double PI = 3.1415926535897932384626433832795;

void fft(vector<complex<double> > & a, bool side) {
	int n = a.size();
	if (n == 1)
		return;

	vector<complex<double> > a0, a1;
	for (int i = 0; i < n; i++) {
		if (i % 2 == 0)
			a0.push_back(a[i]);
		else
			a1.push_back(a[i]);
	}

	fft(a0, side);
	fft(a1, side);

	double ang = 2 * PI / n * (side ? 1 : -1);
	complex<double> w(1), wn(cos(ang), sin(ang));
	for (int i = 0; i < n / 2; i++) {
		a[i] = a0[i] + a1[i] * w;
		a[i + n / 2] = a0[i] - a1[i] * w;
		if (!side) {
			a[i] /= 2;
			a[i + n / 2] /= 2;
		}
		w *= wn;
	}
}

void multiply(vector<int> & a, vector<int> & b, vector<int> & rez) {
	vector<complex<double> > fa(a.begin(), a.end()), fb(b.begin(), b.end());
	int n = 1;
	while (n < max(a.size(), b.size()))
		n <<= 1;
	n <<= 1;
	fa.resize(n);
	fb.resize(n);

	fft(fa, true);
	fft(fb, true);
	for (int i = 0; i < n; i++) {
		fa[i] *= fb[i];
	}

	fft(fa, false);

	rez.resize(n);
	for (int i = 0; i < n; i++) {
		rez[i] = fa[i].real() > 0 ? (int)(fa[i].real() + 0.5) : (int)(fa[i].real() - 0.5);
	}
}

int modm = 2;

int mn(string & s, int fi, int se) {
	int y = 0;
	for (int i = fi; i <= se; i++) {
		y = y * 10 + s[i] - '0';
	}
	return y;
}

void fn(string & s, vector<int> & n) {
	int m = (s[0] == '-' ? 1 : 0);
	for (int i = s.size() - 1; i >= m; i -= modm)
		n.push_back(mn(s, max(i - (modm - 1), m), i));
}

int ns(int a) {
	if (a == 0)
		return 1;
	int k = 0;
	while (a > 0) {
		a /= 10;
		k++;
	}
	return k;
}


int main() {
	/*freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);*/

	string a, b;
	cin >> a >> b;
	vector<int> a1, b1;
	vector<int> rez;

	fn(a, a1);
	fn(b, b1);

	multiply(a1, b1, rez);

	for (int i = 0; i < rez.size() - 1; i++) {
		rez[i + 1] += rez[i] / 100;
		rez[i] %= 100;
	}

	while (rez[rez.size() - 1] > 100) {
		int a = rez[rez.size() - 1] / 100;
		rez[rez.size() - 1] %= 100;
		rez.push_back(a);
	}

	while (rez.size() > 1 && rez[rez.size() - 1] == 0)
		rez.pop_back();
	if (rez[rez.size() - 1] != 0) {
		if ((a[0] == '-' && b[0] != '-') || (a[0] != '-' && b[0] == '-'))
			cout << "-";
	}
	for (int i = rez.size() - 1; i >= 0; i--) {
		if (i != rez.size() - 1) {
			int ss = ns(rez[i]);
			while (ss < modm) {
				cout << 0;
				ss++;
			}
		}
		cout << rez[i];
	}
	return 0;
}
