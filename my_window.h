#include <QWidget>

class my_window : public QWidget
{
    Q_OBJECT

  private:
    int func_id, n, s, p, type;
    double max_f, min_f;
    double a, b, an, bn;
    double *pts;
    double *val;
    double *d;
    double *c;
    double *c1;
    double extr[2];

  public:
    my_window(QWidget *parent);
    QSize sizeHint() const;
    int parse_command_line(int argc, char *argv[]);

  public slots:
    void next_func();
    void change_type();
    void double_x();
    void half_x();
    void double_n();
    void half_n();
    void double_p();
    void half_p();

  protected:
    void update_n();
    void update_f();
    void update_s();
    void update_coeff();
    void update_dots();
    void paintEvent(QPaintEvent *event);
};

