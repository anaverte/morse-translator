/*
Program wykorzystuje następujące przełączniki:
−i plik_wejsciowy		nazwa pliku wejściowego
−o plik_wyjsciowy		nazwa pliku wyjściowego
-k strona_tłumaczenia	strona tłumaczenia
Przełączniki mogą być podane w dowolnej kolejności.

2017-12-08
*/

#include <fstream>
#include <iostream>
#include <string>


using namespace std;

//tablice znaków, które biorą udział w tłumaczeniu
string znaki = "abcdefghijklmnoprstuvwxyz";
string ZNAKI = "ABCDEFGHIJKLMNOPRSTUVWXYZ";
string morse[25] = { ".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.",".-.","...","-","..-","...-",".--","-..-","-.--","--.." };

//zmienne odpowiadające przełącznikom
string pi = "-i", pk = "-k", po = "-o";

//zmienne odpowiadające za stronę tłumaczenia oraz przechowujące nazwy plików
string nm = "l2m", zm = "m2l", p_wej, p_wyj;

//zmienna wskazująca, w którą stronę będzie przeprowadzone tłumaczenie
bool strona;

//funkcja wyświetlająca pomoc po dostarczeniu nieprawidłowych danych
void pomoc() {
	cerr << "Wprowadzono nieprawidlowe dane" << endl <<
		"Upewnij sie, ze dane podawane sa we wlasciwy sposob:" << endl <<
		"-i plik_wejsciowy" << endl << "-o plik_wyjsciowy" << endl <<
		"-k kierunek_tlumaczenia, przy czym l2m oznacza tlumaczenie z alfabetu" << endl <<
		"lacinskiego na alfabet Morse'a, a m2l odwrotnie. " << endl <<endl<<
		"Przykladowe wywolanie programu: program.exe -o wyjscie -k m2l -i wejscie." << endl <<endl<<
		"Uwaga: w niniejszym tlumaczu kropce odpowiada znak '.', a kresce '-'." << endl <<
		"Po zakonczeniu litery nalezy wstawic znak '/', a wyrazu '//'"<<endl<<endl;
}

int main(int argc, char * argv[])//arg c odpowiada za liczbę przekazywanych argumentów
{
	//warunek wyświetlający informację o błędach po podaniu niewłaściwej liczby argumentów
	if (argc != 7)
	{
		cerr <<endl<< "Podano zle wartosci, sprobuj ponownie."<<endl;
		pomoc();
		exit(1);
	}

	//pętla sprawdzająca, czy po żadnym przełączniku nie został zapisany przełącznik- w wypadku błędu wyświetla pomoc
	//W warunkach i++ pozwala ominąć powtórzenie sprawdzające wartości bezpośrednio po przełączniku.
	for (int i = 1; i < (argc - 1); i++)
	{
		if (argv[i] == pk && argv[i + 1] == nm)//warunek informujący, czy zostało wybrane tłumaczenie na alfabet Morse'a
		{
			strona = true;i++;
		}
		else {
			if (argv[i] == pk && argv[i + 1] == zm)
			{
				strona = false;i++;
			}
			else {
				if (argv[i + 1] != pi&&argv[i + 1] != pk&&argv[i] == po)
				{
					p_wyj = argv[i + 1];i++;
				}
				else {
					if (argv[i] == pi&&argv[i + 1] != pk&&argv[i + 1] != po)
					{
						p_wej = argv[i + 1];i++;
					}
					else {
						cerr << "Podano zle wartosci, sprobuj ponownie." << endl;pomoc();exit(1);
					}
				}
			}
		}
	}

	//otwieranie pliku wejściowego
	ifstream plik_wejsciowy;
	plik_wejsciowy.open(p_wej);
	if (!plik_wejsciowy.is_open())//komunikat o błędzie, jeśli plik nie zostanie otwarty
	{
		cerr << "Otwieranie pliku nie powiodlo sie." << endl; {pomoc();exit(1);}
	}

	//otwieranie pliku wyjsciowego
	ofstream plik_wyjsciowy;
	plik_wyjsciowy.open(p_wyj);
	if (!plik_wyjsciowy.is_open())//komunikat o błędzie
		pomoc();

	//Tłumaczenie z alfabetu łacińskiego na Morse'a
	
	string slowo;//zmienna przechowująca aktualne słowo
	bool p=0;//pomocnicza zmienna wskazująca, czy dany znak znalazł swój odpowiednik
	int licz = 0;//zmienna licząca ile słów pobrano z pliku
	if (strona == true) {//warunek rozpoczęcia tłumaczenia
		while (plik_wejsciowy >> slowo)//pętla pobierająca słowa z pliku
		{
			licz++;
			int k = slowo.length();//zmienna przechowująca długość słowa
			for (int i = 0; i < k; i++)//pętla analizująca każdy znak słowa
			{
				p = 0;
				for(int j=0;j<25;j++)// pętla szukająca właściwego znaku
				{
					if (slowo[i] == ZNAKI[j] || slowo[i] == znaki[j])//warunek wpisujący do pliku znaki odpowiadające pobranej literze
					{
						plik_wyjsciowy << morse[j] << "/";
						 p = 1;
					}	
					
				}
				if (p == 0) {
					plik_wyjsciowy.close();
					plik_wyjsciowy.open(p_wyj);
					plik_wyjsciowy << "";
					plik_wyjsciowy.close();

					cerr << "Podano zly znak. "; pomoc();exit(1); }
			}plik_wyjsciowy << "/";//rozdzielenie poszczególnych liter

		}if(licz==0){ cerr << "Podany plik jest pusty. ";pomoc();exit(1); }
	}

	// Tłumaczenie z alfabetu Morse'a na łaciński

	string l="";//zmienna przechowująca pobraną linię
	string litera="";//zmienna przechowująca aktualną literę
	int licznik = 0;//licznik znaków /
	if (strona == false)
	{//warunek rozpoczęcia tłumaczenia
		while (getline(plik_wejsciowy, l))
		{ //wczytywanie linii
		int k = l.length();//sprawdzenie długości linii
		for (int i = 0; i < k; i++)//analiza po znaku
			{if (!(isspace(l[i])))//znaki białe są ignorowane
				{
					if (l[i] == '/') { licznik++; }
					if (l[i] !='/')//składanie znaków w literę(morsową)
					{
						litera += l[i];
					}
					else
					{	for (int j = 0; j < 25; j++)//pętla szukająca właściwej litery
						{
							if (litera == morse[j])//warunek wpisujący do pliku literę odpowiadającą pobranym znakom
							{
								plik_wyjsciowy << znaki[j];
								p = 1;
							}
						}
					if (p == 0) { cerr << "Podano zly znak. " << endl; pomoc();exit(1); }//jeśli znaki nie mają swojego odpowiednika, program jest zamykany po wyświetleniu stosownego komunikatu
						if (l[i + 1] == '/')//jeśli występuje //, tworzona jest spacja
						{
							plik_wyjsciowy << " ";
						}
						litera = "";//przygotowanie zmiennej na nowe znaki
					}
				}
			}
		}
		if(licznik==0){ cerr << "Podany plik jest pusty. ";pomoc();exit(1); }//nie znaleziono żadnej litery
	}	
	
	//zamknięcie plików
	plik_wejsciowy.close();
	plik_wyjsciowy.close();
	system("pause");
}
