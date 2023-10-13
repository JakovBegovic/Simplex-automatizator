#include <iostream>
#include <cstring>
#include <vector>
#include <limits.h>
#include "Razlomci.h"

using namespace std;

char koraci;
vector<int> fCilja;
vector<vector<int>> fOgranicenja; // fOgranicenja[red][stupac]
int brStrukt, brOgran, odabir; // broj strukturnih varijabli, broj funkcija ogranicenja
				// i odabir izmedju maks i min problema (maks = 1 , min = 2)
			
bool vec_stvorena = false; // za listaBv
bool M_stvoreno = false; // za M
bool preveden = false; // jesu li fOgranicejna i fCija prevedene u dual?
bool uneseno = false; // jesu li vriejdnosti unesene?
	
int brRedaka;
int brStupaca; // dimenzije simpleksice


vector<int> listaBv; // lista bazicnih varijabli
vector<vector<Razlomak>> M;
	
void Unos(){
	int unos;

	preveden = false;

	//unos
	cout << "Koliko strukturnih varijabli zelis: ";
	cin >> brStrukt;
	
	cout << "Koliko funkcija ogranicenja zelis: ";
	cin >> brOgran;
	
	fCilja.clear();
	
	for(int i=0; i<brStrukt; i++){
		cout << "Unesi cjelobrojni koeficjent uz x" << i+1 << " u funkciji cilja: ";
		cin >> unos;
		fCilja.push_back(unos);
	}
	
	fOgranicenja.clear();
	int j=0;
	for(int i=0; i<brOgran; i++){
		vector<int> red;
		
		for(j=0; j<brStrukt; j++){
			cout << "Unesi cjelobrojni koeficjent uz x" << j+1 << " u funkciji "<< i+1 <<". ogranicenja: "; 
			cin >> unos;
			red.push_back(unos);
		}
		
		cout << "Unesi vrijednost slobodnog koeficjenta u funkciji "<< i+1 <<". ogranicenja: ";
		cin >> unos;
		red.push_back(unos);
		
		fOgranicenja.push_back(red);
		

	}	
		
}

void Ispis(){
	
	int brojilo;
	brojilo = 1 ;
	
    cout << "Funkcija cilja: " << endl;
    
    for (int i = 0; i < fCilja.size(); i++){
    	cout << fCilja[i] <<"*x" << brojilo;
    	if(brojilo<brStrukt){
    		cout << " + ";
		}
        brojilo++;
	}
    cout << " = Z";
    
    if(odabir==1){
    	cout << " -> max" << endl;
	} else {
    	cout << " -> min" << endl;
		
	}
    
    brojilo=1;
    int brojilo2;
    brojilo2 = 1;
    
    cout << "Funkcije ogranicenja:" << endl;
    for (int i = 0; i < fOgranicenja.size(); i++){
    	cout << "P" << brojilo << ": ";
    	brojilo++;
    	for (int j = 0; j < fOgranicenja[i].size(); j++){
    		
    		if(brojilo2>1 && brojilo2<(brStrukt+1) && (fOgranicenja[i][j])>=0){
    			cout << " + "<< fOgranicenja[i][j] << "*x" << brojilo2;
    			
			}else if(brojilo2>1 && brojilo2<(brStrukt+1) && (fOgranicenja[i][j])<0){
				cout << " "<< fOgranicenja[i][j] << "*x" << brojilo2;
				
			}else if(brojilo2==(brStrukt+1)){
				cout << " "<< fOgranicenja[i][j];
				
			}else{
    			cout << fOgranicenja[i][j]<< "*x" << brojilo2;
			}
    		
			if(brojilo2==brStrukt){
				if(odabir==1){
					cout << " <=";
				} else if(odabir==2){
					cout << " >=";
				}
				
			}
    	
			
    		brojilo2++;
		}
		brojilo2=1;
		cout << endl;
	}
	
	for(int i=0; i<brStrukt; i++){
		cout << "x" << i+1 << ", ";
	}
	
	for(int i=0; i<brOgran-1; i++){
	
		cout << "u" << i+1 << ", ";

	}
	
	cout << "u" << brOgran;
	
	cout << " >= 0" << endl;
}

void ispisSimpleks(vector<vector<Razlomak>>& M){
	
	for( int i=0; i<(M.size()); i++){
		
		for( int j=0; j<(M[i].size()); j++){
		//	cout <<"Na (" <<i << ", " << j << "): " <<M[i][j] << endl;
			cout << M[i][j] << " ";
		}
		cout << endl;
	}
	
	for(int i=0; i<listaBv.size(); i++){
		cout << listaBv[i] << " ";
	}
	
	cout << endl;
}

void stvoriMatricuMax(vector<vector<Razlomak>>& M){
	
	if(!vec_stvorena){
		for(int i=0; i<brOgran; i++){
			
			listaBv.push_back(brStrukt+i); // podaci koje su varijable bazicne 
					// predstavljaju njihov indeks stupca u retku
			
		}
		vec_stvorena = true;
		
	}else{
		listaBv.clear();
		
		for(int i=0; i<brOgran; i++){
			
			listaBv.push_back(brStrukt+i); // podaci koje su varijable bazicne 
					// predstavljaju njihov indeks stupca u retku
			
		}
	}
	 
	
	Razlomak priv(0, 1);
	
	// 1. korak - popunjavanje sa koeficjentima strukturnih od (0,0) do (brRedaka-2, brStrukt-1)
	
	
	for( int i=0; i<(brRedaka-1); i++){
		
		for( int j=0; j<brStrukt; j++){
			Razlomak priv(fOgranicenja[i][j], 1);
			M[i][j] = priv;
		}
	}

	// 2. korak - popunjavanje sa jedinicnom matricom od dimenzija (0, brStrukt) do (brRedaka-2, brStupaca-2)
	priv.brojnik=1;
	priv.nazivnik=1;
	int brojnik=0;
	
		
	for(int j=brStrukt; j<(brStupaca-1); j++){
		
		
		M[brojnik][j]=priv;
		
		brojnik++;
	}
	
	// 3. korak - popunjavanje zadnjeg retka
	
	for(int i=0; i<brStrukt; i++){
		Razlomak priv(-fCilja[i], 1);
		M[brRedaka-1][i]=priv;
	}
	
	// 4. korak - popunjavanje zadnjeg stupca
	for(int i=0; i<brOgran; i++){
		Razlomak priv(fOgranicenja[i][brStrukt], 1);
		M[i][brStrukt + brOgran] = priv;
	}
	
}

void ispisFinal(vector<vector<Razlomak>>& M, vector<int>& listaBv){
	
	if(odabir==1){
		
	
		cout << "Vrijednosti za optimalno rjesenje su: " << endl;
		bool nadjeno = false;
		
		for(int i=0; i<(brStrukt+brOgran); i++){
			if(i<brStrukt){
				cout << "x" << i+1 << " = ";
				nadjeno = false;
				
				for(int j=0; j<listaBv.size(); j++){
					
					if(listaBv[j] == i){
						cout << M[j][brStupaca-1] << endl;
						nadjeno = true;
	
					}
				}
				
				if(!nadjeno){
					cout << 0 << endl;
				}
				
			} else{
				cout << "u" << i+1-brStrukt << " = ";
				nadjeno = false;
				for(int j=0; j<listaBv.size(); j++){
				
					if(listaBv[j] == i){
						cout << M[j][brStupaca-1] << endl;
						nadjeno = true;
	
					}
				}
				if(!nadjeno){
					cout << 0 << endl;
				}
			}
		}
		
		cout << "Z = " << M[brRedaka-1][brStupaca-1] << endl;
		
		cout << endl << "Vrijednosti varijabli za dualni problem su:" << endl;
		
		for(int i=0; i<M[0].size()-1; i++){ // za svaki stupac u M osim zadnjeg
			
				if(i<brStrukt){
					cout << "u" << i+1 << " = " << M[brRedaka-1][i] << endl;
						
				} else {
					cout << "x" << i+1-brStrukt << " = "<< M[brRedaka-1][i] << endl;
				}
					
		}
		
		
		cout << "Z = " << M[brRedaka-1][brStupaca-1] << endl;
		
	} else { // ako treba ispisati vrijednosti za min
		// prvo ispišemo vrijednosti bazičnih varijabli
		// one su u zadnjem redu gdje god je vrijednost != 0
		
		for(int i=0; i<M[0].size()-1; i++){ // za svaki stupac u M osim zadnjeg
			
				if(i<brStrukt){
					cout << "u" << i+1 << " = " << M[brRedaka-1][i] << endl;
						
				} else {
					cout << "x" << i+1-brStrukt << " = "<< M[brRedaka-1][i] << endl;
				}
					
		}
		
		
		cout << "Z = " << M[brRedaka-1][brStupaca-1] << endl;
		
		
		cout << endl << "Vrijednosti varijabli za dualni problem su:" << endl;
		
		bool nadjeno = false;
		
		for(int i=0; i<(brStrukt+brOgran); i++){
			if(i<brStrukt){
				cout << "x" << i+1 << " = ";
				nadjeno = false;
				
				for(int j=0; j<listaBv.size(); j++){
					
					if(listaBv[j] == i){
						cout << M[j][brStupaca-1] << endl;
						nadjeno = true;
	
					}
				}
				
				if(!nadjeno){
					cout << 0 << endl;
				}
				
			} else{
				cout << "u" << i+1-brStrukt << " = ";
				nadjeno = false;
				for(int j=0; j<listaBv.size(); j++){
				
					if(listaBv[j] == i){
						cout << M[j][brStupaca-1] << endl;
						nadjeno = true;
	
					}
				}
				if(!nadjeno){
					cout << 0 << endl;
				}
			}
		}
		
		cout << "Z = " << M[brRedaka-1][brStupaca-1] << endl;
	}
}

int simpleksPostupak(){ // rjijesi simpleks postupak sa globalnom M i listomBv
	Razlomak priv(0, 1);
	
	int stanje = 0; // 1 - optimalno rjesenje, 2- rjesenje neograniceno
	
	while(true){
		
		// Odredjivanje vodeceg stupca
		int vodeci_stupac = 0;
		
		priv.nazivnik = M[brRedaka-1][0].nazivnik;
		priv.brojnik = M[brRedaka-1][0].brojnik;
		
		for(int i=1; i<brStupaca; i++){
		//	cout << "Vodeci" << vodeci_stupac << "; vrijednost u stupcu:" << priv.Decimalni() << endl;
			if(M[brRedaka-1][i].Decimalni() < priv.Decimalni()){
				
				priv.nazivnik = M[brRedaka-1][i].nazivnik; // 0 u i
				priv.brojnik = M[brRedaka-1][i].brojnik; // 0 u i
				vodeci_stupac = i;
			}
			
		}
		
		if(M[brRedaka-1][vodeci_stupac].brojnik == 0){ // ako vise nema negativnih brojeva u poslejdnjem redu 
			stanje = 1;
			break;
		}
		
		// Ogredjivanje vodeceg reda:
		
		int vodeci_red = 0;
		
		priv.brojnik = M[0][brStupaca-1].brojnik;
		priv.nazivnik = M[0][brStupaca-1].nazivnik;
		
		double	koefStari; 
		
		// Tako da se ne dogodi da dijelimo sa nulom
		if(M[0][vodeci_stupac].Decimalni()==0){
			koefStari = INT_MAX;
		} else{
			koefStari = priv.Decimalni() / M[0][vodeci_stupac].Decimalni();
		}
	
		
		for(int i=1; i<brRedaka; i++){
			double koefNovi;
			
			if( M[i][vodeci_stupac].Decimalni() == 0){ // Tako da se ne dogodi da dijelimo sa nulom
				koefNovi = INT_MAX;
			} else{
				koefNovi = M[i][brStupaca-1].Decimalni() / M[i][vodeci_stupac].Decimalni();
			}
			
			if(koefStari<0){
				vodeci_red = i;
				koefStari = koefNovi;
			} else if(koefNovi > 0) {			
				if(koefNovi < koefStari){
					koefStari = koefNovi;
					vodeci_red = i;
				}
			}
			 
		}
		
		// Promjena bazicne varijable
		listaBv[vodeci_red] = vodeci_stupac;
		
		
		// Podjela reda sa pivot elementom
		Razlomak stozerni;
		
		stozerni.brojnik = M[vodeci_red][vodeci_stupac].brojnik;
		stozerni.nazivnik = M[vodeci_red][vodeci_stupac].nazivnik;
		
		for(int i=0; i<brStupaca; i++){
			M[vodeci_red][i] = M[vodeci_red][i] / stozerni;
		}
		
		// Preracunavnaje pivot stupca
		
		Razlomak koef;
		for(int i=0; i<(brRedaka); i++){
			
			if(i!=vodeci_red){
				koef = M[i][vodeci_stupac];
				
				for(int j=0; j<(brStupaca); j++){
					M[i][j] = M[i][j] - M[vodeci_red][j]*koef;
					
				}
			
			}
			
			
		}
		
		
	
	}

	return stanje;
	
}

void rijesiMax(){
	
		// Za simpleksicu

	brRedaka = brOgran+1;
	brStupaca = brOgran + brStrukt+1;
	
	Razlomak priv(0, 1);
	
	if(!M_stvoreno){
		M.resize(brRedaka, vector<Razlomak>(brStupaca, priv)); 
				// matrica dimenzija (brOgran+1, brOgran + brStrukt+1) 
								// za (broj redova, broj stupaca)
		M_stvoreno = true;
		
	}else{
		M.clear();
		M.resize(brRedaka, vector<Razlomak>(brStupaca, priv)); 
	}
	
				
	stvoriMatricuMax(M);
	int stanje = simpleksPostupak();
	
	
	if(stanje==1){
	//	ispisSimpleks(M);
		cout << endl;
		ispisFinal(M, listaBv);
	} else if(stanje==2){
	//	ispisSimpleks(M);
		cout << "Rjesenje je neograniceno." << endl;
	}	
	
}

void rijesiMin(){
	
	if(!preveden){
		
		
		// prevađanje ograničenja u dual
		vector<int> priv_fCilja = fCilja;
		vector<vector<int>> priv_fOgranicenja = fOgranicenja;
		
		fOgranicenja.clear();
		fCilja.clear();
		
		fOgranicenja.resize(priv_fCilja.size(), vector<int>(priv_fOgranicenja.size()+1, 0));	
		fCilja.resize(priv_fOgranicenja.size());
		
		for(int i=0; i<priv_fOgranicenja.size(); i++){ // za svaki red u staroj matrici
			
			for(int j=0; j<(priv_fOgranicenja[i].size()-1); j++){ // za svaki stupac po redu, osim zadnji
					fOgranicenja[j][i] = priv_fOgranicenja[i][j]; // transponiraj
			}
		}
		
		for(int i=0; i<fOgranicenja.size(); i++){ // za svaki red u novoj matrici
			fOgranicenja[i][fOgranicenja[i].size()-1] = priv_fCilja[i]; // za zadnji element u redu
		}
		
		for(int i=0; i<priv_fOgranicenja.size(); i++){ // za svaki red u staroj fOgranicenja
			fCilja[i] = priv_fOgranicenja[i][priv_fOgranicenja[i].size()-1]; // uzmi zadnji element
		}
		
		preveden = true;
	}//if(!preveden)
	
	brStrukt = fCilja.size();
	brOgran = fOgranicenja.size();
	
	// kolko redaka ima nova matrica?
	
	brRedaka = brOgran+1;
	brStupaca = brOgran + brStrukt+1;
	
	Razlomak priv(0, 1);
	
	if(!M_stvoreno){
		M.resize(brRedaka, vector<Razlomak>(brStupaca, priv)); 
				// matrica dimenzija (brOgran+1, brOgran + brStrukt+1) 
								// za (brjo redova, broj stupaca)
		M_stvoreno = true;
		
	}else{
		M.clear();
		M.resize(brRedaka, vector<Razlomak>(brStupaca, priv)); 
	}
	
				
	stvoriMatricuMax(M);
	
	int stanje = simpleksPostupak();
	
	
	if(stanje==1){
	//	ispisSimpleks(M);
		cout << endl <<  "Pronadjeno je optimalno rjesenje sa vrijednostima: " << endl;
		ispisFinal(M, listaBv);
	} else if(stanje==2){
	//	ispisSimpleks(M);
		cout << "Rjesenje je neograniceno." << endl;
	}	
	
}


int main(){
	int br = 0;
	koraci = 'n';
	cout << "Napomena: ovaj program radi samo sa standardnim problemima minimuma i maksimuma i stoga pretpostavlja ogranicenja na unesene vrijednosti funkcija ogranicenja. Takoder, ne prima negativne vrijednosti koeficjenata varijabli funkcija cilja niti varijabli funkcija ogranicenja." << endl << endl;
	do{
		cout << "Unesite odabir:"<< endl;
		cout << "1. Izracun standardnog problema maksimuma" << endl;
		cout << "2. Izracun standardnog problema minimuma" << endl;
	
	
		cin >> odabir;
	}while(odabir!=1 && odabir!=2);
	
	
	while(br!=9){
		cout << endl;
		cout << "Unesite odabir:"<< endl;
		cout << "1. Unos novih vrijednosti" << endl;
		cout << "2. Ispis unesenih vrijednosti" << endl;
		cout << "3. Rijesi problem" << endl;
		cout << "9. Izadji iz programa" << endl;
		cin >> br;
		
		switch(br){
			case 1:
				uneseno = true;
				Unos();
				break;
			case 2:
				if(uneseno){
					Ispis();
					
				} else {
					cout << "Vrijednosti nisu unesene" << endl;
				}
				break;
			case 3:
				
				if(odabir==1){
					rijesiMax();
					
				}else{
					rijesiMin();
				}
				
				break;
				
			case 9:
				break;
				
			default:
				cout << "Krivi unos! Unesite brojeve 1, 2, 3, 4 ili 9." << endl;		
		}
	}
		
	
	return 0;
}