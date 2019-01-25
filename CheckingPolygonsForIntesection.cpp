/*
Автор:
Кора Александр

Задача:
Даны два выпуклых многоугольника на плоскости. В первом n точек, во втором m. Определите, пересекаются ли они за O(n + m). Указание. Используйте сумму Минковского.
*/

#include <algorithm>
#include <iostream>
#include <climits>
#include <stdarg.h> 
#include <vector>

#pragma warning(disable:4996);

using namespace std;

struct Point {
    double x = 0, y = 0;

    Point() {}
    Point(double nx, double ny) : x(nx), y(ny) {}

    Point operator = (const Point& p) {
        x = p.x;
        y = p.y;
        return *this;
    }
    friend const Point operator + (const Point& p1, const Point& p2);

    ~Point() {}
};
Point const operator + (const Point& p1, const Point& p2) {
    Point ans(p1.x + p2.x, p1.y + p2.y);
    return ans;
};

struct Polygon {
    size_t s = 0;
    vector <Point> verts;

    Polygon() {};
    Polygon(const vector<vector<double> >& our_pol) : s(our_pol.size()) {
        for (int i = 0; i < s; ++i) {
            Point p(our_pol[i][0], our_pol[i][1]);
            verts.push_back(p);
        }
    };
    Polygon(const vector<Point>& our_pol) : s(our_pol.size()), verts(our_pol) {};

    Polygon operator = (const Polygon& p) {
        s = p.s;
        verts = p.verts;
        return *this;
    }

    ~Polygon() {};
};

double Space(const Point& p1, const Point p2, const Point q1, const Point q2) {
    return (p2.x - p1.x) * (q2.y - q1.y) - (p2.y - p1.y) * (q2.x - q1.x);
}

bool SmallerAngle(const Point& p1, const Point p2, const Point q1, const Point q2) {
    
    double a = atan2(p2.y - p1.y, p2.x - p1.x);
    double b = atan2(q2.y - q1.y, q2.x - q1.x);

    if (a < 0 && b >= 0) {
        return false;
    }
    
    if (b < 0 && a >= 0) {
        return true;
    }

    if (a <= 0 && b <= 0) {
        a = -a;
        b = -b;
        if (a < b) {
            return false;
        } else {
            return true;
        }
    }
    
    if (a < b) {
        return true;
    } else {
        return false;
    }
};

Polygon MinSum(const Polygon& plgn1, const Polygon& plgn2) {
    int n = plgn1.s, m = plgn2.s;

    Point f_p1(LONG_MAX, LONG_MAX), f_p2(LONG_MAX, LONG_MAX);
    int ind_f_p1 = 0, ind_f_p2 = 0;
    for (int i = 0; i < n; ++i) {
        if (plgn1.verts[i].y < f_p1.y || (plgn1.verts[i].y == f_p1.y && plgn1.verts[i].x <= f_p1.x)) {
            f_p1 = plgn1.verts[i];
            ind_f_p1 = i;
        }
    }
    for (int i = 0; i < m; ++i) {
        if (plgn2.verts[i].y < f_p2.y || (plgn2.verts[i].y == f_p2.y && plgn2.verts[i].x <= f_p2.x)) {
            f_p2 = plgn2.verts[i];
            ind_f_p2 = i;
        }
    }

    vector<Point> ans;
    int i = ind_f_p1, j = ind_f_p2;
    while (n > 0 || m > 0) {
        ans.push_back(plgn1.verts[i % plgn1.verts.size()] + plgn2.verts[j % plgn2.verts.size()]);
        
        if (n == 0 && m != 0) {
            j = (j + 1) % plgn2.verts.size();
            --m;
        }

        if (n != 0 && m == 0) {
            i = (i + 1) % plgn1.verts.size();
            --n;
        }
        if(n != 0 && m != 0) {
            if (SmallerAngle(plgn1.verts[i % plgn1.verts.size()], plgn1.verts[(i + 1) % plgn1.verts.size()], plgn2.verts[j % plgn2.verts.size()], plgn2.verts[(j + 1) % plgn2.verts.size()])) {
                i = (i + 1) % plgn1.verts.size();
                --n;
            }
            else {
                if (SmallerAngle(plgn2.verts[j % plgn2.verts.size()], plgn2.verts[(j + 1) % plgn2.verts.size()], plgn1.verts[i % plgn1.verts.size()], plgn1.verts[(i + 1) % plgn1.verts.size()])) {
                    j = (j + 1) % plgn2.verts.size();
                    --m;
                }
                else {
                    ++i;
                    ++j;
                    --n;
                    --m;
                }
            }
        }
    }

    Polygon ans_p(ans);
    return ans_p;
};

bool CheckZero(const Polygon& p) {
    bool neg_s = false;
    Point zero(0, 0);
    for (int i = 0; i < p.s; ++i) {
        if (Space(zero, p.verts[i], zero, p.verts[(i + 1) % p.s]) < 0) {
            neg_s = true;
        }
    }

    if (!neg_s) {
        return true;
    }
    else {
        return false;
    }
};

int main() {

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int n, m;

    cin >> n;
    vector<vector<double> > f_plgn(n, vector<double>(2));
    for (int i = 0; i < n; ++i) {
        cin >> f_plgn[i][0] >> f_plgn[i][1];
    }

    cin >> m;
    vector<vector<double> > s_plgn(m, vector<double>(2));
    for (int i = 0; i < m; ++i) {
        cin >> s_plgn[i][0] >> s_plgn[i][1];
        s_plgn[i][0] *= -1;
        s_plgn[i][1] *= -1;
    }

    reverse(s_plgn.begin(), s_plgn.end());
    reverse(f_plgn.begin(), f_plgn.end());

    Polygon p1(f_plgn), p2(s_plgn), ans = MinSum(p1, p2);

    if (CheckZero(ans)) {
        cout << "YES";
    }
    else {
        cout << "NO";
    }

    return 0;
}