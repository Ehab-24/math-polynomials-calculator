#include<iostream>
#include<fstream>
#include<conio.h>
#include<windows.h>
using namespace std;


struct Polynomial
{
	int deg;
	float* c;
};


void Refresh() {

	cout << endl;
	system("pause");
	system("cls");
}

void Operations_List(int &op) {

	cout << "\t1. Add two polynomials.\n"
		<< "\t2. Subtract two polynomials.\n"
		<< "\t3. Multiply two polynomials.\n"
		<< "\t4. Quotient of two polynomials.\n"
		<< "\t5. Remainder two polynomials.\n"
		<< "\t6. Evaluate a polynomial.\n"
		<< "\t7. Differentiate a polynomial.\n"
		<< "\t8. Integrate a polynomial.\n"
		<< "Select an Option OR Enter 0 to exit: ";
	cin >> op;
}


void LoadPolynomials(Polynomial*& Ps, int & size)
{

	ifstream fin("Polynomials.txt");
	fin >> size;
	Ps = new Polynomial[size];
	
	for (int pi = 0; pi < size; pi++)
	{
		fin >> Ps[pi].deg;
		Ps[pi].c = new float[Ps[pi].deg + 1];

		for (int ci = Ps[pi].deg; ci >= 0; ci--)
			fin >> Ps[pi].c[ci];
	}
}

void DisplayPolynomial(const Polynomial p)
{
	if (p.c == nullptr or p.deg < 0)
		cout << "NaN";
	else
	for (int i = p.deg; i >= 0; i--)
	{
		if (p.c[i] != 0)
		{
			if (p.c[i] > 0 && i != p.deg)
				cout << '+';
			if(!(p.c[i] == 1 && i != 0))
				cout << p.c[i];
			if (i != 0)
			{
				cout << 'x';
				if (i != 1)
					cout << '^' << i;
			}
		}
	}
}
void DisplayAllPolynomials(Polynomial *Ps, int size){

	cout << char(26) << " We have Polynomials : \n";
	for (int i = 0; i < size; i++)
	{
		cout << char('a' + i) << ")\tP_" << char('a' + i) << "(X) = ";
		DisplayPolynomial(Ps[i]);
		cout << endl;
	}
}


float getCoff(Polynomial p, int term)
{
	if (term > p.deg)
		return 0;
	return p.c[term];
}

Polynomial add(Polynomial p1, Polynomial p2)
{
	Polynomial A;
	A.deg = p1.deg > p2.deg ? p1.deg : p2.deg;
	A.c = new float[A.deg + 1];

	for (int term = 0; term < A.deg + 1; term++) 
		A.c[term] = getCoff(p1, term) + getCoff(p2, term);

	return A;
}

void copy(float* a, float* b, int size)
{
	for (int i = 0; i < size; i++)
		a[i] = b[i];
}
void ShrinkPolynomial(Polynomial &p) {

	for (int i = p.deg; i >= 0; i--)
	{
		if (p.c[i] == 0)
			p.deg--;
		else break;
	}
	float*temp = new float[p.deg + 1];
	
	copy(temp, p.c, p.deg + 1);
	
	delete[] p.c;
	p.c = temp;
}

Polynomial subtract(Polynomial p1, Polynomial p2)
{
	Polynomial S;
	S.deg = p1.deg >= p2.deg ? p1.deg : p2.deg;
	S.c = new float[S.deg + 1];

	for (int term = 0; term < S.deg + 1; term++)
		S.c[term] = getCoff(p1, term) - getCoff(p2, term);
	
	ShrinkPolynomial(S);

	return S;
}

Polynomial multiply(Polynomial p1, Polynomial p2)
{
	Polynomial mul;
	mul.deg = p1.deg + p2.deg;
	mul.c = new float[mul.deg + 1]{};

	for (int i = 0; i < p1.deg + 1; i++)
		for (int j = 0; j < p2.deg + 1; j++)
			mul.c[i + j] = mul.c[i + j] + (getCoff(p1, i) * getCoff(p2, j));

	return mul;
}
Polynomial Remainder(Polynomial p1, Polynomial p2)
{
	Polynomial r, q;		//Remainder, Quotient

	r.c = nullptr;
	r.deg = 0;
	q.deg = p1.deg - p2.deg;
	q.c = new float[q.deg + 1]{};

	for (int i = 0; p2.deg <= p1.deg; i++)
	{
		q.c[q.deg - i] = p1.c[p1.deg] / p2.c[p2.deg];

		Polynomial temp1;
		temp1.deg = q.deg - i;
		temp1.c = new float[temp1.deg + 1]{};
		temp1.c[q.deg - i] = q.c[q.deg - i];

		Polynomial temp2 = multiply(temp1, p2);
		r = subtract(p1, temp2);
		p1 = r;
	}
	return r;
}

Polynomial quotient(Polynomial p1, Polynomial p2)
{
	Polynomial r, q;		//Remainder, Quotient

	q.deg = p1.deg - p2.deg, r.deg = p1.deg;
	q.c = new float[q.deg + 1]{};

	for (int i = 0; p2.deg <= p1.deg; i++)
	{
		q.c[q.deg - i] = p1.c[p1.deg] / p2.c[p2.deg];

		Polynomial temp1;
		temp1.deg = q.deg - i;
		temp1.c = new float[temp1.deg + 1]{};
		temp1.c[q.deg - i] = q.c[q.deg - i];

		Polynomial temp2 = multiply(temp1, p2);	//<--
		r = subtract(p1, temp2);
		p1 = r;
	}
	return q;
}

float Evaluate(Polynomial p, float x)
{
	float ans = 0;
	for (int term = 0; term <= p.deg; term++)
		ans += (p.c[term] * pow(x, term));
	
	return ans;
}

Polynomial derivative(Polynomial a)
{
	Polynomial der;
	der.deg = a.deg - 1;
	der.c = new float[der.deg + 1];

	for (int i = a.deg; i > 0; i--)
		der.c[i - 1] = getCoff(a, i) * i;

	return der;
}
Polynomial  Integral(Polynomial A, int upper_limit, int lower_limit, float &answer)
{
	Polynomial I;
	I.deg = A.deg + 1;
	I.c = new float[I.deg + 1]{};

	for (int i = I.deg; i > 0; i--)
		I.c[i] = getCoff(A, i - 1) / i;

	answer = Evaluate(I, upper_limit) - Evaluate(I, lower_limit);

	return I;
}



void main1(Polynomial*Ps)
{
	char p1, p2;
	cout << char(26) << " Select 1st Polynomial (a, b, ....): ";
	cin >> p1;
	p1 -= 'a';
	cout << char(26) << " Select 2nd Polynomial (a, b, ....): ";
	cin >> p2;
	p2 -= 'a';

	Polynomial a = add(Ps[p1], Ps[p2]);
	cout << "\n\n";		DisplayPolynomial(a);
}

void main2(Polynomial* Ps) 
{
	char p1, p2;
	cout << char(26) << " Select 1st Polynomial (a, b, ....): ";
	cin >> p1;
	p1 -= 'a';
	cout << char(26) << " Select 2nd Polynomial (a, b, ....): ";
	cin >> p2;
	p2 -= 'a';

	Polynomial s = subtract(Ps[p1], Ps[p2]);
	cout << "\n\n";		DisplayPolynomial(s);
}

void main3(Polynomial* Ps)
{
	char p1, p2;
	cout << char(26) << " Select 1st Polynomial (a, b, ....): ";
	cin >> p1;
	p1 -= 'a';
	cout << char(26) << " Select 2nd Polynomial (a, b, ....): ";
	cin >> p2;
	p2 -= 'a';

	Polynomial m = multiply(Ps[p1], Ps[p2]);
	cout << "\n\nProduct:\t";		DisplayPolynomial(m);
}

void main4(Polynomial* Ps)
{
	char p1, p2;
	cout << char(26) << " Select dividend Polynomial (a, b, ....): ";
	cin >> p1;
	p1 -= 'a';
	cout << char(26) << " Select divisor Polynomial (a, b, ....): ";
	cin >> p2;
	p2 -= 'a';

	Polynomial r = Remainder(Ps[p1], Ps[p2]);
	cout << "\n\nRemainder:\t";		DisplayPolynomial(r);
}

void main5(Polynomial* Ps)
{
	char p1, p2;
	cout << char(26) << " Select dividend Polynomial (a, b, ....): ";
	cin >> p1;
	p1 -= 'a';
	cout << char(26) << " Select divisor Polynomial (a, b, ....): ";
	cin >> p2;
	p2 -= 'a';

	Polynomial q = quotient(Ps[p1], Ps[p2]);
	cout << "\n\nQuotient:\t";		DisplayPolynomial(q);
}

void main6(Polynomial *Ps)
{
	char p;
	cout << char(26) << " Select Polynomial to be evaluated (a, b, ....): ";
	cin >> p;
	p -= 'a';
	
	int x;
	cout << char(26) << " Enter value of x: ";
	cin >> x;
	int e = Evaluate(Ps[p], x);
	cout << "\n\nEvaluated Answer = " << e;
}

void main7(Polynomial *Ps)
{
	char p;
	cout << char(26) << " Select Polynomial for differentitation (a, b, ....): ";
	cin >> p;
	p -= 'a';
	
	Polynomial der = derivative(Ps[p]);
	cout << "\n\nDerivative:\t";		DisplayPolynomial(der);
}

void main8(Polynomial* Ps)
{
	char p;
	cout << char(26) << " Select Polynomial for Integration (a, b, ....): ";
	cin >> p;
	p -= 'a';

	float answer = 0;
	int upper_limit, lower_limit;
	cout << char(26) << " Enter Upper Limit: ";
	cin >> upper_limit;
	cout << char(26) << " Enter Lower Limit: ";
	cin >> lower_limit;

	Polynomial I = Integral(Ps[0], upper_limit, lower_limit, answer);
	cout << "\nIntegral:\t";		DisplayPolynomial(I);
	cout << "\nAnswer at limits "<< upper_limit << " and " << lower_limit << " is: " << answer;
}




int main()
{
	int size;
	int i, j;
	int op, c = 26;
	Polynomial* Ps;

	LoadPolynomials(Ps, size);

	do
	{
		DisplayAllPolynomials(Ps, size);
		cout << endl;
		Operations_List(op);

		switch (op)
		{
		case 1:
			main1(Ps);
			break;
		case 2:
			main2(Ps);
			break;
		case 3:
			main3(Ps);
			break;
		case 4:
			main4(Ps);
			break;
		case 5:
			main5(Ps);
			break;
		case 6:
			main6(Ps);
			break;
		case 7:
			main7(Ps);
			break;
		case 8:
			main8(Ps);
			break;

		default:
			if (op != 0)
				cout << "Invalid input!!!";
		}

		Refresh();

	} while (op != 0);

	return 0;
}