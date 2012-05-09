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
CreateRecords::processSingleQueryString(std::string& q,DocList& result)
{
  std::size_t pos;
  std::string suffix;
  std::string prefix;
  bool isAndQuery=true;

  if((pos=q.find("and"))!=std::string::npos)
  {
   suffix=q.substr(pos+4);
   prefix=q.substr(0,pos-1);
  }
  else if((pos=q.find("or"))!=std::string::npos)
  {
   suffix=q.substr(pos+3);
   prefix=q.substr(0,pos-1);
   isAndQuery=false;
  }

  DocList& list1 =iIndex[prefix];
  DocList& list2 =iIndex[suffix];
  list1.sort(boost::bind(&CreateRecords::helper_sort,this,_1,_2));
  list2.sort(boost::bind(&CreateRecords::helper_sort,this,_1,_2));
  
  std::cout<<prefix;
  printDocList(list1);
  std::cout<<suffix;
  printDocList(list2);
  
  if(isAndQuery)
   processANDQueryString(list1,list2,result);
  else
   processORQueryString(list1,list2,result); 
  
  std::cout<<"result ";
  printDocList(result);
}

void
CreateRecords::processANDQueryString(DocList& list1,DocList& list2,DocList& result)
{
  if (list1.empty()||list2.empty())
  {
    return;
  }
  DocList::iterator resIter=result.begin();
  DocList::iterator itList1,itList2;
  for(itList1=list1.begin(),itList2=list2.begin();itList1!=list1.end() && itList2!=list2.end();)
  {
    if ((*itList1)->getDocID()==(*itList2)->getDocID())
    {
     resIter=result.insert_after(resIter,(*itList1));
     ++itList1;++itList2;
     continue;
    }
    (*itList1)->getDocID()<(*itList2)->getDocID()?++itList1:++itList2;
  }
}

void
CreateRecords::processORQueryString(DocList& list1,DocList& list2,DocList& result)
{
  if (list1.empty()||list2.empty())
  {
    list1.empty()?result=list2:result=list1;
    return;
  }
  result=list1;
  result.merge(list2);
  result.unique();
}


void
CreateRecords::printDocList(DocList& d)
{
  if(!d.empty())
  {
    for(DocList::iterator dit=d.begin();dit!=d.end();++dit)
    {
      std::cout<<" -> "<<(*dit)->getDocID();
    }
  }
    std::cout<<"\n";
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
  DocList result;
  cr.processSingleQueryString(query,result);
}
