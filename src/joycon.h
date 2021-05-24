#ifndef JOYCON_H
#define JOYCON_H

#include <map>
#include <string>
#include <stdint.h>
#include "utils.h"
#include "/usr/include/hidapi/hidapi.h"

enum ReportType {
  subcommand_reply = 0x21,
  nfc_ir_fw_update = 0x23,
  full_imu_report = 0x30,
  nfc_ir_mcu_mode = 0x31,
};

struct SixAxisData {
  int time = 0;
  Vector3 accel;
  Vector3 gyro;
};

struct Buttons {
  bool down : 1;
  bool up : 1;
  bool right : 1;
  bool left : 1;
  bool sr_l : 1;
  bool sl_l : 1;
  bool l : 1;
  bool zl : 1;

  bool minus : 1;
  bool plus : 1;
  bool r_stick : 1;
  bool l_stick : 1;
  bool home : 1;
  bool capture : 1;
  bool : 1;
  bool charging_grip : 1;

  bool y : 1;
  bool x : 1;
  bool b : 1;
  bool a : 1;
  bool sr_r : 1;
  bool sl_r : 1;
  bool r : 1;
  bool zr : 1;
};

enum JoyConType {
  left = 0x2006,
  right = 0x2007
};

struct JoyConStatus {
  unsigned char type;
  unsigned char timer;
  uint8_t battery_level;
  unsigned char button_status[3];
  unsigned char left_stick[3];
  unsigned char right_stick[3];
  unsigned char vibrator_report;
  unsigned char data[37];
};

// struct JoyconStatusBitField {
//   unsigned char type : 8;
//   unsigned char timer : 8;
//   uint8_t battery_level : 4;
//   uint8_t connection_info : 4;
//   unsigned char button_status[3] : 12;
//   unsigned char left_stick[3] : 12;
//   unsigned char right_stick[3] : 12;
//   unsigned char vibrator_report : 8;
//   unsigned char data[37] : 37*8;
// };


enum InputReportMode {

};

enum BatteryLevel {
  empty = 0,
  critical = 2,
  low = 4,
  medium = 6,
  full = 8
};

class JoyCon {
private:
  const uint report_len = 49;
  const unsigned char default_buf[8] = { 0x0, 0x1, 0x40, 0x40, 0x0, 0x1, 0x40, 0x40 };

  const uint vendor_id = 0x057e;
  uint type;
  std::string serial_number;

  std::map<unsigned char, std::string> report_types = {
    {0x21, "Subcommand reply"},
    {0x23, "NFC/IR firmware update"},
    {0x30, "Full report w/ 6-axis data"},
    {0x31, "NFC/IR MCU mode"}
  };

  uint8_t global_count = 0;

  hid_device* handle;
  //JoyConInfo device_info;

  int ReceiveRaw(unsigned char* data);

public:
  JoyCon(uint type, const char* serial_no);
  virtual ~JoyCon();

  uint64_t timestamp = 0;
  // For UdpServer
  int padId = 0;
  int battery = -1;
  int model = 2;
  int constate = 2;
  int connection = 3;

  void subcommand(unsigned char sc, unsigned char buf[], uint len, unsigned char response[]);

  void blink_home_light();
  void set_player_led(unsigned char leds);
  void enable_imu(bool enable);
  void enable_rumble(bool enable);
  void set_input_report_mode(uint8_t mode);
  void print_report(JoyConStatus jc_status);
  void get_6axis_data(unsigned char* sensor_data, SixAxisData samples[3]);
  Buttons get_buttons(unsigned char* button_status);
  void print_buttons(Buttons buttons);
};

#endif /* end of include guard: JOYCON_H */
