#include "Toker.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

unsigned int global_doc_id=1;

Toker::Toker(std::string& filename)
  :stopFile(filename),
   isValid(true)
{
  if (!initializeStopWords())
  {
    std::cout<<"Could not get stop words - check if file exists";
    isValid=false;
  }
}

Toker::~Toker()
{
}

bool
Toker::getIsValid()
{
  return isValid;
}

bool
Toker::initializeStopWords()
{
  std::ifstream stopwords(stopFile.c_str());
  if (!stopwords)
  {
    printf("Error:couldn't get stop words\n");
    return false;
  }

  char word[256];
  memset(word,'\0',256);
  
  while (stopwords.getline(word,256,'\n'))
  {
    if(!word[0])
      continue;
    stops.push_back(std::string(word));
    memset(word,'\0',256);
  }
  return true;
}

bool
Toker::tokenize(std::string& filename,LinkedList& tokens,AttrMap& atMap)
{
  std::ifstream f(filename.c_str());
  std::string tokenstring;
  if (f)
  {
    std::string tempFill((std::istreambuf_iterator<char>(f)),
	                  std::istreambuf_iterator<char>());
    tokenstring=tempFill.c_str();
  }
  else
  {
    std::cout<<"Could not open file for reading\n";
    return false;
  }

  Tokenizer tok(tokenstring);
  removeStops(tok,tokens,atMap);
  return true;
}

bool
Toker::removeStops(Tokenizer& tok,LinkedList& tokens,AttrMap& atMap)
{
  LinkedList::const_iterator it =tokens.begin();
  for(Tokenizer::iterator iter=tok.begin();iter!=tok.end();++iter)
  {
    std::string temp(*iter);
    boost::algorithm::to_lower(temp);
    if(std::find(stops.begin(),stops.end(),temp)==stops.end())
    {
      if(std::find(tokens.begin(),tokens.end(),temp)!=tokens.end())
      {
	TermAttributes* ta=atMap[temp];
	ta->increaseCount();
      }
      else
      {
	tokens.insert_after(it,temp);
	atMap[temp]=new TermAttributes();
      }
    }
  }
}
