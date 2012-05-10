#ifndef _TOKER
#define _TOKER

#include "ContainerDefs.h"
#include <stdio.h>

class TermAttributes
{
 public:
  TermAttributes()
    :termCount(1){}
  ~TermAttributes(){}
  int getCount(){return termCount;}
  void increaseCount(){++termCount;}
 private:
  int termCount;
  //in future prob add term location
};


class Toker
{
 public:
  Toker(std::string& filename);
  ~Toker();
  bool tokenize(std::string& filename,LinkedList& tokens,AttrMap& atMap);
  bool getIsValid();
 private:
  bool initializeStopWords();
  bool removeStops(Tokenizer& tok,LinkedList& tokens,AttrMap& atMap);
  std::vector<std::string> stops;
  std::string stopFile;
  bool isValid;
};

#endif
