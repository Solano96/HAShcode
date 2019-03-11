#include<iostream>
#include<vector>
#include<fstream>
#include<cmath>
#include<string>
#include<numeric>
#include <algorithm>

#include<stdlib.h>
#include<time.h>

using namespace std;


struct foto{
	int ID;
	char pos;
	int num_tag;
	vector<string> tags;
};


struct diapositiva{
	int foto1;
	int foto2;
};


vector<foto> fotos;

vector<diapositiva> presentacion;


void intercambiar(int num, int num2, int tam){	
	for(int i = 0; i < tam; i++){
		diapositiva temp = presentacion[num+i];
		presentacion[num+i] = presentacion[num2+i];
		presentacion[num2+i] = temp;
	}
}

void intercambiar_verticales(int id1, int id2, int pos1, int pos2){
	vector<int> diapositiva1;
	diapositiva1.push_back(presentacion[id1].foto1);
	diapositiva1.push_back(presentacion[id1].foto2);

	vector<int> diapositiva2;
	diapositiva2.push_back(presentacion[id2].foto1);
	diapositiva2.push_back(presentacion[id2].foto2);

	int temp = diapositiva1[pos1];
	diapositiva1[pos1] = diapositiva2[pos2];
	diapositiva2[pos2] = temp;

	presentacion[id1].foto1 = diapositiva1[0];
	presentacion[id1].foto2 = diapositiva1[1];

	presentacion[id2].foto1 = diapositiva2[0];
	presentacion[id2].foto2 = diapositiva2[1];
}



int slide_score2(int i){
	vector<string> tags1(200);
	vector<string> tags2(200);
	

	if(presentacion[i].foto2 == -1)
		tags1 = fotos[presentacion[i].foto1].tags;
	else{
		set_union(fotos[presentacion[i].foto1].tags.begin(), fotos[presentacion[i].foto1].tags.end(), fotos[presentacion[i].foto2].tags.begin(), fotos[presentacion[i].foto2].tags.end(), tags1.begin());
	}


	if(presentacion[i].foto2 == -1)
		tags2 = fotos[presentacion[i+1].foto1].tags;
	else{
		set_union(fotos[presentacion[i+1].foto1].tags.begin(), fotos[presentacion[i+1].foto1].tags.end(), fotos[presentacion[i+1].foto2].tags.begin(), fotos[presentacion[i+1].foto2].tags.end(), tags2.begin());
	}

	vector<string> interseccion;

	set_intersection(tags1.begin(), tags1.end(), tags2.begin(), tags2.end(), back_inserter(interseccion));

	vector<int> values;
	values.push_back(interseccion.size());
	values.push_back(tags1.size()-interseccion.size());
	values.push_back(tags2.size()-interseccion.size());

	sort(values.begin(),values.end());

	return values[0];
}


int score(){
	int sum = 0;
	for(int i = 0; i < presentacion.size()-1; i++){
		// puntuacion_presentacion[i] = slide_score(i)
		//sum += puntuacion_presentacion[i];
		sum += slide_score2(i);
	}
	return sum;
}



int get_sub_score(int num, int num2){
	int sub_score1 = 0;
	int sub_score2 = 0;
	int sub_score3 = 0;
	int sub_score4 = 0;

	if(num != 0)
		sub_score1 = slide_score2(num-1);

	if(num != presentacion.size()-1)
		sub_score2 = slide_score2(num);

	if(num2 != 0)
		sub_score3 = slide_score2(num2-1);

	if(num2 != presentacion.size()-1)
		sub_score4 = slide_score2(num2);

	int sub_score = sub_score1 + sub_score2 + sub_score3 + sub_score4;

	return sub_score;
}



int mutacion(int score){
	//int tam = rand()%4+1;

	int tam = 1;

	int num = rand()%(presentacion.size()-tam);

	int sub_score, sub_score_nueva;

	if(presentacion[num].foto2 == -1){

		int num2 = rand()%(presentacion.size()-tam);

		while(!(num2 <= num-tam || num2 >= num+tam))
			num2 = rand()%(presentacion.size()-tam);


		sub_score = get_sub_score(num, num2);
		
		intercambiar(num, num2, tam);

		sub_score_nueva = get_sub_score(num, num2);

		return sub_score_nueva-sub_score;
	}
	else{
		int num2 = rand()%(presentacion.size());

		while((num2 > num-tam && num2 < num+tam) || (presentacion[num2].foto2 == -1 && num == num2))
			num2 = rand()%(presentacion.size());

		if(presentacion[num2].foto2 == -1){
			sub_score = get_sub_score(num, num2);			
			intercambiar(num, num2, tam);
			sub_score_nueva = get_sub_score(num, num2);
			return sub_score_nueva-sub_score;			
		}
		else{
			int cambio = rand()%5;

			switch(cambio){
				case 0:
					sub_score = get_sub_score(num, num2);	
					intercambiar_verticales(num, num2, 0, 0);
					sub_score_nueva = get_sub_score(num, num2);
					return sub_score_nueva-sub_score;
				break;
				case 1:
					sub_score = get_sub_score(num, num2);	
					intercambiar_verticales(num, num2, 0, 1);
					sub_score_nueva = get_sub_score(num, num2);
					return sub_score_nueva-sub_score;
				break;
				case 2:
					sub_score = get_sub_score(num, num2);	
					intercambiar_verticales(num, num2, 1, 0);
					sub_score_nueva = get_sub_score(num, num2);
					return sub_score_nueva-sub_score;
				break;
				case 3:
					sub_score = get_sub_score(num, num2);	
					intercambiar_verticales(num, num2, 1, 1);
					sub_score_nueva = get_sub_score(num, num2);
					return sub_score_nueva-sub_score;
				break;
				case 4:
					sub_score = get_sub_score(num, num2);

					int temp = presentacion[num].foto1;
					presentacion[num].foto1 = presentacion[num2].foto1;
					presentacion[num2].foto1 = temp;

					temp = presentacion[num].foto2;
					presentacion[num].foto2 = presentacion[num2].foto2;
					presentacion[num2].foto2 = temp;

					sub_score_nueva = get_sub_score(num, num2);
					return sub_score_nueva-sub_score;
				break;
			}

		}
	}

}



int solucion_aleatoria(){

	for(int i = 0; i < fotos.size(); i++){
		if(fotos[i].pos == 'H'){
			diapositiva d;
			d.foto1 = i;
			d.foto2 = -1;
			presentacion.push_back(d);
		}
	}

	for(int i = 0; i < fotos.size(); i++){
		if(fotos[i].pos == 'V'){
			diapositiva d;
			d.foto1 = i;
			d.foto2 = -2;

			for(int j = i+1; j < fotos.size(); j++){
				if(fotos[j].pos == 'V'){
					d.foto2 = j;
					i = j+1;
					j = fotos.size();
				}
			}

			if(d.foto2 != -2){
				presentacion.push_back(d);
			}
		}
	}

	random_shuffle(presentacion.begin(), presentacion.end());
}


void algoritmo(int num_iter){

	solucion_aleatoria();

	int score_actual = score();

	for(int i = 0; i < num_iter; i++){

		if(i%2000==0)
			cout << i << endl;

		vector<diapositiva> presentacion_temporal;

		presentacion_temporal = presentacion;

		int sub_score = mutacion(score_actual);

		if(sub_score > 0){
			score_actual+=sub_score;
		}
		else{
			presentacion = presentacion_temporal;
		}
	}

	cout << score_actual << endl;
	cout << score() << endl;
}

int main(int argc, char **argv){
	srand(time(0));
	char pos;
	int num_tag;
	string tag;

	string line;

	//string name = "a_example";
	string name = "b_lovely_landscapes";
	//string name = "c_memorable_moments";
	//string name = "d_pet_pictures";
	//string name = "e_shiny_selfies";

	ifstream file(name + ".txt");

	int N;

	file >> N;

	for(int i = 0; i < N; i++){
		foto f;
		file >> pos;
		file >> num_tag;
		vector<string> tags;

		for(int j = 0; j < num_tag; j++){
			file >> tag;
			tags.push_back(tag);
		}

		sort(tags.begin(), tags.end());

		f.ID = i;
		f.pos = pos;
		f.num_tag = num_tag;
		f.tags = tags;

		fotos.push_back(f);
	}
		file.close();


	algoritmo(30000);

	ofstream fs(name + "_sol.txt"); 

	fs << presentacion.size() << endl;

	for(int i = 0; i < presentacion.size(); i++){
		if(presentacion[i].foto2 == -1){
			fs << presentacion[i].foto1 << endl;
		}
		else{			
			fs << presentacion[i].foto1 << " " << presentacion[i].foto2 << endl;
		}
	}

}