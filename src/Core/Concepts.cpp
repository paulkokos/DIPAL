// src/Core/Concepts.cpp
#include "../../include/DIPAL/Core/Concepts.hpp"

#include <string>
#include <typeinfo>

namespace DIPAL {
// Since C++ concepts are compile-time only constructs,
// this file privides runtime utilities for working with the concepts

/*
* @brief Get a human readable name for common pixel types
*/

std::string getPixelTypeName(const std::type_info& type) {
  if (type == typeid(std::uint8_t)) return "uint8";
  if (type == typeid(std::uint16_t)) return "uint16";
  if (type == typeid(std::uint32_t)) return "uint32";
  if (type == typeid(std::int8_t)) return "int8";
  if (type == typeid(std::int16_t)) return "int16";
  if (type == typeid(std::int32_t)) return "int32";
  if (type == typeid(float)) return "float";
  if (type == typeid(double)) return "double";
  if (type == typeid(bool)) return "bool";
  return type.name;
}
} //namespace DIPAL
