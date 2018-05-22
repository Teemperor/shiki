#include <iostream>
#include <regex>
#include <unordered_map>

static const char *Red = "\x1b[31m";
static const char *Green = "\x1b[32m";
static const char *Yellow = "\x1b[33m";
static const char *Blue = "\x1b[34m";
static const char *Magenta = "\x1b[35m";
static const char *Cyan = "\x1b[36m";

bool hasSuffix(std::string const &Str, std::string const &Suffix) {
  if (Suffix.empty())
    return true;
  if (Str.length() >= Suffix.length())
    return (Str.compare(Str.length() - Suffix.length(), Suffix.length(),
                        Suffix) == 0);
  else
    return false;
}

static std::string addColor(const char *Color, const std::string &Token) {
  static const char *Reset = "\x1b[0m";
  return Color + Token + Reset;
}

bool colorize(std::string &Str, const std::string &Token, const char *Color) {
  size_t start_pos = Str.find(Token);
  if (start_pos == std::string::npos)
    return false;
  Str.replace(start_pos, Token.length(), addColor(Color, Token));
  return true;
}

struct Highlight {
  const char *Str;
  const char *Color;
  const char *Suffix;
};

static void handleLine(std::string Line) {

  std::regex NinjaStatus("\\[[0-9]+/[0-9]+\\]");
  std::smatch Match;
  if (std::regex_search(Line, Match, NinjaStatus)) {
    colorize(Line, Match[0].str(), Yellow);
  } else {
    std::cout << Line << std::endl;
    return;
  }

  static const std::vector<Highlight> ColorCodes = {
      {"Building C object", Green, ""},
      {"Building CXX object", Green, ""},
      {"Linking CXX static library", Magenta, ""},
      {"Updating ", Blue, "..."},
      {"Building ", Cyan, "..."},
      {"Generating ", Cyan, "..."},
      {"Creating export file for ", Cyan, "..."},
      {"Linking CXX executable ", Red, ""},
      {"Linking CXX shared module", Red, ""},
  };
  const char *LastUsedColor = nullptr;
  for (auto &P : ColorCodes) {
    if (hasSuffix(Line, P.Suffix)) {
      if (colorize(Line, P.Str, P.Color)) {
        LastUsedColor = P.Color;
      }
    }
  }
  if (LastUsedColor) {
    if (hasSuffix(Line, ".o")
        || hasSuffix(Line, ".a")
           || hasSuffix(Line, "...")) {
      std::size_t StartOfLastWord = Line.size() - 1;
      for (; StartOfLastWord >= 0; StartOfLastWord--) {
        if (Line.at(StartOfLastWord) == ' ')
          break;
      }
      std::string LastWord = Line.substr(StartOfLastWord);
      colorize(Line, LastWord, LastUsedColor);
    }
  }

  std::cout << Line << std::endl;
}

int main() {
  std::string InputLine;
  while (std::cin) {
    getline(std::cin, InputLine);
    handleLine(InputLine);
  };
}