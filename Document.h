#ifndef _DOCUMENT
#define _DOCUMENT

#include "Toker.h"

class Document
{
 public:
  Document(std::string& filename,Toker* t);
  ~Document();
  bool processDocument();
  LinkedList& getTokenList();
  unsigned int& getDocID();
  std::string& getDocName();
  int getTermCountAttribute(std::string& term);
  bool isDocValid();
  void printDetails();
 private:
  Toker* toker;
  unsigned int docID;
  std::string docName;
  std::string file;
  LinkedList tokens;
  AttrMap attributeMap;
};

#endif
