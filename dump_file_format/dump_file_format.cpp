#include <fstream>
#include <iostream>
#include <wchar.h>
#include <vector>

namespace {
	constexpr wchar_t ofile_postfix[] = L"_barr.txt";
	auto line_sz = 32;
	constexpr char hex_ascii_map[] = {
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46
	};
};

void write_file(std::ofstream& ostream, std::vector<uint8_t> buffer, std::size_t line_sz) {
	char _c[2]{};
	for (std::size_t i = 0; i < buffer.size(); i++) {
		if (i == 0) {
			ostream.write("0x", 2);
		}
		else if (i % line_sz == 0) {
			ostream.write(",\n0x", 4);
		}
		else {
			ostream.write(", 0x", 4);
		}
		_c[0] = hex_ascii_map[(buffer[i] >> 4)];
		_c[1] = hex_ascii_map[(buffer[i] & 0xF)];
		ostream.write(_c, 2);
	}
}

int wmain(int argc, wchar_t* argv[])
{
	if (argc < 2) {
		std::cout << "Pass file to dump, (optional) line size";
		return -1;
	}
	if (argc > 2) {
		line_sz = std::wcstol(argv[2], nullptr, 10);
	}
	std::wstring inp_f_path = (argv[1]);
	std::ifstream file_ifstream(inp_f_path, std::ios::binary | std::ios::in);

	std::vector<uint8_t> _buffer;
	_buffer.assign((std::istreambuf_iterator<char>(file_ifstream)), std::istreambuf_iterator<char>());
	file_ifstream.close();

	inp_f_path.append(ofile_postfix);
	printf("writing to: %S\n", inp_f_path.c_str());
	std::ofstream file_ofstream(inp_f_path, std::ios::binary | std::ios::out);
	write_file(file_ofstream, _buffer, line_sz);

	file_ofstream.close();

	return 0;
}