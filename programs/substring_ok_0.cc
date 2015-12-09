//SUBSTRING KRAJNE HODNOTY



int main(){

string ret = "";
auto code = "++++[>+++++<-]>[<+++++>-]+<+[ >[>+>+<<-]++>>[<<+>>-]>>>...........";
int x = 999;
for(int i = 0; i != 999; i = i + 1){
	x = x - 1;
	
	ret = substr(code, i, x);
	//ret = substr(code, 0, 100);
	//ret = substr(code, 0, 200);  // -> vypise se vse do konce code
	/*int m = 0;
	m = m - 200;
	ret = substr(code, m, 0);*/   // -> ERR - interpretacni chyba overflow
	//ret = substr(code, 300, 0);  // -> ERR - interpretacni chyba overflow
	/*int m = 0;
	m = m - 200;
	ret = substr(code, 0, m);*/ // -> vypise se vse do konce code
	

	cout << x << ":" << i << ":" << ret << "\n";
}


return 0;
}


//tohle je nějaký text 
