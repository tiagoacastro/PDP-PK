#include <iostream>
#include <cctype>
#include <cmath>
using namespace std;

//To run each exercise, just pass its number by parameter

//Choose 2 floats (x=2nd parameter, y=3rd parameter) and 2 integers (i=4th parameter, j=5th parameter)
int ex2(int argc, char *argv[]);
void ex4();
void ex5();
//Input the array values separated in different parameters
int ex6(int argc, char *argv[]);
//Input the radius as 2nd parameter
int ex7(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    if(argc <= 1 || !isdigit(*argv[1])){
        cout << "To run each exercise, just pass its number on the first parameter" << endl;
        return -1;
    }

    switch(atoi(argv[1])){
        case 1:
            cout << "Hi, my name is Tiago Araujo Castro." << endl;
            break;
        case 2:
            return ex2(argc, argv);
        case 3:
            cout << "There is no 3" << endl;
            break;
        case 4:
            ex4();
            break;
        case 5:
            ex5();
            break;
        case 6:
            return ex6(argc, argv);
        case 7:
            return ex7(argc, argv);
            break;
        default:
            cout << "Choose a number between 1 and 7" << endl;
            break;
    }

    return 0;
}

//Choose 2 floats (x=2nd parameter, y=3rd parameter) and 2 integers (i=4th parameter, j=5th parameter)
int ex2(int argc, char *argv[]){
    if(argc != 6 || !isdigit(*argv[2]) || !isdigit(*argv[3]) || !isdigit(*argv[4]) || !isdigit(*argv[5])) {
        cout << "Choose 2 floats (x=2nd parameter, y=3rd parameter) and 2 integers (i=4th parameter, j=5th parameter)" << endl;
        return -1;
    }

    float x = atof(argv[2]), y = atof(argv[3]);
    float i = atoi(argv[4]), j = atoi(argv[5]);

    double v = (log(pow(x,2.0)) + 2.0*pow(x,3.0) + (pow(y,-2.0))) / ((sin(x) - 10.0) * j) + (2.0*i) / (j + 1.0);
    double w = ((x*cos(x+1.0))/2.0) * pow(sin(5.0*pow(i,2.0)),2.0);

    cout << "v = " << v << " | w = " << w << endl;

    return 0;
}

//Only has the exact steps on the diagram (no input checking) and a result print
void ex4(){
    int a;

    do {
        cin >> a;
    } while (a<=0);

    cout << 3*a;
}

//Only has the exact steps on the diagram (no input checking) and a result print
void ex5(){
    int a;

    cin >> a;
    while (a<=0) {
        cin >> a;
    }

    cout << 3*a;
}

//Input the array values separated in different parameters
int ex6(int argc, char *argv[]){
    if(argc < 3) {
        cout << "Input the array values separated in different parameters" << endl;
        return -1;
    }

    int size = argc-2;
    int arr[size];
    for (int i = 2; i < argc; ++i) {
        if(!isdigit(*argv[i])){
            cout << "Input the array values separated in different parameters" << endl;
            return -1;
        }
        arr[i-2] = atoi(argv[i]);
    }

    int highest = arr[0], lowest = arr[0];

    for (int i = 1; i < size; ++i) {
        if(arr[i] > highest)
            highest = arr[i];
        else if(arr[i] < lowest)
            lowest = arr[i];
    }

    cout << "Min: " << lowest << " | Max: " << highest << endl;

    return 0;
}

float sphereArea(float *r);
float spherePerimeter(float *r);

//Input the radius as 2nd parameter
int ex7(int argc, char *argv[]){
    if(argc != 3 || !isdigit(*argv[2])) {
        cout << "Input the radius as 2nd parameter" << endl;
        return -1;
    }

    float r = atof(argv[2]);

    float area = sphereArea(&r);
    float perimeter = spherePerimeter(&r);

    cout << "Area: " << area << " | Perimeter: " << perimeter << endl;

    return 0;
}

float sphereArea(float *r){
    return 4.0*M_PI*pow((*r),2.0);
}

float spherePerimeter(float *r){
    return (4.0/3.0)*M_PI*pow((*r),3.0);
}


