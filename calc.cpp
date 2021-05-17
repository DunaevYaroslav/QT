#include <math.h>
#include <iostream>
#include "calc.h"
#include <cmath>
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

int sign(double a){
    if(a > 0)
        return 1;
    if(a < 0)
        return -1;
    return 0;
}

double f(int func_id, double x)
{
    switch (func_id) {
    case 0:
        return 1;
    case 1:
        return x;
    case 2:
        return x * x;
    case 3:
        return x * x * x;
    case 4:
        return x * x * x * x;
    case 5:
        return exp(x);
    case 6:
        return 1.0 / (25 * x * x + 1.0);
    }
    return 0;
}

double fval(int func_id, double x)
{
    switch (func_id) {
    case 0:
        return 0;
    case 1:
        return 0;
    case 2:
        return 2;
    case 3:
        return 6 * x;
    case 4:
        return 12 * x * x;
    case 5:
        return exp(x);
    case 6:
        return (5000.0 * x * x) / ((25.0 * x * x + 1) * (25.0 * x * x + 1) * (25.0 * x * x + 1)) - 50.0/((25.0 * x * x + 1) * (25.0 * x * x + 1));
    }
    return 0;
}

double pol(double *c, int i, double x, double *pts)
{
   return c[4 * i] + c[4 * i + 1] * (x - pts[i]) + c[4 * i + 2] * (x - pts[i]) * (x - pts[i]) + c[4 * i + 3] * (x - pts[i]) * (x - pts[i]) * (x - pts[i]);
}

double diff(double y1, double y2, double x1, double x2)
{
    return (y2 - y1) / (x2 - x1);
}

void fill(double *pts, double *val, double a, double b, int n, int func_id)
{
    double delta_x = (b - a) / n;

    for(int i = 0; i < n + 1; i++)
        pts[i] = a + i * delta_x;

    for(int i = 0; i < n+1; i++)
        val[i] = f(func_id, pts[i]);

    return;
}

void find_coeff(double *d, double *c, double *pts, double *val, int func_id, int n)
{
   /* for(int i = 1; i < n; i++){
                d[i] = ((pts[i + 1] - pts[i]) * diff(val[i - 1], val[i], pts[i - 1], pts[i]) + (pts[i] - pts[i - 1]) * diff(val[i], val[i + 1], pts[i], pts[i + 1])) / (pts[i + 1] - pts[i - 1]);
            
            
            d[0] = fval(func_id, pts[1]) - (fval(func_id, pts[2])-fval(func_id, pts[1]));//линейно экстраполируем
            d[n-1] = fval(func_id, pts[n-1]) + (fval(func_id, pts[n-1])-fval(func_id, pts[n-2]));

            for(i=1;i<n-1;i++){
                std::cout << d[i] << '\n';
            }
            std::cout << "________________NEXT________________";
    }
*/
    int i, j;
        double tmp1, tmp2;
        double f1, f2, f_1, f_2;

        for (i = 1; i < n - 1; i++)
        {
            tmp1 = (val[i] - val[i - 1]) / (pts[i] - pts[i - 1]);
            tmp2 = (val[i + 1] - val[i]) / (pts[i + 1] - pts[i]);
            d[i] = ((pts[i + 1] - pts[i]) * tmp1 + (pts[i] - pts[i - 1]) * tmp2) / (pts[i + 1] - pts[i - 1]);
        }
        d[0] = fval(func_id, pts[1]) - (fval(func_id, pts[2])-fval(func_id, pts[1]));//линейно экстраполируем
        d[n-1] = fval(func_id, pts[n-1]) + (fval(func_id, pts[n-1])-fval(func_id, pts[n-2]));
        tmp1 = pts[0] - (pts[1] - pts[0]);
        f1 = fval(func_id, tmp1);
        tmp2 = pts[n - 1] + (pts[n - 1] - pts[n - 2]);
        f2 = fval(func_id, tmp2);

        f_1 = (val[0] - f1) / (pts[0] - tmp1);
        f_2 = (val[1] - val[0]) / (pts[1] - pts[0]);

        f_1 = (val[n - 1] - val[n - 2]) / (pts[n - 1] - pts[n - 2]);
        f_2 = (f2 - val[n - 1]) / (tmp2 - pts[n - 1]);

        j = 0;

        for (i = 0; i < n - 1; i++)
        {
            c[j + 0] = val[i];
            c[j + 1] = d[i];
            tmp2 = pts[i + 1] - pts[i];
            tmp1 = (val[i + 1] - val[i]) / tmp2;
            c[j + 2] = (3 * tmp1 - 2 * d[i] - d[i + 1]) / tmp2;
            tmp2 *= tmp2;
            c[j + 3] = (d[i] + d[i + 1] - 2 * tmp1) / tmp2;
            j += 4;
        }
        for (i = 1;i < n;i++){
            if (fabs(d[i])>1e2){
                d[i]=d[i-1];
            }
        }
  /*  for (int i = 0; i < n; i++)
    {
        c[4 * i] = val[i];
        c[4 * i + 1] = d[i];
        c[4 * i + 2] = (3.0 * diff(val[i], val[i + 1], pts[i], pts[i + 1]) - 2.0 * d[i] - d[i + 1])/(pts[i + 1] - pts[i]);
        c[4 * i + 3] = (d[i] + d[i + 1] - 2.0 * diff(val[i], val[i + 1], pts[i], pts[i + 1])) / ((pts[i + 1] - pts[i]) * (pts[i + 1] - pts[i]));
    }*/

    return;
}

void find_extr(double *c, double *c1, double *pts, double *extr, double min_f, double max_f, int n, int func_id, double an, double bn, double eps, int type)
{
    double max_y3, min_y3, max_y2, min_y2, max_y1, min_y1, y;

    max_y1 = max_y2 = max_f;
    min_y1 = min_y2 = min_f;

    min_y3 = 0.01 * min_f;
    max_y3 = 0.01 * max_f;

    for(int i = 0; i < n; i++)
        for(double x = pts[i]; x < pts[i + 1] + eps; x += eps)
            if(x > an - eps && x < bn + eps)
            {
                y = pol(c, i, x, pts);
                if (y < min_y1)
                    min_y1 = y;
                if (y > max_y1)
                    max_y1 = y;

                y = f(func_id, x) - y;
                if (y < min_y3)
                    min_y3 = y;
                if (y > max_y3)
                    max_y3 = y;

                y = pol(c1, i, x, pts);
                if (y < min_y2)
                    min_y2 = y;
                if (y > max_y2)
                    max_y2 = y;

                y = f(func_id, x) - y;
                if (y < min_y3)
                    min_y3 = y;
                if (y > max_y3)
                    max_y3 = y;
            }

    if(type == 0){
        extr[0] = min_y1;
        extr[1] = max_y1;
    }

    if(type == 1){
        extr[0] = min_y2;
        extr[1] = max_y2;
    }

    if(type == 2){
        extr[0] = min(min_y2, min_y1);
        extr[1] = max(max_y2, max_y1);
    }

    if(type == 3){
        extr[0] = min_y3;
        extr[1] = max_y3;
    }

    return;
}

