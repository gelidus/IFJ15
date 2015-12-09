int sqCompare(double a, double b) {
    int c;
    c = b * b;
    c = a > c;
    return c;
}

double omgBrutalniVyraz(double a, double b, double c) {
    return (a+b+a+b)*(b/a-a/b*c)*c*b/(a+c*b);
}

double sumOfPowers(double a, double b) {
    double c;
    c = b * b;
    b = a * a;
    c = c + b;
    return c;
}

double rekurDvojnasob(double a) {
    a = a*2;
    if(a>100) {
        return a;
    } else {
	double x = rekurDvojnasob(a);
	return x;
    }
}


int main() {
    double a = 8;
    double b = 33;
    double c = 1;
    double d = 4;
    int i;

    i = sqCompare(a,b);
    c = sumOfPowers(a,b);
    d = omgBrutalniVyraz(1,b,d);



//    i = sqCompare(a+a,b-c*d);
//    c = sumOfPowers(a,b);
//    d = omgBrutalniVyraz(d+a*a+1,b*a/c*(d+a-b)*(0-10),0.1);


    
    a = rekurDvojnasob(a);

    int ret = a+b+c+d+i;
    cout << ret/* == 1322*/;

    return 0;
}
