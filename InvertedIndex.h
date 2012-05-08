#ifndef _INVERTED_INDEX
#define _INVERTED_INDEX 

#include "ContainerDefs.h"

class InvertedIndex
{
  InvertedIndex(std::string t,DocList& d)
    :term(t),
    docList(d)
  ~InvertedIndex(){}
  std::string& getTerm(){return term;}
  DocList& getDocList(){return docList;}
 private:
  std::string term;
  DocList docList;
};

#endif
