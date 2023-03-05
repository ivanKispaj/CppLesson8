//
// BadAnswer.cpp
// 2023-02-03
// Ivan Konishchev
//

#include "BadAnswer.h"
#include <iostream>

BadAnswer::BadAnswer(ArrayError errorCode) : _errorCode(errorCode) {}

const char *BadAnswer::showError() const
{
  switch (_errorCode)
  {
  case 0:
    return "Value not found!\n";
  case 1:
    return "Index out of range!\n";
  case 2:
    return "Array is empty!\n";
  case 3:
    return "Bad array leinght!\n";
  case 4:
    return "Incorrect array leinght, create empty array!\n";
  }
}