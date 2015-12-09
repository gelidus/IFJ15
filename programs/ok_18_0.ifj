//Výsledek (faktoriál): Zadejte cislo: x\nVysledek: (zaporny: -x!)

/* Program 2: Vypocet faktorialu (rekurzivne) */

int factorial(int n) //Definice funkce pro vypocet
{
	int temp_result;
	auto decremented_n = n - 1;

	if (n < 2) {
		return 1;
	} else {
		temp_result = factorial(decremented_n);
		return n * temp_result;
	}
}

int main()
{
	int a; int vysl;

	cout << "Zadejte cislo: ";
	cin >> a;

	if (a < 0) {
		cout << "Faktorial_nelze_spocitat!\n";
	}	else {
		vysl = factorial(a);
		auto neg = 0 - vysl;
		cout << "Vysledek: " << "(zaporny: " << neg << ")\n";
	}
	
	return 0;
}
