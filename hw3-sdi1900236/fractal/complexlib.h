typedef struct {
double real;
double imag;
} complex;

void add ( complex * a, complex * b, complex * result );
void subtract ( complex * a, complex * b, complex * result );
void multiply ( complex * a, complex * b, complex * result );
int divide ( complex * a, complex * b, complex * result );
double magnitude_squared ( complex * a );
