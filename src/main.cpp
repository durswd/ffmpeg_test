
#define STB_IMAGE_IMPLEMENTATION
#include "../thirdparty/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../thirdparty/stb/stb_image_write.h"

#include "../thirdparty/tiny-process-library/process.hpp"

#include <iostream>
#include <regex>
#include <cstdio>
#include <vector>

int main()
{
	std::string filepath = "input.mp4";
	// get info
	int width = 0;
	int height = 0;

	{
		std::string command = "ffmpeg -i " + filepath;
		TinyProcessLib::Process process1(command, "",
			[](const char* bytes, size_t n) {},
			[&](const char* bytes, size_t n) {


				std::regex re("( [0-9]+x[0-9]+ )");
				std::smatch match;
				auto s = std::string(bytes, n);
				std::cout << s << std::endl;
				if (std::regex_search(s, match, re))
				{
					std::sscanf(match[0].str().c_str(), " %dx%d ", &width, &height);
				}
			});
		auto exit_status = process1.get_exit_status();
	}

	// load
	{
		// TODO : support padding
		int imageSize = 14 + 40 + width * height * 3;
		std::vector<char> data;
		data.reserve(imageSize * 2);

		int count = 0;
		int num = 0;
		TinyProcessLib::Process process1("ffmpeg -i input.mp4 -c:v bmp -f rawvideo pipe:1", "", [&](const char* bytes, size_t n) {
			const auto offset = data.size();
			data.resize(offset + n);
			memcpy(data.data() + offset, bytes, n);

			if (data.size() >= imageSize)
			{
				
				auto dst = "img_" + std::to_string(num) + ".png";

				// TODO swap b and r
				stbi_write_png(dst.data(), width, height, 3, data.data() + 54, width * 3);
				num++;

				data.erase(data.begin(), data.begin() + imageSize);
			}

			// bmp
			std::cout << n << std::endl;
			count += n;
			});
		auto exit_status = process1.get_exit_status();
		std::cout << "Example 1 process returned: " << exit_status << " (" << (exit_status == 0 ? "success" : "failure") << ")" << std::endl;
		std::cout << count << std::endl;
	}

	return 0;
}