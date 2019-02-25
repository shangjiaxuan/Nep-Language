#include <iostream>
#include <string>
#include <sstream>

//Nep! Nep? Nep~ Nep,
//Nepu! Nepu? Nepu~ Nepu,

const char* word_table[8] {"nep!","nep?","nep~","nep,","nepu!","nepu?","nepu~","nepu,"};

std::string NepEncode(std::string str) {
	size_t size = str.size();
	//Find the length of text in bits
	size_t length = size * 8;
	//Find the lenghth required for a 3-bit encoding...
	length = (length / 3) + bool(length % 3);
	std::string rtn;
	rtn.reserve(length * 6);
	signed char offset = -3;
	size_t out_length = 0;
	bool capitalize = true;
	char digit = 0;
	for (size_t i = 0; i < size; i++) {
		unsigned char c = str[i];
		offset += 8;
		while (offset >= 0) {
			if (!rtn.empty()) rtn += ' ';
			digit |= ((c >> offset) & 0x07);
			offset -= 3;
			rtn += word_table[digit];
			if (capitalize) {
				rtn[out_length] -= 0x20;
			}
			switch (digit) {
				case 0: case 1: case 2:
					capitalize = true;
					out_length += 5;
					break;
				case 3:
					capitalize = false;
					out_length += 5;
					break;
				case 4: case 5: case 6:
					capitalize = true;
					out_length += 6;
					break;
				case 7:
					capitalize = false;
					out_length += 6;
					break;
				default:
					throw std::runtime_error("Encode: unknown digit encountered!");
			}
			digit = 0;
		}
		digit = (c << (-offset)) & 0x07;
	}
	if (digit) {
		if (!rtn.empty()) rtn += ' ';
		rtn += word_table[digit];
		if (capitalize) {
			rtn[out_length] -= 0x20;
		}
		switch (digit) {
		case 0: case 1: case 2:
			capitalize = true;
			out_length += 5;
			break;
		case 3:
			capitalize = false;
			out_length += 5;
			rtn[out_length - 1] = '.';
			break;
		case 4: case 5: case 6:
			capitalize = true;
			out_length += 6;
			break;
		case 7:
			capitalize = false;
			out_length += 6;
			rtn[out_length - 1] = '.';
			break;
		default:
			throw std::runtime_error("Encode: unknown digit encountered!");
		}
		digit = 0;
	}
	rtn.shrink_to_fit();
	return rtn;
}

std::string NepDecode(std::string str) {
	std::istringstream iss(str);
	std::string word;
	unsigned char byte = 0;
	signed char offset = 5;
	std::string rtn;
	rtn.reserve((str.size() / 5 + 1) * 3 / 8 + 1);
	char digit = 0;
	while (iss >> word) {
		//decode one word into a 3-bit digit
		switch (word.size()) {
		case 4:
			switch (word[3]) {
			case '!': break;
			case '?': digit += 1; break;
			case '~': digit += 2; break;
			case ',': case '.': digit += 3; break;
			default:
				throw std::runtime_error("NepDecode: unknown word expression");
			}
			break;
		case 5:
			digit += 4;
			switch (word[4]) {
			case '!': break;
			case '?': digit += 1; break;
			case '~': digit += 2; break;
			case ',': case '.': digit += 3; break;
			default:
				throw std::runtime_error("NepDecode: unknown word expression");
			}
			break;
		default:
			throw std::runtime_error("NepDecode: unknown word length");
		}
		byte |= (digit << offset);
		offset -= 3;
		if(offset < 0) {
			digit = 0;
			if (iss >> word) {
				switch (word.size()) {
				case 4:
					switch (word[3]) {
					case '!': break;
					case '?': digit += 1; break;
					case '~': digit += 2; break;
					case ',': case '.': digit += 3; break;
					default:
						throw std::runtime_error("NepDecode: unknown word expression");
					}
					break;
				case 5:
					digit += 4;
					switch (word[4]) {
					case '!': break;
					case '?': digit += 1; break;
					case '~': digit += 2; break;
					case ',': case '.': digit += 3; break;
					default:
						throw std::runtime_error("NepDecode: unknown word expression");
					}
					break;
				default:
					throw std::runtime_error("NepDecode: unknown word length");
				}
			}
			else {
				break;
			}
			byte |= (digit >> -offset);
			rtn += byte;
			offset += 8;
			byte = (digit << offset);
			offset -= 3;
		}
		digit = 0;
	}
	if (byte) rtn += byte;
	rtn.shrink_to_fit();
	return rtn;
}

int main() {
	std::string str;
	std::getline(std::cin, str);
	std::cout << std::endl;
	std::string NepLang = NepEncode(str);
	std::cout << NepLang << '\n' << std::endl;
	std::cout << NepDecode(NepLang) << '\n' << std::endl;
	system("pause");
	return 0;
}
