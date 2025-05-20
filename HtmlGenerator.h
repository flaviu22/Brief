#pragma once

#include "DomTree.h"
#include "Trends.h"

#include <string>

using namespace domtree;

class CHtmlGenerator
{
	enum class Color
	{
		green,
		red
	};

public:
	CHtmlGenerator() = default;
	CHtmlGenerator(const std::string& pathIcon)
		:m_pathIcon(pathIcon)
	{}

public:
	std::string GenerateHtml(
		const bool move, 
		const bool keep, 
		SWeather weather, 
		std::vector<STrendItem> google_trends, 
		std::vector<STrendItem> youtube_trends, 
		const size_t cityid);
	std::string GenerateHtmlMessage(std::string message) const;

private:
	Tag* GenerateHeader(CDomTree& dom) const;
	Tag* GenerateSplitLeft(Tag* parent) const;
	void GenerateCustom(Tag* parent, const bool move, const bool keep) const;
	void GenerateWeatherCurrent(Tag* parent, const SWeather& weather) const;
	void GenerateWeatherHourly(Tag* parent, std::vector<SWeatherHourly> hours) const;
	void GenerateWeatherDaily(Tag* parent, std::vector<SWeatherDaily> days) const;
	Tag* GenerateSplitRight(Tag* parent) const;
	Tag* GenerateTopRight(Tag* parent, const size_t cityid) const;
	Tag* GenerateBottomRight(Tag* parent, std::vector<STrendItem> google_trends, std::vector<STrendItem> youtube_trends) const;
	void GenerateGoogleTrendsTitle(Tag* parent) const;
	void GenerateYoutubeTrendsTitle(Tag* parent) const;
	void GenerateTrendsSeparator(Tag* parent) const;
	void GenerateWeather(Tag* parent, SWeather weather) const;
	void GenerateGoogleTrends(Tag* parent, std::vector<STrendItem> trends) const;
	void GenerateYoutubeTrends(Tag* parent, std::vector<STrendItem> trends) const;

private:
	void SetCustomDivs(CDomTree& dom);
	Tag* FindSplitLeft(std::vector<std::shared_ptr<Tag>>& tags) const;
	Tag GenerateCustomDiv(LPCTSTR lpszText = nullptr, bool value = false) const;
	Tag GenerateCustomMove(const bool move) const;
	Tag GenerateCustomKeep(const bool keep) const;
	std::string GetColor(const Color color) const;

private:
	void GenerateClock(Tag* parent, const size_t cityid) const;

private:
	std::string m_pathIcon{};
};
