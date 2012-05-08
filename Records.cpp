#include "Records.h"
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

CreateRecords::CreateRecords(std::vector<std::string>& fileList,std::string& stopFile)
  :files(fileList),
   stopWordFile(stopFile),
   toker(new Toker(stopFile))
{
}


CreateRecords::~CreateRecords()
{
  delete(toker);
  for(DocList::iterator it = docs.begin();it!=docs.end();++it)
  {
    delete(*it);
  }
}


bool
CreateRecords::initTokensAndDocuments()
{
  if(!toker->getIsValid())
  {
    std::cout<<"Toker error:stop words not initialied\n";
    return false;
  }
  DocList::const_iterator dIter=docs.begin();
  for(std::vector<std::string>::iterator it=files.begin();it!=files.end();++it)
  {
    Document *d = new Document(*it,toker);
    if(d->processDocument())
    {
      docs.insert_after(dIter,d);
    }
    else
    {
      std::cout<<"Error processing document: "<<d->getDocName();
      delete(d);
      return false;
    }
  }
  return true;
}

bool
CreateRecords::indexAndCreateII()
{
  for(DocList::iterator d=docs.begin();d!=docs.end();++d)
  {
    BOOST_FOREACH(std::string t,(*d)->getTokenList())
    {
      if (iIndex.find(t)!=iIndex.end())
      {
	DocList& dtemp=iIndex[t];
	DocList::iterator it=dtemp.begin();
	dtemp.insert_after(it,*d);
      }
      else
      {
	DocList temp;
        DocList::iterator it=temp.begin();
	temp.insert_after(it,*d);
        iIndex.insert(std::make_pair<std::string,DocList>(t,temp));
      }
    }
  } 
}

bool 
CreateRecords::helper_sort(Document* first,Document* second)
{
  if (first->getDocID()<second->getDocID())
    return true;
  else
    return false;
}

void
CreateRecords::processANDQueryString(std::string& q)
{
  std::string suffix=q.substr(q.find("and")+4);
  std::string prefix=q.substr(0,q.find("and")-1);
  
  DocList& list1 =iIndex[prefix];
  DocList& list2 =iIndex[suffix];
  list1.sort(boost::bind(&CreateRecords::helper_sort,this,_1,_2));
  list2.sort(boost::bind(&CreateRecords::helper_sort,this,_1,_2));
  std::cout<<prefix<<" ";
  for(DocList::iterator dit=list1.begin();dit!=list1.end();++dit)
  {
      std::cout<<" -> "<<(*dit)->getDocName();
  }
  std::cout<<"\n";
  std::cout<<suffix<<" ";
  for(DocList::iterator dit=list2.begin();dit!=list2.end();++dit)
  {
      std::cout<<" -> "<<(*dit)->getDocName();
  }
  std::cout<<"\n";
}


void
CreateRecords::printDetails()
{
  for(InvertedIndex::iterator it=iIndex.begin();it!=iIndex.end();++it)
  {
    std::cout<<(*it).first;
    std::cout<<" ";
    for(DocList::iterator dit=(*it).second.begin();dit!=(*it).second.end();++dit)
    {
      std::cout<<" -> "<<(*dit)->getDocID();
    }
    std::cout<<"\n";
  }
}

int main(int argc,char *argv[])
{
  //stub
  std::vector<std::string> files;
  std::string sw("./stopwords.txt");
  std::string s;
  while (*++argv)
  {
    s.assign(*argv);
    files.push_back(s);
  }
  CreateRecords cr(files,sw);
  cr.initTokensAndDocuments();
  cr.indexAndCreateII();
  
  //query - simple do only ANDs
  std::string query;
  std::cout<<"Enter query string: ";
  std::getline(std::cin,query);
  cr.processANDQueryString(query);
}
