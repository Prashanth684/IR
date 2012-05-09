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
  void printDocList(DocList& d);
  void processSingleQueryString(std::string& q,DocList& result);
  bool helper_sort(Document* first,Document* second);
 private:
  void processANDQueryString(DocList& list1,DocList& list2,DocList& result);
  void processORQueryString(DocList& list1,DocList& list2,DocList& result);
  Toker* toker;
  std::string stopWordFile;
  std::vector<std::string> files;
  DocList docs;
  InvertedIndex iIndex;
};

#endif
