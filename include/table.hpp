//
//  table.hpp
//
//  Created by Caleb Johnston on 3/11/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <array>
#include <iostream>
#include <map>
#include <vector>
#include <utility>

template<typename RowType>
class Table {
public:
//	typedef std::tuple<typename Fields...> Result;
//	typedef std::function<Result(RowType)> RowResultType;
//	typedef std::tuple<> ResultRecord;
//	typedef std::vector<ResultRecord> ResultSet;
	typedef std::function<bool(RowType)> RowPredicate;
//	typedef std::function<ResultRecord(RowType)> ResultPredicate;
	
public:
	size_t ensure( const RowType& record )
	{
		auto iter = _index.find(record);
		size_t index;
		if (iter == _index.end())
			index = insert( record );
		else
			index = (*iter).second + 1;
		
		return index;
	}
	
	RowType& select( const size_t index )
	{
		return _storage.at(index - 1);
	}
	
	size_t insert( const RowType& record )
	{
		size_t index = _storage.size();
		_storage.push_back( record );
		_index.emplace( std::make_pair( record, index ) );
		return index + 1;
	}
	
	bool remove( const RowType& record )
	{
		auto iter = _index.find(record);
		if (iter == _index.end()) return false;
		
		_index.erase(record);
		_storage.erase(_storage.begin() + iter->second);
		return true;
	}
	
	bool remove( const size_t index )
	{
		if (index >= _storage.size()) return false;
		
		_index.erase(_storage.at(index - 1));
		_storage.erase(_storage.begin()+index);
		return true;
	}
	
	size_t size() const
	{
		return _storage.size();
	}
	
	template<typename RowPredicate>
	auto query(RowPredicate filter) const
	{
		return query(_storage, filter);
	}
	
//	template<typename ResultType, typename ResultPredicate>
//	std::vector<ResultType> filter(const std::vector<RowType>& data, ResultPredicate mapping) const
//	{
//		std::vector<ResultType> filter_results;
//		std::transform(data.begin(), data.end(), std::back_inserter(filter_results), [&](const RowType& datum) {
//			return mapping(datum);
//		});
//		return filter_results;
//	}
	
	template<typename RowPredicate>
	static std::vector<RowType> query(std::vector<RowType> data, RowPredicate filter)
	{
		std::vector<RowType> results;
		std::copy_if(std::begin(data), std::end(data), std::back_inserter(results), filter);
		return results;
	}
	
	template<typename... Fields, typename FieldMapping>
	static std::tuple<Fields...> fields(std::vector<RowType> data, FieldMapping mapping)
	{
		std::tuple<Fields...> results;
		std::transform(data.begin(), data.end(), std::back_inserter(results), mapping);
		return results;
	}
	
private:
	std::map<RowType,size_t> _index;
	std::vector<RowType> _storage;
};

class DataController {
public:
	DataController(size_t limit = 0) : _capacity(limit), _consumed(0) {}
	
	const std::vector<DataRef>& data() const { return _data; }
	
	int64_t load(DataRef record)
	{
		if (record && _capacity - _consumed < record->size) return -1;
		
		size_t index = _data.size();
		_data.push_back( record );
		return index + 1;
	}
	
	DataRef at(int64_t record)
	{
		if (record < 0 || record >= _data.size()) return std::make_shared<Data>(DataAttribs());
		
		return _data.at(record);
	}
	
	bool unload(int64_t record)
	{
		if (record < 0 || record >= _data.size()) return false;
		
		_data.erase(_data.begin() + record);
		return true;
	}
	
	bool unload(DataRef record)
	{
		auto iter = std::find(begin(_data), end(_data), record);
		if (iter == _data.end()) return false;
		
		_data.erase(iter);
		return true;
	}
	
private:
	size_t _capacity;
	size_t _consumed;
	std::vector<DataRef> _data;
};

