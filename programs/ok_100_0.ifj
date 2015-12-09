
int sqCompare(int a, int b) {
    int c;
    c = b * b;
    c = a > c;
    return c;
}

int sumOfPowers(int a, int b) {
    int c;
    c = b * b;
    b = a * a;
    c = c + b;
    return c;
}

int rekurSoucet(int a) {
    int b;
    cin >> b;
    a = a + b;
    cout << a;
    a = rekurSoucet(a); // volat lze pouze prirazenim :D
}

int main() {
    int a;
    int b;
    int c;
    int d;

    cin >> a;
    cin >> b;

    c = sqCompare(a,b);
    cout << c;

    c = sumOfPowers(a,b);
    cout << c;

    c = sqCompare(a,b);
    cout << c;

    // potřebujeme, aby D bylo 0 a na číselné hodnoty dočasný parser se*e
    d = a != a;

    // nekonecna rekurze - zakomentujte, pokud chcete aby program končil jinak, než násilně :D
    //c = rekurSoucet(d);

    // jen povinná formalita - stejně vrátí nulu :)
    return c;
}
