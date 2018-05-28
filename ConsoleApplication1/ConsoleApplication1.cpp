
#include "stdafx.h"
#include "string.h"
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

//wezel 
struct Node {
	int count; //liczebnosc znaku
	char znak; 
	Node *left=NULL; //lewy potomek
	Node *right=NULL; //prawy potomek
	char * kod; //kod znaku
	int dl;		//dlugosc kodu

	//konstruktor
	Node(int c, char z)
	{
		count = c;
		znak = z;
		kod = "";
		dl = 0;
	}
};


//element listy
struct ElemL {
	Node *n; //wezel (poddrzewo)
	ElemL *next=NULL; //nastepny
	ElemL *prev=NULL; // poprzedni 

	//konstruktor
	ElemL(Node * a)
	{
		n = a;
	}

	//konstruktor
	ElemL(int x, char a) // x to czestotliwosc  a to przechowywany znak
	{
		n = new Node(x, a);
	}
};

//lista
struct List {
	int ktr=0; //czy dopiero wsadzamy na liste elementy (0) czy ju¿ sie bawimy jak Huffman nakazal (1)
	ElemL *first; //pierwszy element listy
	ElemL *last; // ostatni element listy - tez nie wiem czy potrzebny 

	///wrzucanie na liste
	void put(ElemL *e)
	{
		ElemL *p = first;
		if (p == NULL) //pusta lista, e pierwszym elementem
		{
			first = e;
			last = e;
		}
		else
		{
			if (e->n->count <= p->n->count || (first->next == NULL && ktr == 1)) // gdy nowy jest mniejszy od pierwszego lub zosta³y nam tylko dwa poddrzewa i trzeba je scalic (zostaly, bo ktr jest rowny 1 dopiero zaraz przed rozpoczeciem dzialania algorytmu hoffmana, a nie w trakcie wsadzania na pusta liste elementow, kiedy to z tego wynika³bym blad gdy mamy na lisie jeden element i wkladamy drugi, ktory jest wiekszy, bo wskoczy nam przed)
			{
				first->prev = e;
				e->next = first;
				first = e;
				return;
			}
			else
				p = p->next;
			if (p == NULL) //mamy liste tylko z jednego elementu mniejszego niz ten, ktory chcemy dodac
			{
				first->next = e;
				e->prev = first;
				last = e;
				return;
			}
			while (p != NULL) //mamy co najmniej dwa elementy w liscie i wkladamy kolejny 
				if (e->n->count > p->n->count)
					p = p->next;
				else
				{
					p->prev->next = e;
					e->prev = p->prev;
					p->prev = e;
					e->next = p;
					return;
				}
			//nowy element ma najwieksza liczebnosc i trzeba go dac na koniec
			last->next = e;
			e->prev = last;
			last = e;
		}
	}


	//usuwanie z listy
	void del(ElemL *e)
	{
		ElemL *p = first;
		if (e == first)//usuwanie pierwszego, w sumie tylko z tego korzystamy (elsa mozna wyjebac ale mamy uniwersalne usuwanie)
		{
			if (first->next != NULL)
				first->next->prev = NULL;
			first = first->next;
		}
		else // usuwanie ktoregosc kolejnego
		{
			while (p != last)
				if (e == p)
				{
					p->next->prev = p->prev;
					p->prev->next = p->next;
					return;
				}
				else
					p = p->next;
			//usuwanie ostatniego
			last->prev->next = NULL;
			last = last->prev;
		}
	}

	//wydruk listy
	void print()
	{
		ElemL *p = first;
		while (p != NULL)
		{
			if (p->n->znak== 10)
				printf("%d(/)", p->n->count); //znak nowej linii
			else
				printf("%d(%c)", p->n->count, p->n->znak);
			p = p->next;
		}
		printf("\n");
	}

};

//element kolejki
struct Point
{
	int count; //liczebnosc znaku
	char znak; //znak
	char *kod; // kod znaku
	int dl; //dlugosc kodu
	Point *next; //nastepny
	Point *prev; // poprzedni - tez nie wiem czy potrzebne (raczej wystarcza listy jedno-wiazane) 

	//konstruktor
	Point(int x, char z)
	{
		count = x;
		znak = z;
		next = NULL;
		prev = NULL;
		dl = 0;
		kod = "";
	}
	//konstruktor
	Point(char z, char *k, int d)
	{
		znak = z;
		next = NULL;
		prev = NULL;
		kod = k;
		dl = d;
	}
};

// kolejka
struct Queue
{
	Point *first = NULL; //pierwszy
	Point *last = NULL; //ostatni

	//konstruktor
	Queue()
	{
		first = NULL;
		last = NULL;
	}

	//dodaje do kolejki nowy element, jesli taki istnieje (tzn znak mamy juz w kolejce) to zwieksza jego wartosc
	void add(Point *e)
	{
		Point *p = first;
		if (p == NULL) //jesli kolejka pusta i wkladamy pierwszy element
		{
			first = e;
			last = e;
		}
		else
		{
			while (p != NULL)
				if (p->znak != e->znak) //jesli nie napotkamy naszego znaku to kolejny
					p = p->next;
				else //jesli napotkamy to zwiekszamy jego liczebnosc
				{
					p->count++; 
					return;
				}
			//naszego znaku nie ma jeszcze w kolejce, pierwszy raz dana litera wchodzi do kolejki
			last->next = e;
			e->prev = last;
			last = e;
		}
	}

	//wyrzuca wezel, ktory trzyma odpowedni znak, potem z tego mozna sciagnac wiecej informacji
	Point * kod(char z)
	{
		Point *p = first;
		while (p != NULL)
		{
			if (p->znak == z)
				return p;
			else
				p = p->next;
		}
		return NULL; //jesli takiego nie ma to null
	}

	//wrzuca elementy z kolejki na liste
	void wsadz(List *lista)
	{
		Point *p = first;

		while (p != NULL)
		{
			ElemL *a = new ElemL(p->count, p->znak);
			lista->put(a);
			p = p->next;
		}
	}

	//drukuje kolejke
	void print()
	{
		Point *p = first;
		while (p != NULL)
		{
			if(p->znak == 10)
				printf("%d(/)", p->count); //znak nowej linii
			else
				printf("%d(%c)", p->count, p->znak);
			p = p->next;
		}
		printf("\n");
	}
};

	//max z dwu liczba
	int max(int a, int b)
	{
		if (a>b)
			return a;
		else
			return b;
	}

	//algorytm huffmana
	void fHuffmana(List *lista)
	{
		ElemL *p = lista->first;
		while (p->next != NULL)
		{
			lista->print(); // drukuje liste (podglad na dzialanie)
			Node *n = new Node(p->n->count + p->next->n->count,'$'); //nowy wierzcholek o liczebnosci = suma z dwoch najmniejszych elementow listy
			n->left = p->n; //pierwszy element list lewym synem
			n->right = p->next->n; //drugi element listy prawym synem
			lista->del(lista->first); //usuwa pierwszy
			lista->del(lista->first); //usuwa drugi (jak usunal wczesniej pierwszy to drugi staje sie pierwszym)
			ElemL *fn = new ElemL(n); //Element listy, ktory ma w sobie nasz nowy wezel
			lista->put(fn); //wrzucenie na nasza liste
			p = lista->first; //znowu lecimy od poczatku (od najmniejszych)
		}
	}

	//przechodzenie przez drzewo, wrzucanie do kolejki i do pliku odpowiednich znakow z kodowaniem
	//Przechodzenie Preorder = Korzen -> Lewy -> Prawy
	void KLP(Node *r, FILE *plik, Queue *q, int wys)
	{
		wys++; //wysokosc wierzchola - (korzen ma wysokosc 0)
			if (r->left != NULL)
			{
				//kodowanie
				int h = wys ;
				r->left->kod = (char*)malloc(h + 1); //przydzielenie pamieci
				for (int i = 0; i < h- 1; i++)
					r->left->kod[i] = r->kod[i]; //bierze kod rodzica
				r->left->kod[h - 1] = '0'; //i dodaje na koncu 0, bo poszlismy w lewo
				r->left->kod[h] = '\0'; //ostatni znak

				r->left->dl = h; //dlugosc kodu

				//jesli nie jest wezlem laczocym to wpisz litere i jej kod do pliku i wsadz do kolejki zaszyfrowanych znakow
				if (r->left->znak != '$')
				{
					if(r->left->znak == 10) //zeby nie przechodzilo do nowej lini w pliku (zeby bylo ladniej)
						fprintf(plik, "/ : "); //znak nowej linii
					else
						fprintf(plik,"%c : ", r->left->znak);
					for (int i = 0; i < h + 1; i++)
					{
						fprintf(plik,"%c", r->left->kod[i]);
					}
					fprintf(plik, "\n");

					Point *p = new Point(r->left->znak, r->left->kod, h);
					q->add(p);
				}
				//rekurencja
				KLP(r->left, plik,q,wys);
			}
			if (r->right != NULL)
			{
				//kodowanie
				int h = wys;
				r->right->kod = (char*)malloc(h + 1); //przydzielenie pamieci
				for (int i = 0; i < h - 1; i++)
					r->right->kod[i] = r->kod[i]; //bierze kod rodzica
				r->right->kod[h - 1] = '1'; //i dodaje 1, bo idziemy w prawo
				r->right->kod[h] = '\0'; //ostatni znak

				r->right->dl = h; //dlugosc kodu

				//jesli nie jest wezlem laczocym to wpisz litere i jej kod do pliku i wsadz do kolejki zaszyfrowanych znakow
				if (r->right->znak != '$')
				{
					if (r->right->znak == 10) //zeby nie przechodzilo do nowej lini w pliku (zeby bylo ladniej)
						fprintf(plik, "/ : "); //znak nowej linii
					else
						fprintf(plik,"%c : ", r->right->znak);
					for (int i = 0; i < h + 1; i++)
					{
						fprintf(plik,"%c", r->right->kod[i]);
					}
					fprintf(plik, "\n");

					Point *p = new Point(r->right->znak, r->right->kod, h);
					q->add(p);
				}
				//rekurencja
				KLP(r->right, plik,q,wys);
			}
	}


int main()
{
	List *lista = new List(); //lista na ktorej bedziemy sie bawic
	FILE * plik = fopen("plik.txt", "r"); //plik do odczytu
	FILE *zapis = fopen("szyfr.txt", "w"); //plik do zapisu
	int number_letter = NULL;

	Queue *q = new Queue; //pierwsza kolejka do odczytywania i uporzadkowania danych z pliku
	//wyczytanie z pliku

	while (number_letter != EOF)
	{
		char letter = getc(plik); // kolejny znak
		Point *p = new Point(1, letter); //nowy punkt kolejki (na razie z liczebnoscia 1)
		q->add(p); //dodajemy go do kolejki
		number_letter = letter;
	}
	fclose(plik); //zamykamy plik 
	q->wsadz(lista); //wrzucamy elementy z kolejki na liste
	lista->ktr = 1; //zmiana ktora byla opisana wyzej
	fHuffmana(lista); //zabawa Huffmana
	lista->print(); //wydruk kontrolny (podglad)
	Queue *sz = new Queue; //nowa kolejka w ktorej beda przechowywane litery i ich kodowania
	//zapisanie kodow dla liter w pliku szyfr
	KLP(lista->first->n,zapis,sz,0); //opis funkcji przy jej implementacji
	fclose(zapis); //zamkniecie pliku z zapisanymi literami i ich kodowaniem

	  ////////////////////////////
	 //////Kodowanie tekstu//////
	////////////////////////////
	plik = fopen("plik.txt", "r"); //plik z tekstem
	zapis = fopen("kod.txt", "w"); //plik do zapisu kodu
	number_letter = NULL;
	//zapisanie kodu z pliku kod (zaszyfrowane zdania z plik.txt)
	while (number_letter != EOF)
	{
		char letter = getc(plik); //bierze kolejna litere
		Point *p = sz->kod(letter); //punkt ktory odpowiada danej litrze w kolejce
		for (int i = 0; i < p->dl; i++)
			fprintf(zapis, "%c", p->kod[i]); //wyciaga z punktu kodowani i wrzuca do pliku
		number_letter = letter;
	}

	fclose(zapis); //zamkniecie pliku do zapisu
	fclose(plik); //zamkniecie pliku do odczytu

	  //////////////////////////////
	 ////Deszyfrowanie kodu ///////
	//////////////////////////////
	plik = fopen("kod.txt", "r"); //plik do odczytu
	zapis = fopen("deszyfr.txt", "w"); //pilk do zapisu
	number_letter = NULL; 
	Node *n = lista->first->n; //pierwszy wezel - korzen drzwa, pomoze do odczytu kodu
	while (number_letter != EOF)
	{
		char letter = getc(plik); //kolejny znak kodu
		if (letter == '1') //jesli 1 to lecimy w prawo
			n = n->right;
		else //jesli 0 to w lewo
			n = n->left;
		if (n->znak != '$') //jesli wezel jest nie jest wezlem laczocym to pobierz znak z wezla i wpisz go do pliku
		{
			fprintf(zapis, "%c", n->znak);
			n = lista->first->n; // lecimy od poczatku drzewa z nowym kodem
		}
		number_letter = letter;
	}

	fclose(zapis); //zapis pliku
	fclose(plik); //zapis pliku

	int x; //zebym mogl przeczytac to co mi wyrzucilo w konsoli
	cin >> x; //pozdrawiam
	
    return 0;
}

