#include<iostream>
#include<vector>
#include<fstream>

using namespace std;

class Pizza{
    private:
        struct slice{
            int r1,r2,c1,c2;
            int result(){                
                if(r1 != -1)
                    return (r2+1-r1)*(c2+1-c1);
                else
                    return 0;
            }
            
            slice(int a, int b, int c, int d){
                r1 = a;
                r2 = b;
                c1 = c;
                c2 = d;
            }
        };
    
        
        vector<vector <int> > pizza;         	//////   ESTO SE UNIRÁ EN UNA PIZZA
        vector<vector <bool> > marcado;	//////   DE PAREJAS (memoria eficiente)
        vector <pair <int,int> > multp;
        vector <slice>	solucion;
        int R, C, L, H; 				///////  FIL, COL, MIN M Y T, MAX M+T
    public:
        
        Pizza(int nR, int nC, int nL, int nH){
            solucion.clear();
            R=nR;
            C=nC;
            L = nL;
            H = nH;
            Divisores();
        }
    
    void add(vector<vector <int> > m){
        for(int i = 0; i < R; i++){
            vector<bool> v;
            for(int j = 0; j < C; j++){
                v.push_back(false);
            }
            marcado.push_back(v);
            pizza = m;       
        }     
           
    }
    
    void Divisores(){	
        for (int i =1;i<=H; i++){
            for (int j=1; j<=H;j++){
                if(j*i <= H && j*i>=2*L){
                    pair<int, int> p(i,j);
                    multp.push_back(p);
                }
            }
        }
    }
        
    void calcular_punto(int &i, int &j){
        bool encontrado = false;
        for(int i_1=0;i_1<R && !encontrado;i_1++){
            for(int j_1=0;j_1<C && !encontrado;j_1++){
                if(!marcado[i_1][j_1]){
                    i = i_1;
                    j = j_1;
                    encontrado = true;
                }
            }
        }
        if(!encontrado){
            i=-1;j=-1;
        }
    }
    
    
    void GORDA(){
        bool  seguir = true;
        int i,j;
        
        while(seguir){
            slice sol(-1,0,0,0);
            calcular_punto(i,j); 
            if(i != -1){
	            for(int k = 0; k < multp.size(); k++){
                    int algo1 = i + multp[k].first-1;
                    int algo2 = j + (int)multp[k].second-1;
                    
		            if(algo1 < R && algo2 < C){
			            bool valido = true;
			            slice p(i,i+multp[k].first-1,j,j+multp[k].second-1);
			            int nM, nT;
			            nM = nT = 0; 
			            for(int i_1= p.r1; i_1<=p.r2 && valido; i_1++){
				            for(int j_1 = p.c1; j_1 <= p.c2 && valido; j_1++ ){
					            if(marcado[i_1][j_1]){
					            	valido = false;
					            }
					            if(pizza[i_1][j_1] == 1){
					            	nM++;
					            }else{
					            	nT++;
					            }
				            }		
			            }
			            if(nT < L || nM < L){
			            	valido = false;
			            }
			            if(valido && p.result() > sol.result()){
			            	sol = p;
			            }
		            }
	            }
	            if(sol.result() != 0){
		            for(int a = sol.r1; a <= sol.r2; a++){
			            for(int b = sol.c1; b <= sol.c2; b++){
			            	marcado[a][b] = true;
			            }
		            }
		            solucion.push_back(sol);
	            }else{
	            	marcado[i][j] = true;
	            }
            }
            else{
                seguir=false;
            }
        }
    }
    
    void printeamelo_todo(){
    	ofstream f;
    	f.open("output.txt");
        int puntuacion = 0;
	    f << solucion.size() << endl;
	    for( int i=0; i<solucion.size(); i++){
	    	f<< solucion[i].r1 << " " << solucion[i].c1 << " " << solucion[i].r2 << " " << solucion[i].c2 << endl; 	
	    	puntuacion += solucion[i].result();
	    }    
	    
	    //cout << "Puntuación: " << puntuacion << endl << endl << "Tamaño pizza: " << R*C << endl;
    }

};

int main(){
	int F,C,L, H, aux;
	cin >> F >> C >> L >> H ;
    vector<vector <int> > m(F,C);

	Pizza carbonara(F,C,L,H);
	for(int i = 0; i < F; i++ ){
		for(int j = 0; j < C; j++){
            char l;
            cin >> l;
            if(l == 'M')
			   aux = 1;
            else
               aux = 0; 
			m[i][j] = aux;
		}
	}
	
	carbonara.add(m);

	carbonara.GORDA();
	carbonara.printeamelo_todo();

}
