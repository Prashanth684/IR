#ifndef _RECORDS
#define _RECORDS

#include "Document.h"

class CreateRecords
{
 public:
  CreateRecords(std::vector<std::string>& fileList,std::string& stopFile);
  ~CreateRecords();
  bool initTokensAndDocuments();
  bool indexAndCreateII();
  void printDetails();
 private:
  Toker* toker;
  std::string stopWordFile;
  std::vector<std::string> files;
  DocList docs;
  InvertedIndex iIndex;
};

#endif
