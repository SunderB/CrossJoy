#include <cstring>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <map>
#include <bitset>

#ifdef __linux__
  #include "/usr/include/hidapi/hidapi.h"
#elif defined(_WIN32)
  #include "hidapi.h"
#endif

// Headers needed for sleeping.
#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

template <typename T> std::string type_name();
#define MAX_STR 255

#include "joycon.h"
#include "utils.h"

JoyCon::JoyCon(uint jc_type, const char* serial_no) {
  int res;
  unsigned char buf[256];
  unsigned char out_buf[256];

	wchar_t wstr[255];

  type = jc_type;
  serial_number = std::string(serial_no);

  printf("Joycon\n vendor id: 0x%04hx\n product id: 0x%04hx\n", vendor_id, type);

	// Set up the command buffer.
	memset(buf,0x00,sizeof(buf));
	buf[0] = 0x01;
	buf[1] = 0x81;


	// Open the device using the VID, PID,
	// and optionally the Serial number.
	////handle = hid_open(0x4d8, 0x3f, L"12345"serial_no.c_str());
	handle = hid_open(vendor_id, type, str_to_wstr(serial_number).c_str());
	if (!handle) {
    std::wstring err_str;
    err_str = hid_error(NULL);

		printf("unable to open device\n");
    printf("Error info: %ls \n", hid_error(NULL));

    if (err_str == L"Permission denied") {
      printf("(Try running this program using sudo)\n");
    }

 		//return 1;
	}
  printf("Handle: %8hhx\n", handle);

	// Read the Manufacturer String
	wstr[0] = 0x0000;
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read manufacturer string\n");
	printf("Manufacturer String: %ls\n", wstr);

	// Read the Product String
	wstr[0] = 0x0000;
	res = hid_get_product_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read product string\n");
	printf("Product String: %ls\n", wstr);

	// Read the Serial Number String
	wstr[0] = 0x0000;
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read serial number string\n");
	printf("Serial Number String: (%d) %ls", wstr[0], wstr);
	printf("\n");

	// Read Indexed String 1
	//wstr[0] = 0x0000;
	//res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	//if (res < 0)
	//	printf("Unable to read indexed string 1\n");
	//printf("Indexed String 1: %ls\n", wstr);

	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 1);

	// Try to read from the device. There should be no
	// data here, but execution should not block.
	res = hid_read(handle, buf, 17);

  // Make sure command is received
  //hid_set_nonblocking(handle, 0);
  buf[0] = 0x80;
  buf[1] = 0x1;
  hid_write(handle, buf, 2);
  hid_read_timeout(handle, buf, 64, 100);
  printf("%02hhx", buf[0]);
  //if (buf[0] != 0x81) { // can occur when USB connection isn't closed properly
  //    printf("Resetting USB connection.\n");
  //    subcommand(0x06, new unsigned char[1] { 0x01 }, 1);
  //    //throw new Exception("reset_usb");
  //    exit(1);
  //}
  //
  // //if (buf[3] == 0x3) {
  // //    PadMacAddress = new PhysicalAddress(new unsigned char[6] { buf[9], buf[8], buf[7], buf[6], buf[5], buf[4] });
  // //}
  //
  // // USB Pairing
  // memset(buf,0,sizeof(buf));
  //
  // buf[0] = 0x80; buf[1] = 0x2; // Handshake
  // hid_write(handle, buf, 2);
  // hid_read_timeout(handle, buf, 64, 100);
  //
  // buf[0] = 0x80; buf[1] = 0x3; // 3Mbit baud rate
  // hid_write(handle, buf, 2);
  // hid_read_timeout(handle, buf, 64, 100);
  //
  // buf[0] = 0x80; buf[1] = 0x2; // Handshake at new baud rate
  // hid_write(handle, buf, 2);
  // hid_read_timeout(handle, buf, 64, 100);
  //
  // buf[0] = 0x80; buf[1] = 0x4; // Prevent HID timeout
  // hid_write(handle, buf, 2); // doesn't actually prevent timout...
  // hid_read_timeout(handle, buf, 64, 100);
  //
  // //dump_calibration_data();
  //

  // Bluetooth manual pairing
  //unsigned char[] btmac_host = Program.btMAC.GetAddressBytes();
  //send host MAC and acquire Joycon MAC
  //unsigned char[] reply = subcommand(0x01, new unsigned char[] { 0x01, btmac_host[5], btmac_host[4], btmac_host[3], btmac_host[2], btmac_host[1], btmac_host[0] }, 7, true);
  //unsigned char[] LTKhash = subcommand(0x01, new unsigned char[] { 0x02 }, 1, true);
  //save pairing info
  //subcommand(0x01, new unsigned char[] { 0x03 }, 1, true);

  blink_home_light();
  set_player_led(0b00000001);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  //set_player_led(0b00000011);
  //std::this_thread::sleep_for(std::chrono::seconds(1));
  //subcommand(0x40, new unsigned char[1] { (imu_enabled ? (byte)0x1 : (byte)0x0) }, 1);
  enable_imu(true);
  enable_rumble(true);
  set_input_report_mode(0x30);

  printf("Done with init.\n");

  hid_set_nonblocking(handle, 1);

  unsigned char response[report_len];
  for (size_t i = 0; i < 10; i++) {
    int res = hid_read_timeout(handle, response, report_len, 100);
    if (res < 1) {
      printf("No response.\n");
    } else {
      printf("Response recieved:\n");
      ReceiveRaw(response);
    }
  }

	// Send a Feature Report to the device
	//buf[0] = 0x2;
	//buf[1] = 0xa0;
	//buf[2] = 0x0a;
	//buf[3] = 0x00;
	//buf[4] = 0x00;
	//res = hid_send_feature_report(handle, buf, 17);
	//if (res < 0) {
	//	printf("Unable to send a feature report.\n");
	//}

	// memset(buf,0,sizeof(buf));
  //
	// // Read a Feature Report from the device
	// buf[0] = 0x2;
	// res = hid_get_feature_report(handle, buf, sizeof(buf));
	// if (res < 0) {
	// 	printf("Unable to get a feature report.\n");
	// 	printf("%ls", hid_error(handle));
	// }
	// else {
	// 	// Print out the returned buffer.
	// 	printf("Feature Report\n   ");
	// 	for (i = 0; i < res; i++)
	// 		printf("%02hhx ", buf[i]);
	// 	printf("\n");
	// }

	//memset(buf,0,sizeof(buf));

	// Toggle LED (cmd 0x80). The first byte is the report number (0x1).
	//buf[0] = 0x1;
	//buf[1] = 0x80;
	//res = hid_write(handle, buf, 17);
	//if (res < 0) {
	//	printf("Unable to write()\n");
	//	printf("Error: %ls\n", hid_error(handle));
	//}


	// Request state (cmd 0x81). The first byte is the report number (0x1).


	// Read requested state. hid_read() has been set to be
	// non-blocking by the call to hid_set_nonblocking() above.
	// This loop demonstrates the non-blocking nature of hid_read().
	// res = 0;
	// while (res == 0) {
	// 	res = hid_read(handle, buf, sizeof(buf));
	// 	if (res == 0)
	// 		printf("waiting...\n");
	// 	if (res < 0)
	// 		printf("Unable to read()\n");
	// 	#ifdef WIN32
	// 	Sleep(500);
	// 	#else
	// 	usleep(500*1000);
	// 	#endif
	// }
  //
	// printf("Data read:\n   ");
	// // Print out the returned buffer.
	// for (i = 0; i < res; i++)
	// 	printf("%02hhx ", buf[i]);
	// printf("\n");
}

void JoyCon::subcommand(unsigned char sc, unsigned char buf[], uint len, unsigned char response[]) {
    unsigned char buf_i[report_len];
    std::memcpy(buf_i, &default_buf[2], 8*sizeof(char));
    std::memcpy(&buf_i[11], buf, len*sizeof(char));

    buf_i[10] = sc;
    buf_i[1] = global_count;
    buf_i[0] = 0x1;

    if (global_count == 0xf) {
      global_count = 0;
    } else {
      global_count += 1;
    }

    //PrintArray(buf_, DebugType.COMMS, len, 11, "Subcommand 0x" + string.Format("{0:X2}", sc) + " sent. Data: 0x{0:S}");
    hid_write(handle, buf_i, len + 11);

    int tries = 0;
    while (tries < 10 && response[0] != 0x21 && response[14] != sc) {
        int res = hid_read_timeout(handle, response, report_len, 100);
        if (res < 1) {
          printf("No response.\n");
        } else {
          printf("Response recieved:\n");
          print_array(response, res);
        }
        tries += 1;
    }

    ReceiveRaw(response);
}



void JoyCon::blink_home_light() {
  printf("Blinking home light\n");
  unsigned char buf[25];
  unsigned char response[report_len];

  memset(buf,0xFF,sizeof(buf));
  buf[0] = 0x18;
  buf[1] = 0x01;

  subcommand(0x38, buf, 25, response);
}


void JoyCon::set_input_report_mode(uint8_t mode) {
  printf("Setting input report mode to 0x%02hhx\n", mode);
  unsigned char response[report_len];
  subcommand(0x03, new unsigned char[1] { mode }, 1, response);
}

void JoyCon::set_player_led(unsigned char leds) {
  printf("Attempting to set JoyCon LEDs...\n");
  unsigned char buf[1] = {leds};
  unsigned char response[report_len];
  subcommand(0x30, &leds, 1, response);
}

void JoyCon::enable_imu(bool enable) {
  unsigned char response[report_len];
  if (enable) {
    printf("Enabling IMU (6-axis sensors)\n");
    subcommand(0x40, new unsigned char[1] { 0x01 }, 1, response);
  } else {
    printf("Disabling IMU (6-axis sensors)\n");
    subcommand(0x40, new unsigned char[1] { 0x00 }, 1, response);
  }
}

void JoyCon::enable_rumble(bool enable) {
  unsigned char response[report_len];
  if (enable) {
    printf("Enabling rumble\n");
    subcommand(0x50, new unsigned char[1] { 0x01 }, 1, response);
  } else {
    printf("Disabling rumble\n");
    subcommand(0x50, new unsigned char[1] { 0x00 }, 1, response);
  }
}

unsigned char ts_en;

int JoyCon::ReceiveRaw(unsigned char* data) {
  if (!handle) {
    return -2;
  }

  unsigned char raw_buf[256];
  memcpy(raw_buf, data, report_len);

  unsigned char lag = (unsigned char)std::max(0, raw_buf[1] - ts_en - 3);

  JoyConStatus jc_status;
  jc_status.type = raw_buf[0];
  jc_status.timer = raw_buf[1];
  jc_status.battery_level = raw_buf[2] >> 4;
  memcpy(jc_status.button_status, &raw_buf[3], 3);
  memcpy(jc_status.left_stick, &raw_buf[6], 3);
  memcpy(jc_status.right_stick, &raw_buf[9], 3);
  jc_status.vibrator_report = raw_buf[12];
  memcpy(jc_status.data, &raw_buf[13], 37);

  timestamp += (uint64_t)lag * 5000; // add lag once

  print_report(jc_status);
  return 0;
}

void JoyCon::print_report(JoyConStatus jc_status) {
  printf("=======================\n");
  printf("Report type: 0x%02hhx (%s)\n", jc_status.type, report_types[jc_status.type].c_str());
  printf("Timer: 0x%02hhx\n", jc_status.timer);
  printf("Battery Level: %i/8\n", jc_status.battery_level);

  printf("Button status: ");
  Buttons buttons;
  buttons = get_buttons(jc_status.button_status);
  print_buttons(buttons);
  //print_array_as_binary(jc_status.button_status, 3);

  printf("Left | Right Sticks: ");
  print_array_as_binary(jc_status.left_stick, 3, false);
  printf("| ");
  print_array_as_binary(jc_status.right_stick, 3);

  printf("Vibrator: %s\n", jc_status.vibrator_report ? "On" : "Off");
  switch (jc_status.type) {
    case 0x21:
      printf("Subcommand ack: 0x%02hhx\n", jc_status.data[0]);
      printf("Subcommand id: 0x%02hhx\n",  jc_status.data[1]);
      printf("Subcommand reply data:\n");
      print_array(&jc_status.data[2], 35);
      break;
    case 0x30:
      printf("6-axis data:\n", jc_status.data[0]);
      print_array(jc_status.data, 37);
      SixAxisData samples[3];
      get_6axis_data(jc_status.data, samples);
      break;
  }
}

Buttons JoyCon::get_buttons(unsigned char* button_status) {
  Buttons buttons;

  buttons.down  = button_status[2] & 1;
  buttons.up    = button_status[2] & 2;
  buttons.right = button_status[2] & 4;
  buttons.left  = button_status[2] & 8;
  buttons.sr_l  = button_status[2] & 16;
  buttons.sl_l  = button_status[2] & 32;
  buttons.l     = button_status[2] & 64;
  buttons.zl    = button_status[2] & 128;

  buttons.minus         = button_status[1] & 1;
  buttons.plus          = button_status[1] & 2;
  buttons.r_stick       = button_status[1] & 4;
  buttons.l_stick       = button_status[1] & 8;
  buttons.home          = button_status[1] & 16;
  buttons.capture       = button_status[1] & 32;
  //buttons.l     = button_status[1] & 64;
  buttons.charging_grip = button_status[1] & 128;

  buttons.y     = button_status[0] & 1;
  buttons.x     = button_status[0] & 2;
  buttons.b     = button_status[0] & 4;
  buttons.a     = button_status[0] & 8;
  buttons.sr_r  = button_status[0] & 16;
  buttons.sl_r  = button_status[0] & 32;
  buttons.r     = button_status[0] & 64;
  buttons.zr    = button_status[0] & 128;

  //uint32_t data = (button_status[0] << 16) + (button_status[1] << 8) + button_status[2];
  // = {data};
  return buttons;
}

void JoyCon::print_buttons(Buttons buttons) {
  if (type == JoyConType::left) {
    printf("⯅: %i, ⯆: %i, ⯇: %i, ⯈: %i\n", buttons.up, buttons.down, buttons.left, buttons.right);
    printf("L-stick: %i\n", buttons.l_stick);
    printf("      L: %i,      ZL: %i\n", buttons.l, buttons.zl);
    printf("Left SL: %i, Left SR: %i\n", buttons.sl_l, buttons.sr_l);
    printf("      -: %i, Capture: %i\n", buttons.minus, buttons.capture);
  } else {
    //printf("\n");
    printf("🅐: %i, 🅑: %i, 🅧: %i, 🅨: %i\n", buttons.a, buttons.b, buttons.x, buttons.y);
    printf(" R-stick: %i\n",buttons.r_stick );
    printf("       R: %i,       ZR: %i \n", buttons.r, buttons.zr);
    printf("Right SL: %i, Right SR: %i\n", buttons.sl_r, buttons.sr_r);
    printf("       +: %i,     Home: %i\n", buttons.plus, buttons.home);
  }
}

void JoyCon::get_6axis_data(unsigned char* sensor_data, SixAxisData samples[3]) {
  for (size_t i = 0; i < 3; i++) {
    unsigned char* sample_data = &sensor_data[12*i];
    samples[i].time = 0;

    int16_t accel_x = (sample_data[0] << 8) + sample_data[1];
    int16_t accel_y = (sample_data[2] << 8) + sample_data[3];
    int16_t accel_z = (sample_data[4] << 8) + sample_data[5];

    int16_t gyro_x  = (sample_data[6] << 8) + sample_data[7];
    int16_t gyro_y  = (sample_data[8] << 8) + sample_data[9];
    int16_t gyro_z  = (sample_data[10] << 8) + sample_data[11];

    samples[i].accel = Vector3({
      accel_x * 0.000244,
      accel_y * 0.000244,
      accel_z * 0.000244
    });

    samples[i].gyro = Vector3({
      gyro_x * 0.06103,
      gyro_y * 0.06103,
      gyro_z * 0.06103
    });
    printf("Acceleration: ");
    print_vector(samples[i].accel, false);
    printf(" | Gyroscope: ");
    print_vector(samples[i].gyro);
  }
}

JoyCon::~JoyCon () {
  hid_close(handle);
}
