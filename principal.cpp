#include <iostream>
#include<cstdlib> //para rand() e srand()
#include <string>
#include <vector>
#include<cmath>
#include<queue>

using namespace std;

vector<int> firstprimes = { 2 , 3 , 5 , 7 , 11 , 13 , 17 , 19 , 23 ,
29 , 31 , 37 , 41 , 43 , 47 , 53 };

//máximo divisor comum, também chamado de algoritmo Euclidiano simples
long long int mcd(long long int e, long long int phi) {
    long long int temp; //intermediário
    while (e != 0) {
        temp = phi % e;
        phi = e;
        e = temp;
    }
    return phi;
}

bool isCoprime(long long int a, long long int b) {
    return mcd(a, b) == 1;
}

//exponenciação modular
unsigned long long modPow(unsigned long long base, unsigned long long expoente, unsigned long long modulo) {
    unsigned long long resul = 1;
    base %= modulo;
    while (expoente > 0) {
        // Verifica se o ultimo digito do numero binário é 1
        if (expoente & 1) {
            resul = (resul * base) % modulo;
        }
        base = (base * base) % modulo;
        expoente >>= 1; //divide o expoente por 2
    }
    return resul;
}

//verifica se o número é divisível pelos primeiros primos.
bool easy(long long int numero) {
    for (int z = 0; z < firstprimes.size(); z++) {
        if (numero % firstprimes[z] == 0)
            return false;//indica que o numero fornecido não é primo
    }
    return true;
}

//verifica se o número é composto, faz parte da verificação de MillerRabin
bool check(long long int d, long long int n, int k) {
    long long int a = 2 + rand() % (n - 4); //numero aleatório
    long long x = modPow(a, d, n);
    if (x == 1 || x == n - 1)
        return false;
    for (int m = 1; m < k; m++) {//verifica se algum termo é divisível por n
        x = (x * x) % n;
        if (x == n - 1)
            return false;
        if (x == 1)
            return true;
    }
    return true;
}


bool millerRabin(long long int n, int iter) {
    if (n <= 4)
        return n == 2 || n == 3;

    long long int d = n - 1;//d é par se n for primo diferente de 2
    int k = 0;
    //quando é divisível por 2
    while ((d & 1) == 0) {
        d >>= 1;
        k++;
    }
    for (int j = 0; j < iter; j++) { 
        if (check(d, n, k))
            return false;
    }
    return true;
}


bool isPrime(long long int numero) {
    if (easy(numero)) {
        if (millerRabin(numero, 5))
            return true;
        else
            return false;
    }
    else
        return false;
}


//algoritmo euclidiano extendido
long long int extd(long long int a, long long int b) {
    queue<long long int> filax;
    queue<long long int> filay;
    long long int q;
    long long int w = a;
    filax.push(1);
    filax.push(0);
    filay.push(0);
    filay.push(1);

    while (b > 0) {
        q = a / b;
        long long int temp = a;
        a = b;
        b = temp % b;

        filax.push(filax.front() - (q * filax.back()));
        filay.push(filay.front() - (q * filay.back()));
        filay.pop();
        filax.pop();
    }
    if (filay.front() > 0) {
        return filay.front();
    }
    else {
        w = filay.front() + w;
        return w;

    }
}


long long int encode(unsigned char c, long long int n, long long int e) {
    long long int ciphervalue;
    int value = static_cast<int>(c);
    ciphervalue = modPow(value, e, n);
    return ciphervalue;

}

char decode(long long int  value, long long int n, long long int d) {
    long long int original;
    char ori;
    original = modPow(value, d, n);
    ori = static_cast<char>(original);
    return ori;

}


int main()
{   
    string message;
    string texto_original;
    long long int n, p, q;
    long long int e = 2;
    long long int d = 2;
    vector <unsigned long long int> ciphermessage;
   
    srand((unsigned)time(nullptr));

    cout << "*************ALGORITMO RSA*************\n";
    cout << "DIGITE A MENSAGEM A SER CRIPTOGRAFADA:\n ";
    getline(cin, message);

    //para achar p, gera número de 16bits 
    do {
        p = (pow(2, 15) - 1) + (rand() & 0X7FFF);
    } while (!isPrime(p));


    //para achar valor q , gera número de 16bits
    do {
        q = (pow(2, 15) - 1) + (rand() & 0X7FFF);
    } while (!isPrime(q));


    //operações
    n = p * q;
    long long int phi = (p - 1) * (q - 1);

    //chave pública
    do {
        e = rand() % phi + 2;
    } while (!isCoprime(e, phi));

    //chave privada 
    d = extd(phi, e);

    //para codificar cada letra com o mesmo e
    for (unsigned char c : message) {
        long long int cipherchar = encode(c, n, e);
        ciphermessage.push_back(cipherchar);

    }
    //imprimindo a mensagem cifrada
    cout << "\nMENSAGEM CIFRADA: [ ";
    for (unsigned long long int ci : ciphermessage) {
        cout << ci << " ";

    }
    cout << "]\n";
    cout << "\nMENSAGEM DECIFRADA: ";
    //decifrando a mesnagem
    for (unsigned long long int ci : ciphermessage) {
        texto_original += decode(ci, n, d);

    }
    //saída
    cout << texto_original;
    cout << "\n\n*************VALORES USADOS*************";
    cout << "\nP: " << p << "   ";
    cout << "Q: " << q << "   ";
    cout << "n : " << n << "   ";
    cout << "\nPHI : " << phi << endl;
    cout << "CHAVE PUBLICA: " << e << endl;
    cout << "CHAVE PRIVADA: " << d << endl;

}

