#include <algorithm>
#include "SymbolTable.h"
#include "Exception.h"

void SymbolTable::Serialize(Serializer& out) const
{
	out<<dictionary_.size();
	std::map<const std::string, unsigned int>::const_iterator it;
	for (it=dictionary_.begin();  it!=dictionary_.end(); ++it)
		out<<it->first<<it->second;
	out<<curId_;
}

void SymbolTable::DeSerialize(DeSerializer& in)
{
	dictionary_.clear();
	unsigned int size;
	in>>size;
	for (unsigned int i=0; i<size; ++i)
	{
		std::string str;
		unsigned int id;
		in>>str>>id;
		dictionary_[str] = id;
	}
	in>>curId_;

}


unsigned int SymbolTable::Add(const std::string&  str)
{
	dictionary_[str] = curId_;
	return curId_++;
}

unsigned int SymbolTable::Find(const std::string& str) const
{
	std::map<const std::string, unsigned int>::const_iterator it;
	it = dictionary_.find(str);
	if (it != dictionary_.end())
		return it->second;

	return IDNOTFOUND;
}

void SymbolTable::Clear()
{
	dictionary_.clear();
	curId_ = 0;
}

// function object、functor
// 让一个类对象使用起来像一个函数
// STL六大组件之一
// 容器
// 算法
// 迭代器
// 适配器
// 函数对象
// 内存分配器
class IsEqualId
{
public:
	explicit IsEqualId(unsigned int id) : id_(id) {}
	bool operator()(const std::pair<const std::string, unsigned int>& it) const
	{
		return it.second == id_;
	}
private:
	unsigned int id_;
};


std::string SymbolTable::GetSymbolName(unsigned int id) const
{
	std::map<const std::string, unsigned int>::const_iterator it;
	it = find_if(dictionary_.begin(), dictionary_.end(), IsEqualId(id));

	if (it == dictionary_.end())
	{
		throw Exception("Internal error: missing entry in symbol table.");
	}

	return it->first;
}
