#include "Document.h"
#include<boost/foreach.hpp>

Document::Document(std::string& filename,Toker* t)
  :file(filename),
   toker(t)
{
  docName=filename.substr(filename.find_last_of("/\\")+1);
  docID=global_doc_id;
  ++global_doc_id;
}

Document::~Document()
{
}

bool
Document::isDocValid()
{
  if (toker)
    return toker->getIsValid();
  return false;    
}

LinkedList&
Document::getTokenList()
{
  return tokens;
}

unsigned int&
Document::getDocID()
{
  return docID;
}

std::string&
Document::getDocName()
{
  return docName;
}

bool
Document::processDocument()
{
  return (toker->tokenize(file,tokens,attributeMap));
}

int
Document::getTermCountAttribute(std::string& term)
{
  TermAttributes* ta=attributeMap[term];
  if(ta)
    return(ta->getCount());
  else
    return 0;
}

void
Document::printDetails()
{
  std::cout<<"Doc Name: "<<docName<<"\n";
  std::cout<<"Doc ID: "<<docID<<"\n";
  BOOST_FOREACH(std::string t,tokens)
  {
    std::cout<<"Doc Token (sample): "<<t<<"\n";
    break;
  }
}
