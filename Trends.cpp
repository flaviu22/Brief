#include "pch.h"
#include "Trends.h"
#include "DataList.h"

#include <regex>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::unordered_map<std::string, std::string> CTrends::ReadGeos() const
{
	rapidjson::Document doc{};
	doc.Parse(geo_list.data(), geo_list.size());
	std::unordered_map<std::string, std::string> geo{};

	for (const auto& elem : doc.GetArray())
	{
		std::string id{}, name{};
		for (const auto& obj : elem.GetObj())
		{
			if ("id" == obj.name)
				id = obj.value.GetString();
			if ("name" == obj.name)
				name = obj.value.GetString();
		}
		geo.insert(std::make_pair(name, id));
	}

	return geo;
}

std::vector<STrendItem> CTrends::ReadGoogleTrends(std::string trends)
{
	try
	{
		std::unique_ptr<rapidxml::xml_document<>> doc = std::make_unique<rapidxml::xml_document<>>();
		doc->parse<0>(trends.data());
		doc->validate();
		return PopulateGoogleTrends(*doc);
	}
	catch (const rapidxml::parse_error& e)
	{
		m_error = e.what();
	}
	catch (const rapidxml::validation_error& e)
	{
		m_error = e.what();
	}
	catch (const std::exception& e)
	{
		m_error = e.what();
	}

	return {};
}

std::vector<STrendItem> CTrends::ReadYoutubeTrends(std::string trends)
{
	rapidjson::Document doc;
	doc.Parse(trends.c_str(), trends.size());
	if (doc.HasParseError())
	{
		std::stringstream ss;
		ss << "Youtube data parsing error: " << rapidjson::GetParseError_En(doc.GetParseError())
			<< " at offset " << doc.GetErrorOffset();
		m_error = ss.str();
		return {};
	}
	return PopulateYoutubeTrends(doc);
}

std::vector<STrendItem> CTrends::SortTrends(std::vector<STrendItem>&& trends) const
{
	std::sort(trends.begin(), trends.end(), 
		[](const STrendItem& item1, const STrendItem& item2)
		{
			return item1.traffic > item2.traffic;
		});

	return std::move(trends);
}

std::vector<STrendItem> CTrends::PopulateGoogleTrends(const rapidxml::xml_document<>& doc) const
{
	std::vector<STrendItem> output{};

	for (auto root = doc.first_node(); root; root = root->next_sibling())
	{
		for (auto level = root->first_node(); level; level = level->next_sibling())
		{
			for (auto level1 = level->first_node(); level1; level1 = level1->next_sibling())
			{
				STrendItem item{};
				for (auto level2 = level1->first_node(); level2; level2 = level2->next_sibling())
				{
					if (0 == strcmp(level2->name(), "title"))
					{
						item.title = level2->value();
						continue;
					}
					if (0 == strcmp(level2->name(), "link"))
					{
						item.link = level2->value();
						continue;
					}
					if (0 == strcmp(level2->name(), "approx_traffic"))
					{
						std::stringstream ss(std::regex_replace(level2->value(), std::regex(R"([\D])"), ""));
						ss >> item.traffic;
						item.approx_traffic = level2->value();
						continue;
					}
					if (0 == strcmp(level2->name(), "pubDate"))
					{
						item.pubDate = level2->value();
						continue;
					}
					if (0 == strcmp(level2->name(), "picture"))
					{
						item.picture = level2->value();
						continue;
					}
					if (0 == strcmp(level2->name(), "picture_source"))
					{
						item.picture_source = level2->value();
						continue;
					}

					for (auto level3 = level2->first_node(); level3; level3 = level3->next_sibling())
					{
						if (0 == strcmp(level3->name(), "news_item_title"))
						{
							item.news_item_title = level3->value();
							continue;
						}
						if (0 == strcmp(level3->name(), "news_item_snippet"))
						{
							item.news_item_snippet = level3->value();
							if (item.news_item_snippet.empty() && !item.news_item_title.empty())
								item.news_item_snippet = item.news_item_title;
							continue;
						}
						if (0 == strcmp(level3->name(), "news_item_url"))
						{
							item.news_item_url = level3->value();
							continue;
						}
						if (0 == strcmp(level3->name(), "news_item_source"))
						{
							item.news_item_source = level3->value();
						}
					}
					if (!item.news_item_snippet.empty() && !item.news_item_url.empty() && !item.news_item_source.empty())
						break;
				}
				if (!item.title.empty())
					output.push_back(std::move(item));
			}
		}
	}

	return output;
}

std::vector<STrendItem> CTrends::PopulateYoutubeTrends(const rapidjson::Document& doc) const
{
	std::vector<STrendItem> output{};

	if (!doc.IsObject())
		return output;

	for (const auto& obj : doc.GetObj())
	{
		if ("error" == obj.name && obj.value.IsObject())
		{
			STrendItem trend_error{};
			for (const auto& err : obj.value.GetObj())
			{
				if ("code" == err.name && err.value.IsInt())
				{
					trend_error.error_code = err.value.GetInt();
					continue;
				}
				if ("message" == err.name && err.value.IsString())
				{
					trend_error.error = err.value.GetString();
					continue;
				}
			}
			output.push_back(std::move(trend_error));
			break;
		}
		if ("items" == obj.name && obj.value.IsArray())
		{
			for (const auto& item : obj.value.GetArray())
			{
				STrendItem trend{};
				for (const auto& sub : item.GetObj())
				{
					if ("id" == sub.name && sub.value.IsObject())
					{
						for (const auto& ids : sub.value.GetObj())
						{
							if ("videoId" == ids.name && ids.value.IsString())
							{
								trend.video_id = ids.value.GetString();
								continue;
							}
						}
					}
					if ("snippet" == sub.name && sub.value.IsObject())
					{
						for (const auto& snip : sub.value.GetObj())
						{
							if ("publishedAt" == snip.name && snip.value.IsString())
							{
								trend.pubDate = snip.value.GetString();
								std::replace(trend.pubDate.begin(), trend.pubDate.end(), 'T', ' ');
								std::replace(trend.pubDate.begin(), trend.pubDate.end(), 'Z', ' ');
								continue;
							}
							if ("title" == snip.name && snip.value.IsString())
							{
								trend.title = snip.value.GetString();
								continue;
							}
							if ("description" == snip.name && snip.value.IsString())
							{
								trend.description = snip.value.GetString();
								continue;
							}
							if ("thumbnails" == snip.name && snip.value.IsObject())
							{
								for (const auto& thumb : snip.value.GetObj())
								{
									if ("medium" == thumb.name && thumb.value.IsObject())
									{
										for (const auto& def : thumb.value.GetObj())
										{
											if ("url" == def.name && def.value.IsString())
											{
												trend.picture = def.value.GetString();
												continue;
											}
										}
									}
								}
								continue;
							}
							if ("channelTitle" == snip.name && snip.value.IsString())
							{
								trend.channelTitle = snip.value.GetString();
								continue;
							}
						}
					}
				}
				output.push_back(std::move(trend));
			}
		}
	}

	return output;
}
