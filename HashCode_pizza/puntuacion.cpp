#include<iostream>

using namespace std;

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

int main(){
    int s, puntos = 0;
    cin >> s;
    for(int i = 0; i < s; i++){
        int r1, r2, c1, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        slice p(r1, r2, c1, c2);
        puntos+=p.result();    
    }
    cout << "Puntuación: " << puntos << endl;
    system("pause");
}
