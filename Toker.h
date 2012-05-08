#ifndef _TOKER
#define _TOKER

#include "ContainerDefs.h"
#include <stdio.h>

class Toker
{
 public:
  Toker(std::string& filename);
  ~Toker();
  bool tokenize(std::string& filename,LinkedList& tokens);
  bool getIsValid();
 private:
  bool initializeStopWords();
  bool removeStops(Tokenizer& tok,LinkedList& tokens);
  std::vector<std::string> stops;
  std::string stopFile;
  bool isValid;
};

#endif
