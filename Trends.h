#pragma once
#include "BriefDoc.h"

#include <unordered_map>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_iterators.hpp"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

class CTrends
{
public:
	std::string GetError() const { return m_error; }
	std::unordered_map<std::string, std::string> ReadGeos() const;
	std::vector<STrendItem> ReadGoogleTrends(std::string trends);
	std::vector<STrendItem> ReadYoutubeTrends(std::string trends);
	std::vector<STrendItem> SortTrends(std::vector<STrendItem>&& trends) const;

private:
	std::string m_title{};
	std::string m_link{};
	mutable std::string m_error{};

private:
	std::vector<STrendItem> PopulateGoogleTrends(const rapidxml::xml_document<>& doc) const;
	std::vector<STrendItem> PopulateYoutubeTrends(const rapidjson::Document& doc) const;
};
