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
  void processANDQueryString(std::string& q);
  bool helper_sort(Document* first,Document* second);
 private:
  Toker* toker;
  std::string stopWordFile;
  std::vector<std::string> files;
  DocList docs;
  InvertedIndex iIndex;
};

#endif
