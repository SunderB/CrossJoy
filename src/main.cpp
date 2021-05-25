/*******************************************************
 Windows HID simplification
 Alan Ott
 Signal 11 Software
 8/22/2009
 Copyright 2009
 This contents of this file may be used by anyone
 for any reason without any conditions and may be
 used as a starting point for your own applications
 which use HIDAPI.
********************************************************/

//#define _GLIBCXX_USE_CXX11_ABI 0

#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <map>

// Headers needed for sleeping.
#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

#include "hidapi/hidapi.h"

#include "utils.h"
#include "joycon.h"

template <typename T> std::string type_name();

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;

	int res;

  unsigned char buf[256];
  unsigned char out_buf[256];

  #define MAX_STR 255
	wchar_t wstr[MAX_STR];
	int i;

	struct hid_device_info *devs, *cur_dev;

	printf("JoyCon hidapi test/example tool. Compiled with hidapi version %s, runtime version %s.\n", HID_API_VERSION_STR, hid_version_str());
	if (hid_version()->major == HID_API_VERSION_MAJOR && hid_version()->minor == HID_API_VERSION_MINOR && hid_version()->patch == HID_API_VERSION_PATCH) {
		printf("Compile-time version matches runtime version of hidapi.\n\n");
	}
	else {
		printf("Compile-time version is different than runtime version of hidapi.\n]n");
	}

	if (hid_init()) {
    printf("Failed to initialise hidapi.");
		return -1;
  }

	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;
	uint32_t joycon_vid = 0;
	uint32_t joycon_pid = 0;
  std::map<std::wstring, JoyCon*> joycons;
  std::vector<std::wstring> joycon_serial_nos;

	while (cur_dev) {
		//std::cout << cur_dev->path)>() << '\n';
		//if (std::string(cur_dev->vendor_id).compare("/dev/hidraw1") == 0) {
    if (cur_dev->vendor_id == 0x057e) {
      std::wstring serial_no = std::wstring(cur_dev->serial_number);
      switch (cur_dev->product_id) {
        case 0x2006:
          printf("Found Left Joycon with serial no. %ls\n", serial_no.c_str());
          //joycon_serial_nos.push_back(serial_no);
          joycons.insert(std::make_pair(
            serial_no,
            new JoyCon(
              JoyConType::left,
              wstr_to_str(serial_no).c_str()
            )
          ));
          break;

        case 0x2007:
          printf("Found Right Joycon with serial no. %ls\n", serial_no.c_str());
          joycons.insert(std::make_pair(
            serial_no,
            new JoyCon(
              JoyConType::right,
              wstr_to_str(serial_no).c_str()
            )
          ));
          break;

        default:
          printf("Not a Joycon");
          break;
      }
    	//joycon_vid = cur_dev->vendor_id;
			//joycon_pid = cur_dev->product_id;

		}
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		printf("  Release:      %hx\n", cur_dev->release_number);
		printf("  Interface:    %d\n",  cur_dev->interface_number);
		printf("  Usage (page): 0x%hx (0x%hx)\n", cur_dev->usage, cur_dev->usage_page);
		printf("\n");
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);
  //printf("Joycon\n vendor id: 0x%04hx\n product id: 0x%04hx\n", joycon_vid, joycon_pid);

  // for (size_t i = 0; i < joycon_serial_nos.size(); i++) {
  //    joycons.insert(std::make_pair(
  //      joycon_serial_nos[i],
  //      new JoyCon(
  //        wstr_to_str(joycon_serial_nos[i]).c_str()
  //      )
  //    ));
  // }


	/* Free static HIDAPI objects. */
	hid_exit();

#ifdef WIN32
	system("pause");
#endif

	return 0;
}
