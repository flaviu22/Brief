#pragma once

#include <string>
#include <array>

constexpr std::string_view geo_list = R"([{"id":"AR","name":"Argentina"},{"id":"AU","name":"Australia"},{"id":"AT","name":"Austria"},{"id":"BE","name":"Belgium"},{"id":"BR","name":"Brazil"},{"id":"CA","name":"Canada"},{"id":"CL","name":"Chile"},{"id":"CO","name":"Colombia"},{"id":"CZ","name":"Czechia"},{"id":"DK","name":"Denmark"},{"id":"EG","name":"Egypt"},{"id":"FI","name":"Finland"},{"id":"FR","name":"France"},{"id":"DE","name":"Germany"},{"id":"GR","name":"Greece"},{"id":"HK","name":"Hong Kong"},{"id":"HU","name":"Hungary"},{"id":"IN","name":"India"},{"id":"ID","name":"Indonesia"},{"id":"IE","name":"Ireland"},{"id":"IL","name":"Israel"},{"id":"IT","name":"Italy"},{"id":"JP","name":"Japan"},{"id":"KE","name":"Kenya"},{"id":"MY","name":"Malaysia"},{"id":"MX","name":"Mexico"},{"id":"NL","name":"Netherlands"},{"id":"NZ","name":"New Zealand"},{"id":"NG","name":"Nigeria"},{"id":"NO","name":"Norway"},{"id":"PE","name":"Peru"},{"id":"PH","name":"Philippines"},{"id":"PL","name":"Poland"},{"id":"PT","name":"Portugal"},{"id":"RO","name":"Romania"},{"id":"RU","name":"Russia"},{"id":"SA","name":"Saudi Arabia"},{"id":"SG","name":"Singapore"},{"id":"ZA","name":"South Africa"},{"id":"KR","name":"South Korea"},{"id":"ES","name":"Spain"},{"id":"SE","name":"Sweden"},{"id":"CH","name":"Switzerland"},{"id":"TW","name":"Taiwan"},{"id":"TH","name":"Thailand"},{"id":"TR","name":"Turkiye"},{"id":"UA","name":"Ukraine"},{"id":"GB","name":"United Kingdom"},{"id":"US","name":"United States"},{"id":"VN","name":"Vietnam"}])";

constexpr std::string_view html_style = R"(
			body {
				font-family: Arial;
			/*	color: #111111;
				background-color:#f0fae4;*/

				color: #f9f9ff;
				background-color:#161B1F;
			}

			.split {
				height: 100%;
				width: 50%;
				position: fixed;
				z-index: 1;
				top: 0;
				overflow-x: hidden;
			}

			.left {
				left: 0;
				height: 100%;
				overflow-y: auto;
			/*	background-color: RGB(240, 250, 228);*/
			}

			.right {
				right: 0;
			/*	background-color: RGB(240, 250, 228);*/
			}

			.top-right {
				height: 475px;
			}

			.bottom-right {
				height: calc(100% - 475px);
				overflow-y: auto;
			}

			.weather-current-title {
				font-weight: bold;
				padding-top: 10px;
				padding-bottom: 7px;
			}

			.weather-current {
				text-align:center;
				width: 100%;
				padding-top: 30px;
			}

			.weather-current img {
				display: block;
				margin: 0 auto;
			}

			.weather-hourly {
				float:left;
				margin-left:12px;
				text-align:center;
				line-height:50%;
			/*	border-style:solid;
				border-width:1px;*/
			}

			.weather-daily {
				width:100%;
				height:100px;
				display:flex;
			}

			.weather-daily-date {
				padding-left: 12px;
			}

			.weather-daily>* {
			/*	border: 1px dashed #4b2869;*/
			}

			.weather-daily img {
				margin-left:7px;
				margin-top:12px;
				margin-right:7px;
			}

			.weather-daily>*:first-child {
				display:flex;
				text-align:center;
				align-items:center;
				width:110px;
			}

			.weather-daily>*:last-child {
				flex-grow:1;
				margin-left:17px;
				margin-top:15px;
			}

			.trends {
			}

			.trends-title {
				display: block;
				clear: both;
				font-weight: bold;
			}

			.trends-icon {
				float: left;
				margin-inline-end: .5em;
			}

			.trends-item {
			/*	margin-block-end: 1.5em;*/
			/*	border: 1px dashed #4b2869;*/
			}

			.custom {
				width:100%;
				height:20px;
				padding-left:40px;
				padding-top:1px;
				display:flex;
			/*	border: 1px dashed #4b2869;*/
			}

			.custom-left {
				width:100px;
				text-align:right;
			/*	border: 1px dashed #4b2a69;*/
			}

			.custom-right {
				width:100px;
				text-align:left;
				padding-left:10px;
			/*	border: 1px dashed #4b28f9;*/
			}
			)";

				constexpr std::string_view html_style_short = R"(
			body {
				font-family: Arial;
			/*	color: #111111;
				background-color:#f0fae4;*/

				color: #f9f9ff;
				background-color:#161B1F;
			}

			.center-text {
				display: flex;
				align-items: center;
				justify-content: center;
				height: 90vh;
			}

)";

constexpr std::string_view html_script = R"(
			function Move(State)
			{
				if ("ON" == State)
					document.querySelector('.custom-right').innerHTML = '<font color="#ff1131">ON</font>';
				else
					document.querySelector('.custom-right').innerHTML = '<font color="#33ff11">OFF</font>';
			}
)";
	constexpr std::string_view clock_style = R"(
				/*COMMON*/
				.DPAC{display:table;position:relative;box-sizing:border-box;font-size:100.01%;font-style:normal;font-family:Arial;background-position:50% 50%;background-repeat:no-repeat;background-size:cover;overflow:hidden;user-select:none}
				.DPACh,.DPACd{line-height:1.4}
				.DPACh{margin-bottom:1em}
				.DPACt{position:relative;border-radius:50%;line-height:1;border-style:solid}
				.DPACt svg{display:block;position:absolute;top:0;right:0;bottom:0;left:0}
				.DPACd{margin-top:1em}
				.DPACdm::after{content:' '}
				.DPACda::after{content:', '}
				.DPACdt{margin-right:.48em}
				.DPACtn{display:inline-block;position:relative;width:13px;height:13px;border:2px solid;border-radius:50%;overflow:hidden}
				.DPACtn>i{display:block;content:'';position:absolute;right:33%;top:-5%;width:85%;height:85%;border-radius:50%}
				.DPACs{margin:.96em 0 0 -3px;font-size:90%;line-height:1;white-space:nowrap}
				.DPACs sup{padding-left:.24em;font-size:65%}
				.DPACsl::before,.DPACsl::after{display:inline-block;opacity:.4}
				.DPACsl::before{content:'~';margin:0 .12em}
				.DPACsl::after{content:'~';margin:0 .24em}
				.DPACs svg{display:inline-block;vertical-align:bottom;width:1.2em;height:1.2em;opacity:.48}
				/*CUSTOM*/
				.DPAC{width:1%;padding:24px 24px 32px;background-color:#ffffff;border:1px solid #343434;} /* widget width, padding, background, border, rounded corners */
				.DPAC>div{text-align:center} /* initial text-align */
				.DPACt{width:160px;height:160px;margin:0 auto;background-color:#ECEFF1;border-width:0} /* clock width, background, align */
				.DPACtcf>path{fill:#343434} /* clock face digits color */
				.DPACth>path,.DPACtm>path{fill:#343434} /* hour & minute hands color */
				.DPACts>path{fill:#FF0051} /* second hand color */
				.DPACh{color:#007DBF;font-weight:normal} /* headline color, font-weight*/
				.DPACd{color:#343434;font-weight:bold} /* time & date color, font-weight */
				.DPACtn{border-color:#343434} /* night-sign color = time & date color */
				.DPACtn>i{background-color:#343434} /* night-sign color = time & date color */
				.DPACh,.DPACd{font-size:16px} /* headline & date font-size */
)";


	constexpr std::string_view clock_svg = R"(<path style="fill: rgb(255, 255, 255);" d="M0,0v16h1.7c-0.1-0.2-0.1-0.3-0.1-0.5c0-0.9,0.8-1.6,1.6-1.6c0.9,0,1.6,0.8,1.6,1.6c0,0.2,0,0.3-0.1,0.5h1.8 c-0.1-0.2-0.1-0.3-0.1-0.5c0-0.9,0.8-1.6,1.6-1.6s1.6,0.8,1.6,1.6c0,0.2,0,0.3-0.1,0.5h1.8c-0.1-0.2-0.1-0.3-0.1-0.5 c0-0.9,0.8-1.6,1.6-1.6c0.9,0,1.6,0.8,1.6,1.6c0,0.2,0,0.3-0.1,0.5H16V0H0z M4.2,8H2V2h2.2c2.1,0,3.3,1.3,3.3,3S6.3,8,4.2,8z M11.4,6.3h-0.8V8H9V2h2.5c1.4,0,2.4,0.8,2.4,2.1C13.9,5.6,12.9,6.3,11.4,6.3z M4.4,3.5H3.7v3h0.7C5.4,6.5,6,6,6,5 C6,4.1,5.4,3.5,4.4,3.5z M11.3,3.4h-0.8V5h0.8c0.6,0,0.9-0.3,0.9-0.8C12.2,3.7,11.9,3.4,11.3,3.4z"></path>)";
	constexpr std::string_view clock_svg1 = R"(<path d="M53.1,18.6h-1.6v-7.7h-1.9v-1l3.5-0.7V18.6z" style="fill: rgb(255, 255, 255);"></path><path d="M61.8,18.6h-6.3v-1.1l3.1-3.4c0.5-0.6,0.9-1,1.1-1.4c0.2-0.3,0.3-0.7,0.3-1c0-0.4-0.1-0.8-0.4-1.1 c-0.2-0.3-0.6-0.4-1-0.4c-0.6,0-1,0.2-1.2,0.5C57,11,56.9,11.4,56.9,12h-1.6c0-0.9,0.3-1.6,0.9-2.2c0.6-0.6,1.4-0.9,2.4-0.9 c0.9,0,1.7,0.2,2.2,0.7c0.5,0.5,0.8,1.2,0.8,2c0,0.6-0.2,1.1-0.5,1.6c-0.3,0.5-0.8,1.1-1.4,1.8l-2.1,2.3h4.3V18.6z" style="fill: rgb(255, 255, 255);"></path><path d="M98.9,55.3h1.1c0.5,0,0.9-0.1,1.1-0.4c0.2-0.3,0.3-0.6,0.3-1.1c0-0.5-0.1-0.8-0.4-1.1 c-0.2-0.2-0.6-0.4-1.1-0.4c-0.4,0-0.8,0.1-1.1,0.4c-0.3,0.2-0.4,0.6-0.4,1h-1.6c0-0.8,0.3-1.4,0.8-1.9c0.6-0.5,1.3-0.7,2.2-0.7 c0.9,0,1.7,0.2,2.2,0.7c0.5,0.5,0.8,1.1,0.8,2c0,0.4-0.1,0.8-0.4,1.2c-0.2,0.4-0.6,0.7-1.1,0.9c0.6,0.2,1,0.5,1.2,0.9 c0.2,0.4,0.4,0.8,0.4,1.3c0,0.9-0.3,1.6-0.9,2.1c-0.6,0.5-1.4,0.7-2.3,0.7c-0.9,0-1.6-0.2-2.2-0.7c-0.6-0.5-0.9-1.1-0.9-2h1.6 c0,0.4,0.1,0.8,0.4,1c0.3,0.3,0.7,0.4,1.1,0.4c0.5,0,0.9-0.1,1.2-0.4c0.3-0.3,0.4-0.6,0.4-1.1c0-0.5-0.1-0.9-0.4-1.2 c-0.3-0.2-0.7-0.4-1.2-0.4h-1.1V55.3z" style="fill: rgb(255, 255, 255);"></path><path d="M12.1,59.6c0.6,0,1-0.2,1.4-0.6c0.3-0.4,0.5-1,0.5-1.9v-0.2l0,0c-0.2,0.3-0.5,0.5-0.8,0.6 c-0.3,0.1-0.6,0.2-1,0.2c-0.9,0-1.6-0.3-2.2-0.8c-0.5-0.6-0.8-1.4-0.8-2.4c0-1,0.3-1.8,0.9-2.4c0.6-0.6,1.3-0.9,2.2-0.9 c1,0,1.8,0.3,2.4,1c0.6,0.6,0.9,1.6,0.9,2.8v2.2c0,1.2-0.3,2.2-1,2.8c-0.6,0.7-1.5,1-2.5,1c-0.3,0-0.7,0-1-0.1 c-0.3-0.1-0.7-0.2-1-0.3l0.2-1.2c0.3,0.1,0.6,0.2,0.8,0.3C11.4,59.6,11.8,59.6,12.1,59.6z M12.4,56.5c0.4,0,0.7-0.1,1-0.2 c0.3-0.2,0.5-0.4,0.6-0.6v-1c0-0.8-0.1-1.3-0.4-1.7c-0.3-0.4-0.7-0.6-1.2-0.6c-0.4,0-0.8,0.2-1.1,0.6c-0.3,0.4-0.4,0.9-0.4,1.5 c0,0.6,0.1,1.1,0.4,1.5C11.5,56.4,11.9,56.5,12.4,56.5z" style="fill: rgb(255, 255, 255);"></path><path d="M18.7,40.9h-1.6v-7.7h-1.9v-1l3.5-0.7V40.9z" style="fill: rgb(255, 255, 255);"></path><path d="M27.4,37.2c0,1.3-0.3,2.2-0.8,2.8c-0.5,0.6-1.3,1-2.3,1c-1,0-1.7-0.3-2.3-1c-0.6-0.6-0.8-1.6-0.8-2.8v-2.2 c0-1.3,0.3-2.2,0.8-2.8c0.5-0.6,1.3-1,2.3-1c1,0,1.7,0.3,2.3,1c0.6,0.6,0.8,1.6,0.8,2.8V37.2z M25.8,34.8c0-0.8-0.1-1.4-0.4-1.7 c-0.3-0.4-0.6-0.6-1.1-0.6c-0.5,0-0.9,0.2-1.1,0.6c-0.3,0.4-0.4,0.9-0.4,1.7v2.6c0,0.8,0.1,1.4,0.4,1.8c0.3,0.4,0.6,0.6,1.2,0.6 c0.5,0,0.9-0.2,1.1-0.5s0.4-1,0.4-1.8V34.8z" style="fill: rgb(255, 255, 255);"></path><path d="M96.6,37.9h-6.3v-1.1l3.1-3.4c0.5-0.6,0.9-1,1.1-1.4c0.2-0.3,0.3-0.7,0.3-1c0-0.4-0.1-0.8-0.4-1.1 c-0.2-0.3-0.6-0.4-1-0.4c-0.6,0-1,0.2-1.2,0.5c-0.3,0.3-0.4,0.7-0.4,1.3h-1.6c0-0.9,0.3-1.6,0.9-2.2c0.6-0.6,1.4-0.9,2.4-0.9 c0.9,0,1.7,0.2,2.2,0.7c0.5,0.5,0.8,1.2,0.8,2c0,0.6-0.2,1.1-0.5,1.6c-0.3,0.5-0.8,1.1-1.4,1.8l-2.1,2.3h4.3V37.9z" style="fill: rgb(255, 255, 255);"></path><path d="M21.8,76.2c0,0.5-0.1,0.9-0.4,1.3c-0.3,0.4-0.6,0.6-1.1,0.8c0.5,0.2,0.9,0.5,1.2,0.9c0.3,0.4,0.5,0.9,0.5,1.4 c0,0.9-0.3,1.6-0.9,2s-1.3,0.7-2.3,0.7c-1,0-1.7-0.2-2.3-0.7c-0.6-0.5-0.9-1.1-0.9-2c0-0.5,0.1-1,0.4-1.4c0.3-0.4,0.7-0.7,1.3-0.9 c-0.5-0.2-0.8-0.5-1.1-0.8c-0.3-0.4-0.4-0.8-0.4-1.3c0-0.9,0.3-1.5,0.8-2c0.5-0.5,1.2-0.7,2.1-0.7c0.9,0,1.6,0.2,2.1,0.7 C21.5,74.7,21.8,75.4,21.8,76.2z M20.4,80.6c0-0.5-0.1-0.9-0.4-1.1c-0.3-0.3-0.7-0.4-1.1-0.4c-0.5,0-0.9,0.1-1.1,0.4 c-0.3,0.3-0.4,0.7-0.4,1.1c0,0.5,0.1,0.9,0.4,1.1c0.3,0.3,0.7,0.4,1.2,0.4c0.5,0,0.9-0.1,1.1-0.4S20.4,81,20.4,80.6z M20.2,76.3 c0-0.4-0.1-0.8-0.4-1c-0.2-0.3-0.6-0.4-1-0.4c-0.4,0-0.7,0.1-1,0.4c-0.2,0.3-0.4,0.6-0.4,1c0,0.4,0.1,0.8,0.4,1 c0.2,0.3,0.6,0.4,1,0.4c0.4,0,0.7-0.1,1-0.4C20.1,77.1,20.2,76.7,20.2,76.3z" style="fill: rgb(255, 255, 255);"></path><path d="M95.6,78.5h1.2v1.3h-1.2v2.1h-1.6v-2.1H90l0-1l4.1-6.4h1.6V78.5z M91.5,78.5h2.6v-3.9l0,0l-0.1,0.3L91.5,78.5 z" style="fill: rgb(255, 255, 255);"></path><path d="M37.7,88.6c-1.1,1.3-1.9,2.5-2.2,3.5c-0.4,1-0.6,2.3-0.8,3.8l-0.1,1H33l0.1-1c0.2-1.5,0.5-2.8,1-3.9 c0.5-1.1,1.2-2.3,2.1-3.3h-5v-1.3h6.6V88.6z" style="fill: rgb(255, 255, 255);"></path><path d="M34,24.6h-1.6v-7.7h-1.9v-1l3.5-0.7V24.6z" style="fill: rgb(255, 255, 255);"></path><path d="M40.1,24.6h-1.6v-7.7h-1.9v-1l3.5-0.7V24.6z" style="fill: rgb(255, 255, 255);"></path><path d="M79.3,24.6h-1.6v-7.7h-1.9v-1l3.5-0.7V24.6z" style="fill: rgb(255, 255, 255);"></path><path d="M76,92.5l0.5-5.3h5v1.4h-3.7L77.6,91c0.2-0.1,0.4-0.3,0.6-0.3c0.2-0.1,0.5-0.1,0.8-0.1c0.9,0,1.6,0.3,2.1,0.8 c0.5,0.6,0.7,1.4,0.7,2.4c0,0.9-0.3,1.7-0.8,2.3c-0.5,0.6-1.3,0.9-2.3,0.9c-0.9,0-1.6-0.2-2.1-0.7s-0.9-1.1-0.9-2l1.5-0.1 c0,0.5,0.1,0.8,0.4,1.1c0.3,0.3,0.6,0.4,1.1,0.4c0.5,0,0.9-0.2,1.1-0.5c0.3-0.3,0.4-0.8,0.4-1.4c0-0.6-0.1-1.1-0.4-1.4 c-0.3-0.4-0.6-0.5-1.1-0.5c-0.4,0-0.8,0.1-1,0.2c-0.2,0.1-0.4,0.3-0.5,0.6L76,92.5z" style="fill: rgb(255, 255, 255);"></path><path d="M56.4,93.3c0.3,0,0.7,0,1,0.1c0.3,0.1,0.6,0.2,0.8,0.3l-0.3,1.2c-0.2-0.1-0.5-0.2-0.7-0.2 c-0.2-0.1-0.5-0.1-0.8-0.1c-0.6,0-1.2,0.2-1.5,0.7c-0.4,0.5-0.6,1.1-0.6,2v0c0.3-0.2,0.6-0.4,0.9-0.6c0.3-0.1,0.7-0.2,1.1-0.2 c0.9,0,1.5,0.3,2,0.9c0.5,0.6,0.7,1.3,0.7,2.3c0,1-0.3,1.8-0.8,2.4c-0.6,0.6-1.3,0.9-2.2,0.9c-1,0-1.8-0.3-2.4-1 c-0.6-0.7-0.9-1.6-0.9-2.9v-1.8c0-1.3,0.3-2.3,1-3S55.3,93.3,56.4,93.3z M56,97.8c-0.4,0-0.7,0.1-1,0.2c-0.3,0.1-0.5,0.3-0.6,0.6 v0.7c0,0.8,0.2,1.4,0.5,1.9c0.3,0.4,0.7,0.6,1.3,0.6c0.5,0,0.8-0.2,1.1-0.6c0.3-0.4,0.4-0.9,0.4-1.5c0-0.6-0.1-1-0.4-1.4 S56.5,97.8,56,97.8z" style="fill: rgb(255, 255, 255);"></path><path d="M57.5,107.6c0,0.8-0.7,1.5-1.5,1.5s-1.5-0.7-1.5-1.5s0.7-1.5,1.5-1.5S57.5,106.8,57.5,107.6z M4.5,54.5 C3.7,54.5,3,55.2,3,56s0.7,1.5,1.5,1.5S6,56.8,6,56S5.3,54.5,4.5,54.5z M11,80.5c-0.8,0-1.5,0.7-1.5,1.5s0.7,1.5,1.5,1.5 s1.5-0.7,1.5-1.5S11.8,80.5,11,80.5z M101,80.5c-0.8,0-1.5,0.7-1.5,1.5s0.7,1.5,1.5,1.5s1.5-0.7,1.5-1.5S101.8,80.5,101,80.5z M30,99c-0.8,0-1.5,0.7-1.5,1.5s0.7,1.5,1.5,1.5s1.5-0.7,1.5-1.5S30.8,99,30,99z M83,99c-0.8,0-1.5,0.7-1.5,1.5s0.7,1.5,1.5,1.5 s1.5-0.7,1.5-1.5S83.8,99,83,99z M107.5,54.5c-0.8,0-1.5,0.7-1.5,1.5s0.7,1.5,1.5,1.5s1.5-0.7,1.5-1.5S108.3,54.5,107.5,54.5z M56,6 c-0.8,0-1.5-0.7-1.5-1.5S55.2,3,56,3s1.5,0.7,1.5,1.5S56.8,6,56,6z M12.5,30.1c0-0.8-0.7-1.5-1.5-1.5s-1.5,0.7-1.5,1.5 s0.7,1.5,1.5,1.5S12.5,31,12.5,30.1z M102.5,30.1c0-0.8-0.7-1.5-1.5-1.5s-1.5,0.7-1.5,1.5s0.7,1.5,1.5,1.5S102.5,31,102.5,30.1z M31.5,11.6c0-0.8-0.7-1.5-1.5-1.5s-1.5,0.7-1.5,1.5s0.7,1.5,1.5,1.5S31.5,12.5,31.5,11.6z M84.5,11.6c0-0.8-0.7-1.5-1.5-1.5 s-1.5,0.7-1.5,1.5s0.7,1.5,1.5,1.5S84.5,12.5,84.5,11.6z" style="fill: rgb(255, 255, 255);"></path>)";
	constexpr std::string_view clock_svg2 = R"(<path d="M56,33c0.6,0,1,0.4,1,1v22c0,0.6-0.4,1-1,1s-1-0.4-1-1V34C55,33.4,55.4,33,56,33 M56,31c-1.7,0-3,1.3-3,3v22 c0,1.7,1.3,3,3,3c1.7,0,3-1.3,3-3V34C59,32.3,57.7,31,56,31L56,31z" style="fill: rgb(255, 255, 255);"></path>)";
	constexpr std::string_view clock_svg3 = R"(<path d="M57,52.1V23c0-0.6-0.4-1-1-1s-1,0.4-1,1v29.1c-1.7,0.4-3,2-3,3.9c0,2.2,1.8,4,4,4s4-1.8,4-4 C60,54.1,58.7,52.6,57,52.1z" style="fill: rgb(255, 255, 255);"></path>)";
	constexpr std::string_view clock_svg4 = R"(<path d="M58,56.5c0-0.8-0.5-1.6-1.2-1.9L56.1,7.5h-0.2l-0.6,47.1c-0.7,0.3-1.3,1-1.3,1.9c0,0.8,0.5,1.5,1.2,1.8L55,66.7 h2l-0.2-8.4C57.5,58,58,57.3,58,56.5z"></path>)";
	constexpr std::string_view clock_svg5 = R"(<path d="M12,4L7.8,8.2l1.4,1.4c0,0,0.9-0.9,1.8-1.8V14h2c0,0,0-3.3,0-6.2l1.8,1.8l1.4-1.4L12,4z" style="fill: rgb(255, 255, 255);"></path><path d="M6.8,15.3L5,13.5l-1.4,1.4l1.8,1.8L6.8,15.3z M4,21H1v2h3V21z M20.5,14.9L19,13.5l-1.8,1.8l1.4,1.4L20.5,14.9z M20,21v2h3 v-2H20z M6.1,23C6,22.7,6,22.3,6,22c0-3.3,2.7-6,6-6s6,2.7,6,6c0,0.3,0,0.7-0.1,1H6.1z" style="fill: rgb(255, 255, 255);"></path>)";
	constexpr std::string_view clock_svg6 = R"(<path d="M12,14L7.8,9.8l1.4-1.4c0,0,0.9,0.9,1.8,1.8V4h2c0,0,0,3.3,0,6.2l1.8-1.8l1.4,1.4L12,14z" style="fill: rgb(255, 255, 255);"></path><path d="M6.8,15.3L5,13.5l-1.4,1.4l1.8,1.8L6.8,15.3z M4,21H1v2h3V21z M20.5,14.9L19,13.5l-1.8,1.8l1.4,1.4L20.5,14.9z M20,21v2h3 v-2H20z M6.1,23C6,22.7,6,22.3,6,22c0-3.3,2.7-6,6-6s6,2.7,6,6c0,0.3,0,0.7-0.1,1H6.1z" style="fill: rgb(255, 255, 255);"></path>)";
	constexpr std::string_view clock_script = R"(
				var s, t; s = document.createElement("script"); s.type = "text/javascript";
				s.src = "https://cdn.dayspedia.com/js/dwidget.min.vc01b6c64.js";
				t = document.getElementsByTagName('script')[0]; t.parentNode.insertBefore(s, t);
				s.onload = function() {
					window.dwidget = new window.DigitClock();
					window.dwidget.init("dayspedia_widget_3541d001a09bd33e");
				};
)";

	constexpr std::array<std::pair<std::string_view, size_t>, 177> time_zones
	{ {
		{"UTC-10:00 Hawaii", 41016},
		{"UTC-9:00 Alaska", 8330},
		{"UTC-8:00 Baja California", 2095},
		{"UTC-7:00 Arizona", 9224},
		{"UTC-7:00 La Paz", 2353},
		{"UTC-7:00 Mazatlan", 2257},
		{"UTC-7:00 Yukon", 5555},
		{"UTC-6:00 Guadalajara", 2027},
		{"UTC-6:00 Mexico City", 2027},
		{"UTC-6:00 Monterrey", 2028},
		{"UTC-6:00 Saskatchewan", 3296},
		{"UTC-5:00 Bogota", 45129},
		{"UTC-5:00 Lima", 7802},
		{"UTC-5:00 Quito", 9648},
		{"UTC-5:00 Rio Branco", 5504},
		{"UTC-5:00 Chetumal", 2408},
		{"UTC-5:00 Haiti", 10878},
		{"UTC-5:00 Havana", 10749},
		{"UTC-5:00 Turks and Caicos", 18642},
		{"UTC-4:00 Asuncion", 18371},
		{"UTC-4:00 Caracas", 10756},
		{"UTC-4:00 Cuiaba", 5930},
		{"UTC-4:00 Georgetown", 55170},
		{"UTC-4:00 La Paz", 56964},
		{"UTC-4:00 Manaus", 6270},
		{"UTC-4:00 San Juan",  10425},
		{"UTC-4:00 Santiago", 10307},
		{"UTC-3:30 Newfoundland", 3161},
		{"UTC-3:00 Araguaina", 15585},
		{"UTC-3:00 Brasilia", 5510},
		{"UTC-3:00 Cayenne", 12983},
		{"UTC-3:00 Fortaleza", 5782},
		{"UTC-3:00 City of Buenos Aires", 4418},
		{"UTC-3:00 Montevideo", 9335},
		{"UTC-3:00 Punta Arenas", 4706},
		{"UTC-3:00 Saint Pierre and Miquelon", 17998},
		{"UTC-3:00 Salvador", 5525},
		{"UTC-2:00 Greenland", 10512},
		{"UTC-1:00 Azores", 6784},
		{"UTC-1:00 Cabo Verde Is.", 11647},
		{"UTC+00:00 Dublin", 1892},
		{"UTC+00:00 Edinburgh", 7341},
		{"UTC+00:00 Lisbon", 6568},
		{"UTC+00:00 London", 4254},
		{"UTC+00:00 Monrovia", 12588},
		{"UTC+00:00 Reykjavik", 3022},
		{"UTC+00:00 Sao Tome", 12986},
		{"UTC+1:00 Casablanca", 7043},
		{"UTC+1:00 Amsterdam", 2750},
		{"UTC+1:00 Berlin", 1293},
		{"UTC+1:00 Bern", 78},
		{"UTC+1:00 Rome", 3171},
		{"UTC+1:00 Stockholm", 7305},
		{"UTC+1:00 Vienna", 643},
		{"UTC+1:00 Belgrade", 4276},
		{"UTC+1:00 Bratislava", 8849},
		{"UTC+1:00 Budapest", 7952},
		{"UTC+1:00 Ljubljana", 8375},
		{"UTC+1:00 Prague", 5993},
		{"UTC+1:00 Brussels", 3780},
		{"UTC+1:00 Copenhagen", 3019},
		{"UTC+1:00 Madrid", 674},
		{"UTC+1:00 Paris", 1020},
		{"UTC+1:00 Sarajevo", 8865},
		{"UTC+1:00 Skopje", 4584},
		{"UTC+1:00 Warsaw", 5443},
		{"UTC+1:00 Zagreb", 8239},
		{"UTC+2:00 Athens", 5379},
		{"UTC+2:00 Bucharest", 6410},
		{"UTC+2:00 Beirut", 11393},
		{"UTC+2:00 Cairo", 9289},
		{"UTC+2:00 Chisinau", 576},
		{"UTC+2:00 Gaza", 8538},
		{"UTC+2:00 Hebron", 8607},
		{"UTC+2:00 Harare", 10534},
		{"UTC+2:00 Pretoria", 3928},
		{"UTC+2:00 Helsinki", 8708},
		{"UTC+2:00 Kyiv", 1133},
		{"UTC+2:00 Riga", 9638},
		{"UTC+2:00 Sofia", 9918},
		{"UTC+2:00 Tallinn", 9581},
		{"UTC+2:00 Vilnius", 9793},
		{"UTC+2:00 Jerusalem", 3021},
		{"UTC+2:00 Juba", 12954},
		{"UTC+2:00 Kaliningrad", 7559},
		{"UTC+2:00 Khartoum", 10368},
		{"UTC+2:00 Tripoli", 17088},
		{"UTC+2:00 Windhoek", 11621},
		{"UTC+2:00 Tripoli", 12590},
		{"UTC+3:00 Amman", 12060},
		{"UTC+3:00 Baghdad", 10165},
		{"UTC+3:00 Damascus", 12287},
		{"UTC+3:00 Istanbul", 4404},
		{"UTC+3:00 Kuwait", 11230},
		{"UTC+3:00 Riyadh", 9694},
		{"UTC+3:00 Minsk", 1588},
		{"UTC+3:00 Moscow", 4080},
		{"UTC+3:00 St.Petersburg", 347},
		{"UTC+3:00 Nairobi", 1930},
		{"UTC+3:00 Volgograd", 5939},
		{"UTC+3:30 Tehran", 10069},
		{"UTC+4:00 Abu Dhabi", 6409},
		{"UTC+4:00 Muscat", 12977},
		{"UTC+4:00 Astrakhan", 8089},
		{"UTC+4:00 Ulyanovsk", 7605},
		{"UTC+4:00 Baku", 12017},
		{"UTC+4:00 Izhevsk", 3514},
		{"UTC+4:00 Samara", 7590},
		{"UTC+4:00 Port Louis", 10633},
		{"UTC+4:00 Saratov", 8296},
		{"UTC+4:00 Tbilisi", 12083},
		{"UTC+4:00 Yerevan", 11987},
		{"UTC+4:30 Kabul", 2671},
		{"UTC+5:00 Ashgabat", 12061},
		{"UTC+5:00 Tashkent", 12082},
		{"UTC+5:00 Ekaterinburg", 1617},
		{"UTC+5:00 Islamabad", 8632},
		{"UTC+5:00 Karachi", 8642},
		{"UTC+5:00 Qyzylorda", 18597},
		{"UTC+5:30 Chennai", 185},
		{"UTC+5:00 Kolkata", 244},
		{"UTC+5:00 Mumbai", 187},
		{"UTC+5:00 New Delhi", 181},
		{"UTC+5:30 Sri Jayawardenepura", 27578},
		{"UTC+5:45 Kathmandu", 9807},
		{"UTC+6:00 Astana", 6657},
		{"UTC+6:00 Dhaka", 9611},
		{"UTC+6:00 Omsk", 7588},
		{"UTC+6:30 Yangon (Rangoon)", 10380},
		{"UTC+7:00 Bangkok", 4251},
		{"UTC+7:00 Hanoi", 7853},
		{"UTC+7:00 Jakarta", 6377},
		{"UTC+7:00 Barnaul", 7089},
		{"UTC+7:00 Gorno-Altaysk", 14745},
		{"UTC+7:00 Hovd", 20761},
		{"UTC+7:00 Krasnoyarsk", 7589},
		{"UTC+7:00 Novosibirsk", 5942},
		{"UTC+7:00 Tomsk", 5720},
		{"UTC+8:00 Beijing", 9635},
		{"UTC+8:00 Chongqing", 56245},
		{"UTC+8:00 Hong Kong", 9626},
		{"UTC+8:00 Urumqi", 18653},
		{"UTC+8:00 Irkutsk", 2074},
		{"UTC+8:00 Kuala Lumpur", 6957},
		{"UTC+8:00 Singapore", 5329},
		{"UTC+8:00 Perth", 215},
		{"UTC+8:00 Taipei", 11526},
		{"UTC+8:00 Ulaanbaatar", 12629},
		{"UTA+9:00 Chita", 8164},
		{"UTC+9:00 Osaka", 4318},
		{"UTC+9:00 Sapporo", 17513},
		{"UTC+9:00 Tokyo", 4317},
		{"UTC+9:00 Pyongyang", 12984},
		{"UTC+9:00 Seoul", 9479},
		{"UTC+9:00 Yakutsk", 8165},
		{"UTC+9:30 Adelaide", 1882},
		{"UTC+9:30 Darwin", 1883},
		{"UTC+10:00 Brisbane", 208},
		{"UTC+10:00 Canberra", 1611},
		{"UTC+10:00 Melbourne", 207},
		{"UTC+10:00 Sydney", 206},
		{"UTC+10:00 Guam", 11983},
		{"UTC+10:00 Port Moresby", 12910},
		{"UTC+10:00 Hobart", 1884},
		{"UTC+10:00 Vladivostok", 5721},
		{"UTC+11:00 Bougainville Island", 20271},
		{"UTC+11:00 Magadan", 7986},
		{"UTC+11:00 Solomon Islands", 15828},
		{"UTC+11:00 New Caledonia", 11901},
		{"UTC+12:00 Anadyr", 7985},
		{"UTC+12:00 Petropavlovsk-Kamchatsky", 15564},
		{"UTC+12:00 Auckland", 1921},
		{"UTC+12:00 Wellington", 3874},
		{"UTC+12:00 Fiji", 10653},
		{"UTC+12:45 Chatham Islands", 56312},
		{"UTC+13:00 Nuku'alofa", 12930},
		{"UTC+13:00 Samoa", 12987}
	} };
