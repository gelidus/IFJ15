// Výstup: 60

/*#include <iostream>
using namespace std;*/

int main()
{

	int i = (1 == 1) +  (0 != 1) + (1 < 2) + (2 > 1) + (1 <= 2) + (2 >= 1);  // 6
	int x = (0 == 5) +  (9 != 9) + (2 < 1) + (1 > 2) + (2 <= 1) + (1 >= 2);	// 0

cout << i;
cout << x;

return x;
}
