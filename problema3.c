#include <stdio.h>

typedef unsigned (*op_func)(unsigned int, unsigned int);

unsigned add(unsigned a, unsigned b)
{
	//printf("Intra la add cu %u si %u\n", a, b);
	unsigned ret = 0;
	//unsigned carrage = 0; DE SCOS CRED
	for (int i = 0; i < 4; i++)
	{
		ret += (a & (1 << i)) + (b & (1 << i)); 	
	}

	// bitul 4 (al 5lea) mereu sa fie 0
	ret &= ~(1 << 4);
	
	return ret;
}

unsigned xor(unsigned a, unsigned b)
{
	//printf("Intra la xor cu %u si %u\n", a, b);
	unsigned ret = a ^ b;
	return ret;
}

unsigned swap(unsigned a, unsigned b)
{
	//printf("Intra la swap cu %u si %u\n", a, b);
	unsigned ret = a;
	// iau pozitia primului bit care trebuie interschimbat
	unsigned pos1 = 0;
	unsigned bit = (b >> 2) & 1;
	pos1 |= bit;
	bit = (b >> 3) & 1;
	pos1 |= bit << 1;

	// pozitia celui de-al doilea bit	
	unsigned pos2 = 0;
	bit = b & 1;
	pos2 |= bit;
	bit = (b >> 1) & 1;	
	pos2 |= bit << 1;

	int pos_c1 = 3 - pos1;
	int pos_c2 = 3 - pos2;
	
	// construim rezultatul	
	unsigned b1 = (a >> pos_c1) & 1; // iau bitul de pe pozitia pos1
	unsigned b2 = (a >> pos_c2) & 1; // iau bitul de pe pozitia pos2
	
	if (b1)
		ret |= (b1 << pos_c2);
	else
		ret &= ~(1 << pos_c2);

	if (b2)
		ret |= (b2 << pos_c1);
	else
		ret &= ~(1 << pos_c1);

	return ret;
}

unsigned rotate_once(unsigned a)
{
	int ret;
	// iau primul bit
	unsigned b = (a >> 3) & 1;
	// il elimin
	a &= ~(1 << 3);

	ret = a << 1;
	// setez ultimul bit ca fiind primul luat
	if (b)
		ret |= b;
 
	return ret;
}

unsigned rotate(unsigned a, unsigned b)
{
	//printf("Intra la rotate cu %u si %u\n", a, b);
	unsigned ret = 0;
	for (int i = 0; i < b; i++)
		a = rotate_once(a);
	
	ret = a;
	return ret;
}

int main()
{
	int N;
	unsigned M;
	scanf("%d", &N);
	scanf("%u", &M);


	//op_func next_op; DE SCOS CRED
	op_func vec[] = {add, swap, rotate, xor};

	int n_biti = 6 * N + 4;
	int start_index = n_biti - 1;
	
	//printf("start_index = %d\n", start_index);

	unsigned val = 0;
	// iau 4 biti de la start index si ii mut in val
	for (int i = 0; i < 4; i++)
	{
		unsigned v = 0;
		v = M & (1 << (start_index - (3 - i)));
		//v = (M >> (start_index - (3 - i))) & 1;
        v = v >> (start_index - (3 - i));

		val |= (v << i);
	}
	start_index -= 4;

	
	while (start_index > -1)
	{
		// iau urmatoarea operatie
		unsigned op = 0;
		for (int i = 0; i < 2; i++)
		{
			unsigned b = 0;
			b = (M >> (start_index - i)) & 1; 
			
			op |= b << (1 - i);
			
		}
		start_index -= 2;

		// iau urmatorul operand
		unsigned second = 0;
		for (int i = 0; i < 4; i++)
		{
			unsigned v = 0;
			v |= M & (1 << (start_index - (3 - i)));
			second |= v >> (start_index - (3 - i) - i);
		}
		
		start_index -= 4;

		// fac operatia
		val = (vec[op])(val, second);
	}

	printf("%u", val);	

	return 0;
}
