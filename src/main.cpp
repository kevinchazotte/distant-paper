#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "absl/strings/str_join.h"
#include "../proto/test.pb.h"

int main() {
	TestMessage message;
	message.set_text("testing this message");
	message.set_number(2);
	std::string serialized_string;
	message.SerializeToString(&serialized_string);
	std::cout << "Serialized data: " << serialized_string << std::endl;
	std::ofstream output("message.data", std::ios::binary);
	message.SerializeToOstream(&output);
	output.close();
	std::vector<std::string> words = { "hello", "world", "from", "abseil" };
	std::string joined = absl::StrJoin(words, "-");
	std::cout << joined << std::endl;
	return 0;
}
