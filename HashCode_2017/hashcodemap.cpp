#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <cmath>
#include <time.h>

using namespace std;

/* retorna "a - b" en segundos */
double performancecounter_diff(LARGE_INTEGER *a, LARGE_INTEGER *b){
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  return (double)(a->QuadPart - b->QuadPart) / (double)freq.QuadPart;
}


struct casa{
	int latD;
	vector<pair<int,int> > lat_cache;
};

struct peticion{
	int IDcasa, IDvideo, num;
};


class Llutub{
public:
	int nvid, ncasitas, npet, ncaches, tam_cache;
	vector<int> tam_video, cache;
	vector<casa> casitas;
	vector<peticion> peticiones;
	vector<bool> pet_usados;
	vector<set<int> > solucion;	
	vector<vector<double> > prop_latencias;

	Llutub(int anvid,int ancasita,int anpet,int ancache,int atamcache){
		nvid = anvid;
		ncasitas = ancasita;
		npet = anpet;
		ncaches = ancache;
		tam_cache = atamcache;
		
		for(int i = 0; i < ncaches; i++){
		    cache.push_back(0);
            set<int> v;
            solucion.push_back(v);  
        }	  
	}

	void setPesoVideos(vector<int> tams){
		tam_video = tams;
	}
	
	void setCasitas(vector<casa> casi){
		casitas = casi;
	}
	
	void setPeticiones(vector<peticion> peti){
		peticiones = peti;
		for(int i = 0; i < peticiones.size(); i++){
			pet_usados.push_back(false);
		}
	}
		
	void AlgoritmoHashCode(){
        
        srand(time(NULL));
        double num;
        
        quicksort(peticiones, 0, peticiones.size()-1);
        cout << "Iniciando algoritmo." << endl;
        int numerito = 1000;
        //for(int i=0; i < peticiones.size(); i++){
        for(int i=peticiones.size()-1; i >= 0 ; i--){
            int video, cache2, min=99999999;
            		            
            //for(int j = 0; j < casitas[peticiones[i].IDcasa].lat_cache.size(); j++){
			for(int j = casitas[peticiones[i].IDcasa].lat_cache.size()-1; j >= 0; j--){				
				if(tam_video[peticiones[i].IDvideo]+cache[casitas[peticiones[i].IDcasa].lat_cache[j].first] <= tam_cache){
                    num = rand()%numerito;
					int prop_total = casitas[peticiones[i].IDcasa].lat_cache[j].second*cache[casitas[peticiones[i].IDcasa].lat_cache[j].first];					
					if(prop_total < min){
						min = prop_total;
						cache2 = casitas[peticiones[i].IDcasa].lat_cache[j].first;
						video = peticiones[i].IDvideo;
					}
					else if(num < 500){
                        min = prop_total;
						cache2 = casitas[peticiones[i].IDcasa].lat_cache[j].first;
						video = peticiones[i].IDvideo;
                    }
				}
				numerito++;
			}	
                    
            if(solucion[cache2].find(video) == solucion[cache2].end()){
                solucion[cache2].insert(video);                
                cache[cache2] += tam_video[peticiones[i].IDvideo];
            }
		}
	}		
    	
    // Ordenados por numero de peticiones
    int divide(vector<peticion> &array, int start, int end){
        int pivot = array[start].num;
        int right = end;
        int left = start;
        peticion temp;
        
        while(left < right){
            while(array[right].num > pivot)
                right--;
            while(left < right && array[left].num <= pivot)
                left++;
                
            if(left < right){
                temp = array[left];
                array[left] = array[right];
                array[right] = temp;
            }             
        }
        
        temp = array[start];
        array[start] = array[right];
        array[right] = temp;
        
        return right;
    }
    
    void quicksort(vector<peticion> &array, int start, int end){
        if(start < end){
           int pivot = divide(array, start, end);
           quicksort(array, start, pivot-1);
           quicksort(array, pivot+1, end);   
        }    
    }
	
	void printSolucion(){
		ofstream f;
        f.open("output.txt");
        
        int tam_sol = 0;
        set<int>::iterator pos;
        
        for(int i = 0; i < solucion.size(); i++)
            if(solucion[i].size() != 0)
                tam_sol++;
        
        f << tam_sol << endl;
        
        for(int i = 0; i < solucion.size(); i++){
            if(solucion[i].size() != 0){
                f << i << " ";                
                for(pos = solucion[i].begin(); pos != solucion[i].end();++pos)
                    f << *pos << " ";    
                f << endl;
            }    
        }
	}
	
	void Algoritmo2(){
        int tope_cache = cache.size();
        int tope_videos = tam_video.size();
        
        for(int i = tope_cache-1; i >= 0; i--){  
            bool seguir = true;
            bool usados[tam_video.size()];
            
            for(int w = tam_video.size(); w >= 0 ; w--)
            	usados[w] = false;

            int q = 0;
            while(seguir){
                q++;
                int max = 0;
                int index_video = -1;
                
                for(int j = 0; j < tope_videos; j++){
                    if(tam_video[j]+cache[i] <= tam_cache && !usados[j]){
                        solucion[i].insert(j);
                        int score = getScore();
                        
                        if(score > max){
                            max = score;
                            index_video = j;    
                        } 
                        solucion[i].erase(j);     
	                                   
                    }
                }  
                
                if(index_video != -1){
                   usados[index_video] = true;
                   solucion[i].insert(index_video);   
                   cache[i] += tam_video[index_video];
                } 
                else
                    seguir = false;
            }               
        }    
    }
	
	void Algoritmo3(){
		bool usados[ncaches][nvid];

		for(int i = 0; i < ncaches; i++)
			for(int j = 0; j < nvid; j++)
				usados[i][j] = false;

        for(int i = peticiones.size()-1; i >= 0; i--){
            long long int max = 0;
            int index_cache = -1;
            int video = peticiones[i].IDvideo;
            if(i%50 == 0)
                cout << i << endl;
            for(int j = cache.size()-1; j >= 0 ; j--){
                if(tam_video[video]+cache[j] <= tam_cache){                            
                    if(!usados[j][video]){
                        solucion[j].insert(video);  
                        long long int score = getScore();
                        
                        if(score > max){
                            max = score;
                            index_cache = j;    
                        }
                        solucion[j].erase(video);
                    } 
                }
            } 
            if(index_cache != -1){
                solucion[index_cache].insert(video);
                cache[index_cache] += tam_video[video]; 
                usados[index_cache][video] = true;
            }
        }    
    }
	
	long long int getScore(){
        long long int sum_ganancia = 0;
        int tope_peticiones = peticiones.size();
        
        for(int i = 0; i < tope_peticiones; i++){
            long long int min_latencia = 999999999;
            int tope_cache = casitas[peticiones[i].IDcasa].lat_cache.size();

            for(int j = 0; j < tope_cache; j++){
                if(casitas[peticiones[i].IDcasa].lat_cache[j].second < min_latencia){                   
                    if(solucion[casitas[peticiones[i].IDcasa].lat_cache[j].first].find(peticiones[i].IDvideo) != 
                       solucion[casitas[peticiones[i].IDcasa].lat_cache[j].first].end())
                       min_latencia = casitas[peticiones[i].IDcasa].lat_cache[j].second;                                              
                }
            } 
            if(min_latencia != 999999999)
                sum_ganancia += peticiones[i].num*(casitas[peticiones[i].IDcasa].latD - min_latencia);
        }          
        return sum_ganancia;  
    }
    
    int getScoreDefinitive(){
        long long int sum_ganancia = 0;
        long long int sum_peticiones = 0;
        int tope_peticiones = peticiones.size();
        
        for(int i = 0; i < tope_peticiones; i++){
            long long int min_latencia = 999999999;
            int tope_cache = casitas[peticiones[i].IDcasa].lat_cache.size();

            for(int j = 0; j < tope_cache; j++){
                if(casitas[peticiones[i].IDcasa].lat_cache[j].second < min_latencia){                     
                    if(solucion[casitas[peticiones[i].IDcasa].lat_cache[j].first].find(peticiones[i].IDvideo) != 
                       solucion[casitas[peticiones[i].IDcasa].lat_cache[j].first].end())
                       min_latencia = casitas[peticiones[i].IDcasa].lat_cache[j].second;                
                }
            } 
            if(min_latencia != 999999999)
                sum_ganancia += peticiones[i].num*(casitas[peticiones[i].IDcasa].latD - min_latencia);
                
            sum_peticiones += peticiones[i].num;
        }          
        return 1000*sum_ganancia/sum_peticiones;  
    }
};


int main(){
	int n_videos, n_casitas, n_tipo_peticiones, n_caches, tam_cache;
	vector<int> tam_videos;
	vector<casa> casitas;
    vector<peticion> peti;	
    
    
    LARGE_INTEGER t_ini, t_fin;
    double secs_ejecucion, secs_score;
    
	//primera linea
	cin >> n_videos >> n_casitas >> n_tipo_peticiones >> n_caches >> tam_cache;
	Llutub elRubius(n_videos,n_casitas,n_tipo_peticiones,n_caches,tam_cache);
 
    //segunda linea (tamaño de videos)
    for(int i=0; i<n_videos; i++){
		int tam=0;
		cin >> tam;
		tam_videos.push_back(tam);
	}
	
	elRubius.setPesoVideos(tam_videos);

	for(int i=0; i<n_casitas; i++){
		casa temp;
		int connected=0;
		cin >> temp.latD >> connected;
		
		for(int j=0; j<connected; j++){
			int lat=0, id=0;
            cin >> id >> lat;
            pair<int, int> temp_cache(id,lat);
			temp.lat_cache.push_back(temp_cache);
		}	
        casitas.push_back(temp);
    }

	elRubius.setCasitas(casitas);
	int aux1,aux2,aux3;

    for(int i = 0; i < n_tipo_peticiones; i++){
        cin >> aux1 >> aux2 >> aux3;
        peticion p;
        p.IDvideo = aux1;
        p.IDcasa = aux2;
        p.num = aux3;
        peti.push_back(p);	
    }
    
    elRubius.setPeticiones(peti);
    cout << "Lectura completada" << endl;
    QueryPerformanceCounter(&t_ini);
    elRubius.AlgoritmoHashCode();
    //elRubius.Algoritmo2();
    QueryPerformanceCounter(&t_fin);
    secs_ejecucion = performancecounter_diff(&t_fin, &t_ini);
    
    cout << "Algoritmo terminado." << endl << "Get score..." << endl;
    
    
    elRubius.printSolucion();
    
    QueryPerformanceCounter(&t_ini);
    cout << "Score: " << elRubius.getScoreDefinitive() << endl;
    QueryPerformanceCounter(&t_fin);
    secs_score = performancecounter_diff(&t_fin, &t_ini);
    cout << "Tiempo de ejecución: " << secs_ejecucion << " segundos." << endl;
    cout << "Tiempo de score    : " << secs_score << " segundos." << endl;
    system("pause");
}
