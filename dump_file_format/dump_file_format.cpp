#include <fstream>
#include <iostream>
#include <wchar.h>
#include <vector>

namespace {
	long line_sz = 32;

	constexpr auto ofile_postfix = L"_bytes.txt";

	constexpr auto first_byte_prefix = "0x";
	constexpr auto new_line_byte_prefix = ",\n0x";
	constexpr auto byte_prefix = ", 0x";

	constexpr char hex_ascii_map[] = {
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
		0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46
	};
};

void fill_stream(std::ofstream& ostream, std::vector<uint8_t> buffer, std::size_t line_sz) {
	char _c[2];
	for (std::size_t i = 0; i < buffer.size(); i++) {
		if (i == 0) {
			ostream.write(first_byte_prefix, 2);
		}
		else if (i % line_sz == 0) {
			ostream.write(new_line_byte_prefix, 4);
		}
		else {
			ostream.write(byte_prefix, 4);
		}
		_c[0] = hex_ascii_map[buffer[i] >> 4];
		_c[1] = hex_ascii_map[buffer[i] & 0xF];
		ostream.write(_c, 2);
	}
}

int wmain(int argc, wchar_t* argv[]) {
	if (argc < 2) {
		std::cout << "Pass file to dump, line size (optional), output dir (optional)";
		return -1;
	}
	if (argc > 2) {
		line_sz = std::wcstol(argv[2], nullptr, 10);
		if (line_sz < 1) {
			std::cout << "Bad line size arg";
			return -1;
		}
	}
	std::wstring out_f_path = argv[1];
	out_f_path.append(ofile_postfix);
	if (argc > 3) {
		out_f_path = argv[3];
	}

	std::ifstream file_ifstream(argv[1], std::ios::binary | std::ios::in);
	if (!file_ifstream.good()) {
		std::cout << "Bad file path argument";
		return -1;
	}
	std::ofstream file_ofstream(out_f_path, std::ios::binary | std::ios::out);
	if (!file_ofstream.good()) {
		std::cout << "Bad file outuput path argument";
		return -1;
	}

	std::vector<uint8_t> _buffer;
	_buffer.assign((std::istreambuf_iterator<char>(file_ifstream)), std::istreambuf_iterator<char>());
	file_ifstream.close();

	printf("writing to: %S", out_f_path.c_str());

	fill_stream(file_ofstream, _buffer, line_sz);
	file_ofstream.close();

	return 0;
}