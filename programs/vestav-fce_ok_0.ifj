/* Program 3: Prace s retezci a vestavenymi funkcemi */
int main()
{
	string str1;
	{ // Vnoreny blok s lokalni promenou str2
		int x;
		str1 = "Toto je nejaky text";
		string str2;
		str2 = concat(str1, ", ktery trochu obohatime");
		x = find(str2, "text");
		cout << "Pozice retezce \"text\" v str2: "
			<< x << "\n";
		cout << "Zadejte posloupnost vsech znaku a-h"
			<< " bez opakovani: ";
	}
	cin >> str1;
    cout << str1;
	str1 = sort(str1);
    cout << str1;
	if (str1 != "abcdefgh")
	{
		for (auto s = str1; s != "abcdefgh"; s = s)
		{
			cout << "Spatne zadana posloupnost, Zkuste znovu:";
		cin >> str1;
		s = sort(str1);
		}
	}
	else {}
	return 0;
}
