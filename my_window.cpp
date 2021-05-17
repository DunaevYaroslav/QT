#include <QPainter>
#include <math.h>
#include <stdlib.h>

#include "my_window.h"
#include "calc.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

QSize my_window::sizeHint() const
{
    return QSize(1960, 940);
}

my_window::my_window(QWidget *parent) : QWidget(parent)
{
    func_id = 0;
    s = 0;
    p = 0;
    type = 0;
    a = -5;
    b = 5;
    n = 10;

    max_f = min_f = 0;

    an = (a+b)/2 - (b-a)/pow(2,s+1);
    bn = (a+b)/2 + (b-a)/pow(2,s+1);

    c1 = new double[4 * n];
    pts = new double[n + 1];
    val = new double[n + 1];
    d = new double[n + 1];
    c = new double[4 * n];
    extr[0] = extr[1] = 0;

    update_dots();
    update_coeff();
}

void my_window::next_func()
{
    func_id = (func_id + 1) % 7;

    update_dots();
    update_coeff();
    update_f();
    update();
}

int my_window::parse_command_line(int argc, char *argv[])
{
    if (argc != 4)
        return -1;

    sscanf(argv[1], "%lf", &a);
    sscanf(argv[2], "%lf", &b);
    sscanf(argv[3], "%d", &n);

    if (b - a < 1e-30 || n <= 0)
        return -1;

    update_n();
    update_s();
    update_dots();
    update_coeff();

    return 0;
}

void my_window::change_type()
{
    type = (type + 1) % 4;

    update_dots();
    update();
}

void my_window::half_x()
{
    if(s != 0)
        s--;

    update_s();
    update();
}

void my_window::double_x()
{
    s++;

    update_s();
    update();
}


void my_window::double_n()
{
    if(2 * n<1e06)
        n *= 2;

    update_n();
    update_dots();
    update_coeff();
    update();
}

void my_window::half_n()
{
    if(n > 6)
        n /= 2;

    update_n();
    update_dots();
    update_coeff();
    update();
}

void my_window::double_p()
{
    p++;

    update_dots();
    update_coeff();
    update();
}

void my_window::half_p()
{
    p--;

    update_dots();
    update_coeff();
    update();
}

void my_window::update_f()
{
    double y;
    double eps = (b-a)/1000;
    min_f = max_f = 0;

    for(double x = an; x < bn + eps; x += eps)
    {
        y = f(func_id, x);
        if (y < min_f)
            min_f = y;
        if (y > max_f)
            max_f = y;
    }
}

void my_window::update_n()
{
    delete d;
    delete c;
    delete c1;
    delete pts;
    delete val;

    d = new double[n + 1];
    c = new double[4 * n];
    c1 = new double[4 * n];
    pts = new double[n + 1];
    val = new double[n + 1];
}

void my_window::update_s()
{
    an = (a+b)/2 - (b-a)/pow(2,s+1);
    bn = (a+b)/2 + (b-a)/pow(2,s+1);

    update_f();
}

void my_window::update_dots()
{
    fill(pts, val, a, b, n, func_id);
    val[n / 2] += p * 0.1 * max(fabs(max_f), fabs(min_f));
}

void my_window::update_coeff()
{
    find_coeff(d, c, pts, val, func_id, n);
}

void my_window::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QPen pen("white");

    QBrush brush(Qt::white);

    double eps = (b-a)/max(width()-1, 1000);
    char num[100], as[100], bs[100], ymins[100], ymaxs[100], fmins[100], fmaxs[100], ss[100], ps[100];

    find_extr(c, c1, pts, extr, min_f, max_f, n, func_id, an, bn, eps, type);

    extr[0] -= 0.01 * (extr[1] - extr[0]);
    extr[1] += 0.01 * (extr[1] - extr[0]);

    painter.save();

    painter.translate(0.5 * width(), 0.5 * height());
    painter.scale(width() / (bn - an), -height() / (extr[1] - extr[0]));
    painter.translate(-0.5 * (an + bn), -0.5 * (extr[1] + extr[0]));

    painter.setPen(pen);

    pen.setWidth(0);

    if(type != 3)
    {
        pen.setColor("orange");
        painter.setPen(pen);

        for(double x = a + eps; x < b + eps; x += eps)
            painter.drawLine(QPointF(x - eps, f(func_id, x - eps)), QPointF(x, f(func_id, x)));
    }

    if(type == 0 || type == 2)
    {
        pen.setColor("yellow");
        painter.setPen(pen);

        for (int i = 0; i < n; i++)
            for(double x = pts[i]; x < pts[i + 1] + eps; x += eps)
                painter.drawLine(QPointF(x - eps, pol(c, i, x - eps, pts)), QPointF(x, pol(c, i, x, pts)));
    }

    if(type == 1 || type == 2)
    {
        pen.setColor("green");
        painter.setPen(pen);

        for (int i = 0; i < n; i++)
            for(double x = pts[i]; x < pts[i + 1] + eps; x += eps)
                painter.drawLine(QPointF(x - eps, pol(c1, i, x - eps, pts)), QPointF(x, pol(c1, i, x, pts)));
    }

    if(type == 3)
    {
        pen.setColor("orange");
        painter.setPen(pen);

        for (int i = 0; i < n; i++)
            for(double x = pts[i]; x < pts[i + 1] + eps; x += eps)
                painter.drawLine(QPointF(x - eps, f(func_id, x - eps) - pol(c, i, x - eps, pts)), QPointF(x, f(func_id, x) - pol(c, i, x, pts)));

        pen.setColor("green");
        painter.setPen(pen);

        for (int i = 0; i < n; i ++)
            for(double x = pts[i]; x < pts[i + 1] + eps; x += eps)
                painter.drawLine(QPointF(x - eps, f(func_id, x - eps) - pol(c1, i, x - eps, pts)), QPointF(x, f(func_id, x) - pol(c1, i, x, pts)));
    }

    pen.setColor("black");
    painter.setPen(pen);

    for(int u = 1; u < b; u++)
        painter.drawLine(QPointF(u, 0.5 * 0.01 * (extr[1] - extr[0])), QPointF(u, -0.01 * (extr[1] - extr[0])));
    for(int u = -1; u > a; u--)
        painter.drawLine(QPointF(u, 0.5 * 0.01 * (extr[1] - extr[0])), QPointF(u, -0.01 * (extr[1] - extr[0])));

    painter.drawLine(QPointF(0.5 * 0.01 * (bn - an), 1), QPointF(-0.5 * 0.01 * (bn - an), 1));
    painter.drawLine(QPointF(a, 0), QPointF(b, 0));
    painter.drawLine(QPointF(0, extr[0]), QPointF(0, extr[1]));

    painter.restore();

    painter.setPen("orange");

    sprintf(as, "a = %.3e", a);
    sprintf(bs, "b = %.3e", b);
    sprintf(ss, "s = %d", s);
    sprintf(num, "n = %d", n);
    sprintf(fmins, "f_min = %.3e", min_f);
    sprintf(fmaxs, "f_max = %.3e", max_f);
    sprintf(ymins, "y_min = %.3e", extr[0]);
    sprintf(ymaxs, "y_max = %.3e", extr[1]);
    sprintf(ps, "p = %d", p);

    painter.drawText(20, 40, num);
    painter.drawText(20, 60, as);
    painter.drawText(20, 80, bs);
    painter.drawText(20, 100, ymins);
    painter.drawText(20, 120, ymaxs);
    painter.drawText(20, 140, fmins);
    painter.drawText(20, 160, fmaxs);
    painter.drawText(20, 180, ps);

    switch (func_id) {
    case 0:
        painter.drawText(20, 20, "f(x) = 1");
        break;
    case 1:
        painter.drawText(20, 20, "f(x) = x");
        break;
    case 2:
        painter.drawText(20, 20, "f(x) = x*x");
        break;
    case 3:
        painter.drawText(20, 20, "f(x) = x*x*x");
        break;
    case 4:
        painter.drawText(20, 20, "f(x) = x*x*x*x");
        break;
    case 5:
        painter.drawText(20, 20, "f(x) = exp(x)");
        break;
    case 6:
        painter.drawText(20, 20, "f(x) = 1.0/(25*x*x+1.0)");
        break;
    }
        painter.drawText(20, 200, "type 21");
}
