#include <iostream>
#include <string>
#include <random>
#include <memory.h>
#include <fstream>
using namespace std;
uint32_t inright2(uint32_t& in, int size)
{
	return (in >> 2) | (in << size * 8 - 2);
}

uint32_t inleft2(uint32_t in, int size)
{
	return (in << 2) | (in >> size * 8 - 2);
}

void shifr(const char* in, char* out, size_t len, int key, bool decrypt)
{
	int block_size = 4; // Переменная размера блока
	int full_blocks_len = len / block_size; // Число полных блоков по 4 байта
	int part_blocks_len = len % block_size; // Размер неполного последнего блока
	if (part_blocks_len)
	{
		full_blocks_len++; // В цикле последним будет не полный блок
	}

	mt19937 bas(key);
	uniform_int_distribution<uint32_t> gen(0, 0xffffffff);
	uint32_t rez;
	uint32_t gamma;

	for (size_t i = 0; i < full_blocks_len; ++i)
	{
		if (part_blocks_len && (i == full_blocks_len - 1))
		{
			block_size = part_blocks_len; // размер последнего блока
			rez = 0; // Последний блок байты не используемые равны 0
		}

		memcpy(&rez, in + i * 4, block_size); // Копируем блок size байта в rez
		if (decrypt)
		{
			rez = inleft2(rez, block_size); // Побитовый сдвиг циклический
		}
		gamma = gen(bas);
		if (part_blocks_len && (i == full_blocks_len - 1))
		{
			// Для последнего не полного блока обнуляем не используемые байты гаммы
			uint32_t mask = 0xff; // Последний байт заполнен единицами
			for (uint8_t i = 1; i < part_blocks_len; ++i)
			{
				mask <<= 8;
				mask += 0xff;
			}
			gamma &= mask; // Обнуляем не используемые байты
		}
		rez ^= gamma;   // Наложение гаммы
		if (!decrypt)
		{
			rez = inright2(rez, block_size); // Побитовый сдвиг циклический
		}
		memcpy(out + i * 4, &rez, block_size); // Перемещаем данные в выходной массив
	}
}


int main(int argc, const char* argv[])
{

	if (argc != 3) {
		cerr << "Error: Use three parameters\n";
		return 5;
	}
	const string mode(argv[1]); 
	const string file_name(argv[2]); 

	int key;
	cout << "Enter key: ";
	cin >> key;

	if (mode == "encryption") {
		string input;
		cout << "Write down your message:\n";
		cin.ignore();
		getline(cin, input);
		size_t len = input.size();
		char* output = new char[len];

		shifr(input.c_str(), output, len, key, false);

		ofstream out;
		out.open(file_name, ios::binary);
		out.write((char*)&len, sizeof(size_t));
		if (out.is_open())
		{
			for (size_t i = 0; i < len; ++i)
			{
				cout << output[i];
				out.write((char*)&output[i], sizeof(char));
			}
		}
		out.close();

	}
	else if (mode == "decryption") {
		ifstream in; 
		string input = "";
		size_t len;
		in.open(file_name, ios::binary);
		in.read((char*)&len, sizeof(size_t));
		char symbol;
		for (size_t i = 0; i < len; ++i)
		{
			in.read((char*)&symbol, sizeof(char));
			input += symbol;
		}
		in.close();

		char* output = new char[len];
		shifr(input.c_str(), output, len, key, true);

		for (size_t i = 0; i < len; ++i)
		{
			cout << output[i];
		}
	}
	return 0;
}
