int sign(double a);

double f(int func_id, double x);
double fval(int func_id, double x);
double pol(double *c, int i, double x, double *pts);
double diff(double y1, double y2, double x1, double x2);
void fill(double *pts, double *val, double a, double b, int n, int func_id);
void find_coeff(double *d, double *c, double *pts, double *val, int func_id, int n);
void find_extr(double *c, double *c1, double *pts, double *extr, double min_f, double max_f, int n, int func_id, double an, double bn, double eps, int type);
