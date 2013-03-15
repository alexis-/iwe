#ifndef RE_COMMON_UTILS_H
# define RE_COMMON_UTILS_H

# include <string>
# include <algorithm>

static std::string upper_string(const std::string& str)
{
  std::string upper;
  std::transform(str.begin(), str.end(), std::back_inserter(upper), std::toupper);
  return upper;
}

static bool hasCIEnding(std::string const &fullString, std::string const &ending)
{
	std::string newFullString = upper_string(fullString);
	std::string newEnding = upper_string(ending);
  unsigned int lastMatchPos = newFullString.rfind(newEnding); // Find the last occurrence of ending
  bool isEnding = lastMatchPos != std::string::npos; // Make sure it's found at least once

  // If the string was found, make sure that any characters that follow it are the ones we're trying to ignore
  for(int i = lastMatchPos + newEnding.length(); (i < newFullString.length()) && isEnding; i++)
  {
    if((newFullString[i] != '\n') &&
        (newFullString[i] != '\r') )
    {
      isEnding = false;
    }
  }

  return isEnding;
}


#endif // !RE_COMMON_UTILS_H