#pragma once

#include "pch.h"
#include "resource.h"

#include <string>
#include <vector>

#define WMU_THREADDATA						(WM_APP + 13)
#define WMU_POSTINIT						(WM_APP + 14)
#define WMU_SETSTATUSTEXT					(WM_APP + 15)

constexpr UINT MESSAGE_EXTERNALIP_DONE = 1;
constexpr UINT MESSAGE_GETLOCATION_DONE = 2;
constexpr UINT MESSAGE_GETGOOGLETRENDS_DONE = 3;
constexpr UINT MESSAGE_GETYOUTUBETRENDS_DONE = 4;
constexpr UINT MESSAGE_GETWEATHER_DONE = 5;
constexpr UINT TIME_SECOND = 1000;
constexpr UINT TIME_MINUTE = 60 * TIME_SECOND;
constexpr UINT STATUS_RESET = 1;
constexpr UINT STATUS_ERROR = 2;

typedef DWORD time_seconds;

struct STrendItem
{
public:
	std::string title{};
	std::string approx_traffic{};
	std::string link{};
	std::string pubDate{};
	std::string picture{};
	std::string picture_source{};
	std::string news_item_title{};
	std::string news_item_snippet{};
	std::string news_item_url{};
	std::string news_item_source{};
	std::string video_id{};
	std::string description{};
	std::string channelTitle{};
	std::string error{};
	size_t traffic{};
	size_t error_code{};

public:
	STrendItem() = default;
	STrendItem(const STrendItem& rhs) = delete;
	STrendItem& operator=(const STrendItem& rhs) = delete;
	STrendItem(STrendItem&& rhs) = default;
	STrendItem& operator=(STrendItem&& rhs) = default;
	~STrendItem() = default;
};

struct SWeatherLocation
{
	std::string name{};
	std::string adm_area1{};
	std::string adm_area2{};
	std::string country{};
	std::string place_id{};
};

struct Wind
{
	double speed{};
	int angle{};
	std::string dir{};
};

struct Precipitation
{
	double total{};
	std::string type{};
};

struct SWeatherHourly
{
	std::string date{};
	std::string weather{};
	int icon{};
	std::string summary{};
	double temperature{};
	Wind wind{};
	Precipitation precipitation{};
	int cloud_cover{};

public:
	SWeatherHourly() = default;
	SWeatherHourly(const SWeatherHourly& rhs) = delete;
	SWeatherHourly& operator=(const SWeatherHourly& rhs) = delete;
	SWeatherHourly(SWeatherHourly&& rhs) = default;
	SWeatherHourly& operator=(SWeatherHourly&& rhs) = default;
	~SWeatherHourly() = default;
};

struct SWeatherDaily
{
	int icon{};
	std::string day{};
	std::string weather{};
	std::string summary{};

public:
	SWeatherDaily() = default;
	SWeatherDaily(const SWeatherDaily& rhs) = delete;
	SWeatherDaily& operator=(const SWeatherDaily& rhs) = delete;
	SWeatherDaily(SWeatherDaily&& rhs) = default;
	SWeatherDaily& operator=(SWeatherDaily&& rhs) = default;
	~SWeatherDaily() = default;
};

struct SWeather
{
public:
	CString location_name{};
	CString location_country{};
	std::string icon{};
	int icon_num{};
	std::string summary{};
	double temperature{};
	Wind wind{};
	Precipitation precipitation{};
	int cloud_cover{};
	std::vector<SWeatherHourly> hourly{};
	std::vector<SWeatherDaily> daily{};

public:
	SWeather() = default;
	SWeather(const SWeather& rhs) = delete;
	SWeather& operator=(const SWeather& rhs) = delete;
	SWeather(SWeather&& rhs) = default;
	SWeather& operator=(SWeather&& rhs) = default;
	~SWeather() = default;
};
