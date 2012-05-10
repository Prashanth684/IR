#ifndef _CONTAINER_DEFS
#define _CONTAINER_DEFS

#include <boost/tokenizer.hpp>
#include <boost/container/slist.hpp>
#include <vector>
#include <map>

extern unsigned int global_doc_id;

typedef boost::tokenizer<> Tokenizer;
typedef boost::container::slist<std::string> LinkedList;

class Document;
class TermAttributes;
typedef boost::container::slist<Document*> DocList;
typedef std::map<std::string ,DocList > InvertedIndex;
typedef std::map<std::string ,TermAttributes* > AttrMap;

#endif
