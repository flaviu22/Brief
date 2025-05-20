#include "pch.h"
#include "Brief.h"
#include "HtmlGenerator.h"
#include "DataList.h"

std::string CHtmlGenerator::GenerateHtml(
	const bool move, 
	const bool keep, 
	SWeather weather, 
	std::vector<STrendItem> google_trends, 
	std::vector<STrendItem> youtube_trends, 
	const size_t cityid)
{
	CDomTree dom{};

	const auto body = GenerateHeader(dom);
	const auto splitleft = GenerateSplitLeft(body);
	GenerateCustom(splitleft, move, keep);
	if (weather.daily.size() > 0)
		GenerateWeather(splitleft, std::move(weather));
	const auto splitright = GenerateSplitRight(body);
	GenerateTopRight(splitright, cityid);
	GenerateBottomRight(splitright, std::move(google_trends), std::move(youtube_trends));

	return dom.GetData();
}

Tag* CHtmlGenerator::GenerateHeader(CDomTree& dom) const
{
	dom.GetTags().push_back(std::make_shared<Tag>("!DOCTYPE html"));
	Tag head("head");
	head.AddChild({ "meta", { {{"http-equiv"}, {"X-UA-Compatible"}}, {{"content"}, {"IE=edge"}} } });
	head.AddChild({ "meta", { {{"http-equiv"}, {"content-type"}}, {{"content"}, {"text/html; charset=utf-8"}} } });
	head.AddChild({ "meta", { {{"name"}, {"viewport"}}, {{"content"}, {"width=device-width, initial-scale=1"}} } });
	head.AddChild({ "style", std::move(html_style.data()) });

	Tag html("html");
	html.AddAttributes({ {{"lang"}, {"en"}} });
	html.AddChild(head);
	html.AddChild({ "body" });
	dom.GetTags().push_back(std::make_shared<Tag>(std::move(html)));

	return dom.GetTags().back()->m_childs.back().get();
}

Tag* CHtmlGenerator::GenerateSplitLeft(Tag* parent) const
{
	parent->AddChild({ "div", { {{"class"}, {"split left"}} } });
	return parent->m_childs.back().get();
}

void CHtmlGenerator::GenerateCustom(Tag* parent, const bool move, const bool keep) const
{
	parent->m_childs.push_back(std::make_shared<Tag>(GenerateCustomDiv()));
	parent->m_childs.push_back(std::make_shared<Tag>(GenerateCustomMove(move)));
	parent->m_childs.push_back(std::make_shared<Tag>(GenerateCustomKeep(keep)));
}

void CHtmlGenerator::GenerateWeatherCurrent(Tag* parent, const SWeather& weather) const
{
	CString sText;
	sText.Format(_T("%s, %s"), weather.location_name, weather.location_country);
	Tag div("div", { {{"class"}, {"weather-current"}} });
	div.AddChild({ "div", CStringA(sText).GetString(), { {{"class"}, {"weather-current-title"}} } });

	sText.Format(_T("%S%d.png"), m_pathIcon.c_str(), weather.icon_num);
	div.AddChild({ "img", { {{"src"}, CStringA(sText).GetString()} } });
	div.AddText(weather.summary);
	div.AddChild({ "br" });
	sText.Format(_T("Temperature: %.1f&deg; C"), weather.temperature);
	div.AddText(CStringA(sText).GetString());
	div.AddChild({ "br" });
	sText.Format(_T("Wind speed: %.1f m/s, Angle: %d&deg;, Dir: %S"), 
		weather.wind.speed, weather.wind.angle, weather.wind.dir.c_str());
	div.AddText(CStringA(sText).GetString());
	div.AddChild({ "br" });

	parent->AddChild(div);
	parent->AddChild({ "br" });
	parent->AddChild({ "br" });
}

void CHtmlGenerator::GenerateWeatherHourly(Tag* parent, std::vector<SWeatherHourly> hours) const
{
	size_t count{};
	for (const auto& hour : hours)
	{
		COleDateTime dt;
		if (dt.ParseDateTime(CString(hour.date.c_str())))
		{
			const COleDateTimeSpan span = dt - COleDateTime::GetCurrentTime();
			if (span.GetTotalMinutes() < 30)
				continue;
			if (++count > 12)
				break;

			Tag div("div");
			div.AddAttributes({ {{"class"}, {"weather-hourly"}} });

			CString sText;
			if (dt.GetHour() <= 11)
				sText.Format(_T("%d AM"), dt.GetHour());
			else
				sText.Format(_T("%d PM"), 12 == dt.GetHour() ? dt.GetHour() : dt.GetHour() - 12);
			div.AddChild({ "p", CStringA(sText).GetString() });
			sText.Format(_T("%S%d.png"), m_pathIcon.c_str(), hour.icon);
			div.AddChild({ "img", { {{"src"}, {CStringA(sText).GetString()}} } });
			sText.Format(_T("%.1f&deg; C"), hour.temperature);
			div.AddChild({ "p", CStringA(sText).GetString() });
			parent->AddChild(div);
		}
	}
}

void CHtmlGenerator::GenerateWeatherDaily(Tag* parent, std::vector<SWeatherDaily> days) const
{
	size_t count{};
	for (const auto& day : days)
	{
		COleDateTime dt;
		if (dt.ParseDateTime(CString(day.day.c_str())))
		{
			if (COleDateTime::GetCurrentTime() >= dt)
				continue;
			if (++count > 4)
				break;

			Tag div("div", { {{"class"}, {"weather-daily"}} });
			div.AddChild({ "div", CStringA(dt.Format(_T("%A %Y-%m-%d"))).GetString(), { {{"class"}, {"weather-daily-date"}} } });
			CString sText;
			sText.Format(_T("%S%d.png"), m_pathIcon.c_str(), day.icon);
			Tag div2("div");
			div2.AddChild({ "img", { {{"src"}, { CStringA(sText).GetString() }} } });
			div.AddChild(div2);
			Tag div3("div");
			div3.AddChild({ "br" });
			div3.AddText(day.summary);
			div.AddChild(div3);
			parent->AddChild(div);
		}
	}
}

Tag* CHtmlGenerator::GenerateSplitRight(Tag* parent) const
{
	parent->AddChild({ "div", { {{"class"}, {"split right"}} } });
	return parent->m_childs.back().get();
}

Tag* CHtmlGenerator::GenerateTopRight(Tag* parent, const size_t cityid) const
{
	Tag top_right("div", { {{"class"}, {"top-right"}} });
	GenerateClock(&top_right, cityid);
	parent->AddChild(std::move(top_right));
	return parent->m_childs.back().get();
}

Tag* CHtmlGenerator::GenerateBottomRight(
	Tag* parent, std::vector<STrendItem> google_trends, 
	std::vector<STrendItem> youtube_trends) const
{
	bool both = google_trends.size() > 0 && youtube_trends.size() > 0;
	Tag bottom_right("div", { {{"class"}, {"bottom-right"}} });
	if (google_trends.size() > 0)
	{
		GenerateGoogleTrendsTitle(&bottom_right);
		GenerateGoogleTrends(&bottom_right, std::move(google_trends));
	}
	if (both)
		GenerateTrendsSeparator(&bottom_right);
	if (youtube_trends.size() > 0)
	{
		GenerateYoutubeTrendsTitle(&bottom_right);
		GenerateYoutubeTrends(&bottom_right, std::move(youtube_trends));
	}
	parent->AddChild(std::move(bottom_right));
	return parent->m_childs.back().get();
}

void CHtmlGenerator::SetCustomDivs(CDomTree& dom)
{
	auto tag = FindSplitLeft(dom.GetTags());
	if (tag)
	{
		tag->m_childs.insert(tag->m_childs.begin(), std::make_shared<Tag>(GenerateCustomKeep(false)));
		tag->m_childs.insert(tag->m_childs.begin(), std::make_shared<Tag>(GenerateCustomMove(false)));
		tag->m_childs.insert(tag->m_childs.begin(), std::make_shared<Tag>(GenerateCustomDiv()));
	}
}

void CHtmlGenerator::GenerateGoogleTrendsTitle(Tag* parent) const
{
	Tag h2("h2", "Google Trends - " + 
		std::string(CW2A(theApp.GetProfileString(_T("Settings"), _T("TrendsLocationName"), _T("United Kingdom")), CP_UTF8).m_psz));
	parent->AddChild(h2);
}

void CHtmlGenerator::GenerateYoutubeTrendsTitle(Tag* parent) const
{
	Tag h2("h2", "Youtube Trends - " + 
		std::string(CW2A(theApp.GetProfileString(_T("Settings"), _T("TrendsLocationName"), _T("United Kingdom")), CP_UTF8).m_psz));
	parent->AddChild(h2);
}

void CHtmlGenerator::GenerateTrendsSeparator(Tag* parent) const
{
	Tag separator("div", { {{"style"}, {"padding-top:250px"}} });
	parent->AddChild(separator);
}

void CHtmlGenerator::GenerateWeather(Tag* parent, SWeather weather) const
{
	GenerateWeatherCurrent(parent, weather);
	GenerateWeatherHourly(parent, std::move(weather.hourly));
	GenerateWeatherDaily(parent, std::move(weather.daily));
}

void CHtmlGenerator::GenerateGoogleTrends(Tag* parent, std::vector<STrendItem> trends) const
{
	for (auto&& it : trends)
	{
		Tag trends("div", { {{"class"}, {"trends"}} });
		trends.AddChild({ "div", it.title, { {{"class"}, {"trends-title"}} } });

		Tag trends_icon("div", { {{"class"}, {"trends-icon"}} });
		trends_icon.AddChild({ "img", { {{"src"}, {it.picture}} } });
		trends.AddChild(trends_icon);

		Tag div({ "div", { {{"class"}, {"trends-item"}} } });
		div.AddText("Traffic approximatively: " + it.approx_traffic);
		div.AddChild({ "br" });
		div.AddText({ "Source: " });
		div.AddChild({ "a", it.news_item_source, { {{"target"}, {"blank_"}}, {{"href"}, {it.news_item_url}} } });
		div.AddChild({ "br" });
		div.AddText(it.news_item_snippet);
		trends.AddChild(div);
		parent->AddChild(trends);
	}
}

void CHtmlGenerator::GenerateYoutubeTrends(Tag* parent, std::vector<STrendItem> trends) const
{
	for (auto&& it : trends)
	{
		Tag trends("div", { {{"class"}, {"trends"}} });
		trends.AddChild({ "div", it.title, { {{"class"}, {"trends-title"}} } });

		Tag trends_icon("div", { {{"class"}, {"trends-icon"}} });
		trends_icon.AddChild({ "img", { {{"src"}, {it.picture}} } });
		trends.AddChild(trends_icon);

		Tag div({ "div", { {{"class"}, {"trends-item"}} } });
		div.AddText("Channel title: " + it.channelTitle);
		div.AddChild({ "br" });
		div.AddText({ "Source: " });
		div.AddChild({ "a", it.video_id, { {{"target"}, {"blank_"}}, {{"href"}, {"https://www.youtube.com/watch?v=" + it.video_id}}}});
		div.AddChild({ "br" });
		div.AddText(it.description);
		div.AddChild({ "br" });
		div.AddChild({ "br" });
		div.AddText("Published at: " + it.pubDate);
		trends.AddChild(div);
		parent->AddChild(trends);
	}
}

Tag* CHtmlGenerator::FindSplitLeft(std::vector<std::shared_ptr<Tag>>& tags) const
{
	for (auto& it : tags)
	{
		auto found = FindSplitLeft(it->m_childs);
		if (found)
			return found;
		if ("div" == it->m_name)
		{
			for (const auto& attr : it->m_attributes)
			{
				if ("class" == attr.m_key && "split left" == attr.m_value)
					return it.get();
			}
		}
	}

	return nullptr;
}

Tag CHtmlGenerator::GenerateCustomDiv(LPCTSTR lpszText/* = nullptr*/, bool value/* = false*/) const
{
	Tag tag("div", { {{"class"}, {"custom"}} });

	if (lpszText)
	{
		tag.AddChild({ "div", std::move(CW2A(lpszText, CP_UTF8).m_psz), { {{"class"}, {"custom-left"}} } });
		Tag tagCustomRight("div", { {{"class"}, {"custom-right"}} });
		tagCustomRight.AddChild({ "font", value ? "ON" : "OFF", { {{"color"}, {value ? GetColor(Color::red) : GetColor(Color::green)}} } });
		tag.AddChild(std::move(tagCustomRight));
	}

	return tag;
}

Tag CHtmlGenerator::GenerateCustomMove(const bool move) const
{
	return GenerateCustomDiv(_T("Move"), move);
}

Tag CHtmlGenerator::GenerateCustomKeep(const bool keep) const
{
	return GenerateCustomDiv(_T("Keep"), keep);
}

std::string CHtmlGenerator::GetColor(const Color color) const
{
	switch (color)
	{
	case Color::green:
		return "#33ff11";
	case Color::red:
		return "#ff1131";
	}

	return "";
}

std::string CHtmlGenerator::GenerateHtmlMessage(std::string message) const
{
	CDomTree dom{};
	dom.GetTags().push_back(std::make_shared<Tag>("!DOCTYPE html"));
	Tag html("html");
	html.AddAttributes({ {{"lang"}, {"en"}} });
	Tag head("head");
	head.AddChild({ "meta", { {{"http-equiv"}, {"X-UA-Compatible"}}, {{"content"}, {"IE=edge"}} } });
	head.AddChild({ "meta", { {{"name"}, {"viewport"}}, {{"content"}, {"width=device-width, initial-scale=1"}} } });
	head.AddChild({ "style", html_style_short.data() });
	Tag body("body");
	body.AddChild({ "div", message, { {{"class"}, {"center-text"}} } });
	html.AddChild(head);
	html.AddChild(body);
	dom.GetTags().push_back(std::make_shared<Tag>(std::move(html)));

	return dom.GetData();
}

void CHtmlGenerator::GenerateClock(Tag* parent, const size_t cityid) const
{
	Tag div("div");
	div.AddAttributes({ {{"class"}, {"DPAC"}}, {{"lang"}, {"en"}} });
	div.AddAttributes({ {{"cityid"}, std::to_string(cityid)}});
	div.AddAttributes({ {{"id"}, {"dayspedia_widget_3541d001a09bd33e"}}, {{"host"}, {"https://dayspedia.com"}}, {{"nightsign"}, {"true"}} });
	div.AddAttributes({ {{"sun"}, {"true"}} });
//	div.AddAttributes({ {{"auto"}, {"true"}} });
	div.AddAttributes({ {{"style"}, {"width: 100%; padding: 12px 12px 20px; background-color: rgb(22, 27, 31); border-width: 0px; border-color: rgb(250, 252, 255);"}} });
	div.AddChild({ "style", std::move(clock_style.data()), { {{"media"}, {"screen"}}, {{"id"}, {"DPACstyles"}} } });
	Tag a("div");
	CString sText;
	sText.Format(_T("https://dayspedia.com/time/%s/%s/"), 
		theApp.GetProfileString(_T("Settings"), _T("TrendsLocation")),
		theApp.GetProfileString(_T("Settings"), _T("WeatherLocationName")));
	a.AddAttributes({ {{"class"}, {"DPl"}}, {{"href"}, {CStringA(sText).GetString()}} });
//	a.AddAttributes({ {{"class"}, {"DPl"}}, {{"href"}, {"https://dayspedia.com/time/ro/bucharest/"}} });
//	a.AddAttributes({ {{"class"}, {"DPl"}}, {{"href"}, {"https://dayspedia.com/time/"}} });
	a.AddAttributes({ {{"target"}, {"_blank"}}, {{"style"}, {"display:block!important;text-decoration:none!important;border:none!important;cursor:pointer!important;background:transparent!important;line-height:0!important;text-shadow:none!important;position:absolute;z-index:1;top:0;right:0;Brieftom:0;left:0"}} });
	Tag svg("svg", std::move(clock_svg.data()));
	svg.AddAttributes({ {{"xmlns"}, {"http://www.w3.org/2000/svg"}}, {{"viewbox"}, {"0 0 16 16"}} });
	svg.AddAttributes({ {{"style"}, {"position:absolute;right:8px;Brieftom:0;width:16px;height:16px"}} });
	a.AddChild(svg);
//	Tag span("span");
//	span.AddAttributes({ {{"title"}, {"DaysPedia.com"}}, {{"style"}, {"position: absolute; right: 28px; Brieftom: 6px; height: 10px; width: 60px; overflow: hidden; text-align: right; color: rgb(255, 255, 255); font: 10px / 10px Arial, sans-serif !important;"}} });
//	span.value_ = "Powered&nbsp;by DaysPedia.com";
//	a.AddChild(span);
	div.AddChild(a);
	div.AddChild({"div", "Current Time", { {{"class"}, {"DPACh"}}, {{"style"}, {"font-size: 27px; color: rgb(255, 255, 255);"}} } });
	Tag div1("div", { {{"class"}, {"DPACt"}}, {{"style"}, {"width: 240px; height: 240px; font-weight: normal; background-color: rgb(30, 30, 44);"}} });
	div1.AddChild({ "svg", std::move(clock_svg1.data()), { {{"class"}, {"DPACtcf"}}, {{"xmlns"}, {"http://www.w3.org/2000/svg"}}, {{"viewbox"}, {"0 0 112 112"}} } });
	div1.AddChild({ "svg", std::move(clock_svg2.data()), { {{"class"}, {"DPACth"}}, {{"xmlns"}, {"http://www.w3.org/2000/svg"}}, {{"viewbox"}, {"0 0 112 112"}}, {{"style"}, {"transform: rotate(368deg);"}} }});
	div1.AddChild({ "svg", std::move(clock_svg3.data()), { {{"class"}, {"DPACtm"}}, {{"xmlns"}, {"http://www.w3.org/2000/svg"}}, {{"viewbox"}, {"0 0 112 112"}}, {{"style"}, {"transform: rotate(100.8deg);"}} } });
	div1.AddChild({ "svg", std::move(clock_svg4.data()), { {{"class"}, {"DPACts"}}, {{"xmlns"}, {"http://www.w3.org/2000/svg"}}, {{"viewbox"}, {"0 0 112 112"}}, {{"style"}, {"transform: rotate(288deg);"}} } });
	div.AddChild(div1);
	Tag div2("div", { {{"class"}, {"DPACd"}}, {{"style"}, {"font-size: 27px; color: rgb(255, 255, 255); font-weight: normal;"}} });
	div2.AddChild({ "span", "Fri, September 29", { {{"class"}, {"DPACdt"}}, } });
	Tag span1({ "span", { {{"class"}, {"DPACtn"}}, {{"style"}, {"width: 27px; height: 27px; border-color: rgb(255, 255, 255); display: none;"}} } });
	span1.AddChild({ "i", { {{"style"}, {"background-color: rgb(255, 255, 255);"}} } });
	div2.AddChild(span1);
	div.AddChild(div2);
	Tag div3("div", { {{"class"}, {"DPACs"}}, {{"style"}, {"font-size: 27px; color: rgb(255, 255, 255); display: block;"}} });
	Tag span2("span", { {{"class"}, {"DPACsr"}} });
	span2.AddChild({ "svg", clock_svg5.data(), { {{"xmlns"}, {"http://www.w3.org/2000/svg"}}, {{"viewbox"}, {"0 0 24 24"}} } });
	span2.AddText("07:20");
	span2.AddChild({ "sup", "am", { {{"style"}, {"display:none"}} } });
	div3.AddChild(span2);
	div3.AddChild({ "span", "11:51", { {{"class"}, {"DPACsl"}} } });
	Tag span3("span", { {{"class"}, {"DPACss"}} });
	span3.AddChild({ "svg", clock_svg6.data(), { {{"xmlns"}, {"http://www.w3.org/2000/svg"}}, {{"viewbox"}, {"0 0 24 24"}} } });
	span3.AddText("19:10");
	span3.AddChild({ "sup", "pm", { {{"style"}, {"display:none"}} } });
	div3.AddChild(span3);
	div.AddChild(div3);
	div.AddChild({ "script", clock_script .data()});
	div.AddChild({ "!--/DPAC--" });
	parent->AddChild({ "!--Dayspedia.com widget--" });
	parent->AddChild(div);
	parent->AddChild({ "!--Dayspedia.com widget ENDS--" });
}
