#include <cstdio>
#include <stdint.h>
#include <bitset>
#include <locale>
#include <codecvt>
#include <string>
#include "utils.h"

std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstring_converter;
std::string wstr_to_str(std::wstring str) {
  return wstring_converter.to_bytes(str);
}

std::wstring str_to_wstr(std::string str) {
  return wstring_converter.from_bytes(str);
}

void vector3_to_arr(Vector3 vector, double* array) {
  array[0] = vector.x;
  array[1] = vector.y;
  array[2] = vector.z;
}

void print_array(unsigned char buf[], size_t len, bool newline) {
  // Print out the returned buffer.
  for (uint i = 0; i < len; i++) {
    printf("%02hhx ", buf[i]);
  };
  if (newline) printf("\n");
}

void print_vector(Vector3 vector, bool newline) {
  // Print out the returned buffer.
  double arr[3];
  vector3_to_arr(vector, arr);
  printf("( ");
  for (uint i = 0; i < 3; i++) {
    printf("%.6f ", arr[i]);
  };
  printf(")");
  if (newline) printf("\n");
}

void print_array_as_binary(unsigned char buf[], size_t len, bool newline) {
  // Print out the returned buffer.
  for (uint i = 0; i < len; i++) {
    printf("%s ", std::bitset<8>(buf[i]).to_string().c_str());
  };
  if (newline) printf("\n");
}
