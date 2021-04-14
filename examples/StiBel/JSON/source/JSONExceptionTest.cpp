#include "StiBel/JSON/JSONException.h"

using StiBel::Exception;
using StiBel::JSON::JSONException;

int main()
{
  try
  {
    throw JSONException("Attempt to set value on an empty stack");
  }
  catch (StiBel::Exception &e)
  {
    std::cout << "StiBel::Exception caught" << std::endl;
    std::cout << e.what() << std::endl;
    std::cout << e.displayText() << std::endl;
  }

  return 0;
}