#include <iostream>

using namespace std;

class Razlomak{
	
		//Najveći zajednički djelitelj sa Euklidovim algoritmom
		// Radi samo za pozitivne brojeve, ako ubacujes koji negativni, pazi koje je rjesenje i za kaj iti treba to rjesenje
		public:
		long long nzd(long long a, long long b) {
            while (a != b) {
                if (a > b) {
                    a -= b;
                } else {
                    b -= a;
                }
            }
            return a;
        }
        
        long long brojnik, nazivnik;
        
        Razlomak(){
        	brojnik = 0;
        	nazivnik = 1;
		}
		
		Razlomak(long long b, long long n){
			if(n==0){
				cout << "Nazivnik ne moze biti 0." << endl;
				exit(0);
			}else if(b==0){
				brojnik=0;
				nazivnik=1;
			} else{
				int predznak =1;
				if(b<0){
					predznak *=-1;
					b *= -1;
				}
				if(n<0){
					predznak *=-1;
					n *= -1;
					
				}
				
				long long priv= nzd(b, n);
				brojnik = b/priv*predznak;
				nazivnik = n/priv;
			}
		}
		
		
		
		void Ispis(){
			if(nazivnik==1){
				cout << brojnik;
			} else {
				cout << brojnik << "/" << nazivnik;
			}
		}
		
		double Decimalni(){
			return double(brojnik/nazivnik);
		}
		
};

Razlomak operator+(const Razlomak& lijevi, const Razlomak& desni){ // radi
	Razlomak priv(lijevi.brojnik*desni.nazivnik
                +desni.brojnik*lijevi.nazivnik,
                lijevi.nazivnik*desni.nazivnik);
    
	return priv;
}

Razlomak operator-(const Razlomak& lijevi, const Razlomak& desni){
	Razlomak priv(lijevi.brojnik*desni.nazivnik
                -desni.brojnik*lijevi.nazivnik,
                lijevi.nazivnik*desni.nazivnik);
    
	return priv;
}

Razlomak operator-(const Razlomak& lijevi, int desni){ // radi
	
	Razlomak Desni(desni, 1);
	Razlomak priv(lijevi.brojnik*Desni.nazivnik
                -Desni.brojnik*lijevi.nazivnik,
                lijevi.nazivnik*Desni.nazivnik);
    
	return priv;
}

Razlomak operator-(int lijevi, const Razlomak& desni){
	
	Razlomak Lijevi(lijevi, 1);
	Razlomak priv(Lijevi.brojnik*desni.nazivnik
                -desni.brojnik*Lijevi.nazivnik,
                Lijevi.nazivnik*desni.nazivnik);
    
	return priv;
}


Razlomak operator*(int lijevi, const Razlomak& desni) { //radi
    Razlomak priv(lijevi*desni.brojnik,desni.nazivnik);
    return priv;
}

Razlomak operator*(const Razlomak& lijevi, const Razlomak& desni) { // radi
    Razlomak priv(lijevi.brojnik * desni.brojnik, lijevi.nazivnik * desni.nazivnik);
    return priv;
}

Razlomak operator*(const Razlomak& lijevi, int desni) { // radi
    Razlomak priv(desni*lijevi.brojnik, lijevi.nazivnik);
    return priv;
}


Razlomak operator/(const Razlomak& lijevi, const Razlomak& desni) { // radi
    Razlomak priv(lijevi.brojnik*desni.nazivnik, lijevi.nazivnik*desni.brojnik);
    return priv;
}


std::ostream& operator<<(std::ostream &strm, const Razlomak &a) { // radi
    if (a.nazivnik == 1) {
        strm << a.brojnik;
    } else {
        strm << a.brojnik << "/" << a.nazivnik;
    }
    return strm;
}
