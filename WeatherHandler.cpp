#include "pch.h"
#include "WeatherHandler.h"

#include <filesystem>

std::pair<std::string, std::string> CWeatherHandler::DecodeLocation(std::string dom) const
{
	rapidjson::Document doc;
	doc.Parse(dom.c_str(), dom.size());
	std::pair<std::string, std::string> output{};

	if (!doc.IsObject())
		return output;

	auto elem = doc.FindMember("status");
	if (elem != doc.MemberEnd())
	{
		elem = doc.FindMember("countryCode");
		if (elem != doc.MemberEnd())
			output.first = elem->value.GetString();
		elem = doc.FindMember("city");
		if (elem != doc.MemberEnd())
			output.second = elem->value.GetString();
	}

	return output;
}

std::vector<SWeatherLocation> CWeatherHandler::DecodeWeatherLocation(std::string data) const
{
	rapidjson::Document doc;
	doc.Parse(data.c_str(), data.size());
	std::vector<SWeatherLocation> out{};

	if (!doc.IsArray())
		return out;

	for (const auto& elem : doc.GetArray())
	{
		if (!elem.IsObject())
			continue;

		SWeatherLocation item{};
		for (auto obj = elem.MemberBegin(); obj != elem.MemberEnd(); ++obj)
		{
			if (obj->name.IsString() && "name" == obj->name && obj->value.IsString())
			{
				item.name = obj->value.GetString();
				continue;
			}
			if (obj->name.IsString() && "place_id" == obj->name && obj->value.IsString())
			{
				item.place_id = obj->value.GetString();
				continue;
			}
			if (obj->name.IsString() && "adm_area1" == obj->name && obj->value.IsString())
			{
				item.adm_area1 = obj->value.GetString();
				continue;
			}
			if (obj->name.IsString() && "adm_area2" == obj->name && obj->value.IsString())
			{
				item.adm_area2 = obj->value.GetString();
				continue;
			}
			if (obj->name.IsString() && "country" == obj->name && obj->value.IsString())
			{
				item.country = obj->value.GetString();
			}
		}
		if (!item.name.empty())
			out.push_back(item);
	}

	return out;
}

SWeather CWeatherHandler::DecodeWeather(std::string data) const
{
	SWeather weather{};
	rapidjson::Document doc{};
	doc.Parse(data.c_str(), data.size());

	if (!doc.IsObject())
		return weather;

	for (const auto& obj : doc.GetObj())
	{
		if ("detail" == obj.name && obj.value.IsString())
		{
			weather.summary = obj.value.GetString();
			continue;
		}
		if ("current" == obj.name && obj.value.IsObject())
		{
			ReadCurrent(obj.value, weather);
			continue;
		}
		if ("hourly" == obj.name && obj.value.IsObject())
		{
			weather.hourly = ReadHourly(obj.value);
			continue;
		}
		if ("daily" == obj.name && obj.value.IsObject())
		{
			weather.daily = ReadDaily(obj.value);
			continue;
		}
	}

	return weather;
}

void CWeatherHandler::ReadCurrent(const rapidjson::Value& obj, SWeather& output) const
{
	for (const auto& it : obj.GetObj())
	{
		if ("icon" == it.name && it.value.IsString())
		{
			output.icon = it.value.GetString();
			continue;
		}
		if ("icon_num" == it.name && it.value.IsInt())
		{
			output.icon_num = it.value.GetInt();
			continue;
		}
		if ("summary" == it.name && it.value.IsString())
		{
			output.summary = it.value.GetString();
			continue;
		}
		if ("temperature" == it.name)
		{
			output.temperature = GetTemperature(it.value);
			continue;
		}
		if ("wind" == it.name)
		{
			GetWind(it.value, output.wind);
			continue;
		}
		if ("precipitation" == it.name)
		{
			GetPrecipitation(it.value, output.precipitation);
			continue;
		}
		if ("cloud_cover" == it.name && it.value.IsInt())
		{
			output.cloud_cover = it.value.GetInt();
			continue;
		}
	}
}

std::vector<SWeatherHourly> CWeatherHandler::ReadHourly(const rapidjson::Value& obj) const
{
	std::vector<SWeatherHourly> output{};

	for (const auto& it : obj.GetObj())
	{
		for (const auto& elem : it.value.GetArray())
		{
			SWeatherHourly item{};
			for (const auto& hour : elem.GetObj())
			{
				if ("date" == hour.name && hour.value.IsString())
				{
					item.date = GetNormalizedDate(hour.value);
					continue;
				}
				if ("weather" == hour.name && hour.value.IsString())
				{
					item.weather = hour.value.GetString();
					continue;
				}
				if ("icon" == hour.name && hour.value.IsInt())
				{
					item.icon = hour.value.GetInt();
					continue;
				}
				if ("summary" == hour.name && hour.value.IsString())
				{
					item.summary = hour.value.GetString();
					continue;
				}
				if ("temperature" == hour.name)
				{
					item.temperature = GetTemperature(hour.value);
					continue;
				}
				if ("wind" == hour.name)
				{
					GetWind(hour.value, item.wind);
					continue;
				}
				if ("cloud_cover" == hour.name && hour.value.IsObject())
				{
					item.cloud_cover = GetCloudCover(hour.value);
					continue;
				}
				if ("precipitation" == hour.name && hour.value.IsObject())
				{
					GetPrecipitation(hour.value, item.precipitation);
					continue;
				}
			}
			output.push_back(std::move(item));
		}
		continue;
	}

	return output;
}

std::vector<SWeatherDaily> CWeatherHandler::ReadDaily(const rapidjson::Value& obj) const
{
	std::vector<SWeatherDaily> output{};

	for (const auto& it : obj.GetObj())
	{
		for (const auto& elem : it.value.GetArray())
		{
			SWeatherDaily item{};
			for (const auto& day : elem.GetObj())
			{
				if ("day" == day.name && day.value.IsString())
				{
					item.day = GetNormalizedDate(day.value);
					continue;
				}
				if ("weather" == day.name && day.value.IsString())
				{
					item.weather = day.value.GetString();
					continue;
				}
				if ("icon" == day.name && day.value.IsInt())
				{
					item.icon = day.value.GetInt();
					continue;
				}
				if ("summary" == day.name && day.value.IsString())
				{
					item.summary = day.value.GetString();
					continue;
				}
			}
			output.push_back(std::move(item));
		}
	}

	return output;
}

double CWeatherHandler::GetTemperature(const rapidjson::Value& obj) const
{
	if (obj.IsDouble())
		return obj.GetDouble();

	if (obj.IsInt())
		return static_cast<double>(obj.GetInt());

	return 0.0;
}

void CWeatherHandler::GetWind(const rapidjson::Value& obj, Wind& wind) const
{
	for (const auto& it : obj.GetObj())
	{
		if ("speed" == it.name && it.value.IsDouble())
		{
			wind.speed = it.value.GetDouble();
			continue;
		}
		if ("angle" == it.name && it.value.IsInt())
		{
			wind.angle = it.value.GetInt();
			continue;
		}
		if ("dir" == it.name && it.value.IsString())
		{
			wind.dir = it.value.GetString();
			continue;
		}
	}
}

void CWeatherHandler::GetPrecipitation(const rapidjson::Value& obj, Precipitation& precipitation) const
{
	for (const auto& it : obj.GetObj())
	{
		if ("total" == it.name)
		{
			if (it.value.IsDouble())
			{
				precipitation.total = it.value.GetDouble();
				continue;
			}
			if (it.value.IsInt())
			{
				precipitation.total = static_cast<double>(it.value.GetInt());
				continue;
			}
		}
		if ("type" == it.name && it.value.IsString())
		{
			precipitation.type = it.value.GetString();
			continue;
		}
	}
}

int CWeatherHandler::GetCloudCover(const rapidjson::Value& obj) const
{
	for (const auto& it : obj.GetObj())
	{
		if ("total" == it.name && it.value.IsInt())
			return it.value.GetInt();
	}

	return 0;
}

std::string CWeatherHandler::GetNormalizedDate(const rapidjson::Value& obj) const
{
	std::string output{ obj.GetString() };
	std::replace(output.begin(), output.end(), 'T', ' ');
	return output;
}
