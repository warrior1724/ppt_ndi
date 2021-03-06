#include "stdafx.h"

#include <stdlib.h>
#include <chrono>
#include <thread>
#include <string>
#include <windows.h>

#ifdef _WIN32

#ifdef _WIN64
#pragma comment(lib, "C:/Program Files/NewTek/NewTek NDI 3.5 SDK/Lib/x64/Processing.NDI.Lib.x64.lib")
#else // _WIN64
#pragma comment(lib, "C:/Program Files/NewTek/NewTek NDI 3.5 SDK/Lib/x86/Processing.NDI.Lib.x86.lib")
#endif // _WIN64

#endif

#include "C:/Program Files/NewTek/NewTek NDI 3.5 SDK/Examples/C++/NDIlib_Send_PNG/picopng.hpp"
#include <C:/Program Files/NewTek/NewTek NDI 3.5 SDK/Include/Processing.NDI.Lib.h>

int main(int argc, char* argv[])
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	NDIlib_send_create_t NDI_send_create_desc;
	NDIlib_send_instance_t pNDI_send;
	NDIlib_video_frame_v2_t NDI_video_frame;
	std::vector<unsigned char> png_data;
	std::vector<unsigned char> image_data;

	GetConsoleMode(hStdin, &mode);
	SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
	if (!NDIlib_initialize()) {
		printf("Could not initialize NDI.");
		return 0;
	}

	NDI_send_create_desc.p_ndi_name = "PPTNDI";
	pNDI_send = NDIlib_send_create(&NDI_send_create_desc);
	if (!pNDI_send) return 0;

	printf("Do not close this window\n");

	while (true) {
		std::string str;
		std::getline(std::cin, str);

		if (str == "destroy") {
			NDIlib_send_destroy(pNDI_send);
			NDIlib_destroy();
			continue;
		}

		loadFile(png_data, str);
		if (png_data.empty()) return 0;

		unsigned long xres = 0, yres = 0;
		if (decodePNG(image_data, xres, yres, &png_data[0], png_data.size(), true)) continue;

		NDI_video_frame.xres = xres;
		NDI_video_frame.yres = yres;
		NDI_video_frame.FourCC = NDIlib_FourCC_type_RGBA;
		NDI_video_frame.p_data = &image_data[0];
		NDI_video_frame.line_stride_in_bytes = xres * 4;
		NDIlib_send_send_video_v2(pNDI_send, &NDI_video_frame);
	}
	return 0;
}
