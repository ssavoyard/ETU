#include <stdio.h>
#include <math.h>
#include <float.h>

int main() {
double d = pow(10, -5);
double a = 0, f = 0, b = DBL_MAX, a1 = -DBL_MAX;
while ((b - a)> d) {
double c = (a + b) / 2;
if (c + 2 - exp(c) == 0) {
printf("%f\n", c);
} else if ((c + 2 - exp(c)) * (a + 2 - exp(a)) < 0) {
b = c;
} else {
a = c;
}

}
printf("%f\n", (a + b) / 2);
while ((f - a1) > d) {
double c1 = (a1 + f) / 2;
if (c1 + 2 - exp(c1) == 0) {
printf("%f\n", c1);
} else if ((c1 + 2 - exp(c1)) * (f + 2 - exp(f)) < 0) {
a1 = c1;
} else {
f = c1;
}

}
printf("%f\n", (f + a1) / 2);
return 0;
}
