#ifndef UTILS_H
#define UTILS_H

struct Vector3 {
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;
};

struct Int16_Vector3 {
  int16_t x = 0;
  int16_t y = 0;
  int16_t z = 0;
};

std::string wstr_to_str(std::wstring str);
std::wstring str_to_wstr(std::string str);
void vector3_to_arr(Vector3 vector, double* array);
void print_vector(Vector3 vector, bool newline = true);
void print_array(unsigned char buf[], size_t len, bool newline = true);
void print_array_as_binary(unsigned char buf[], size_t len, bool newline = true);

#endif /* end of include guard: UTILS_H */
