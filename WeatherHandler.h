#pragma once

#include "Data.h"
#include "BriefDoc.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

class CWeatherHandler
{
public:
	std::pair<std::string, std::string> DecodeLocation(std::string dom) const;
	std::vector<SWeatherLocation> DecodeWeatherLocation(std::string data) const;
	SWeather DecodeWeather(std::string data) const;

protected:
	void ReadCurrent(const rapidjson::Value& obj, SWeather& output) const;
	double GetTemperature(const rapidjson::Value& obj) const;
	void GetWind(const rapidjson::Value& obj, Wind& wind) const;
	void GetPrecipitation(const rapidjson::Value& obj, Precipitation& precipitation) const;
	std::vector<SWeatherHourly> ReadHourly(const rapidjson::Value& obj) const;
	std::vector<SWeatherDaily> ReadDaily(const rapidjson::Value& obj) const;
	int GetCloudCover(const rapidjson::Value& obj) const;
	std::string GetNormalizedDate(const rapidjson::Value& obj) const;
};
