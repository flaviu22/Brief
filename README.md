C++/MFC application that uses https://github.com/flaviu22/domtree

![image](https://github.com/user-attachments/assets/0d6e9be7-1fb3-4c74-bef4-737c378a531b)

This application presents a set of essential information displayed in a single HTML‑based view, as follow:

Analog clock for your time zone, along with the current date, sunrise time, sunset time, day length, and a sunset indicator.

Weather data, including a summary, hourly forecast, and daily forecast.

The current Google Trends tailored to your region or country.

The current YouTube Trends based on your region or country.

1. Weather Data Setup (Meteosource)

Brief retrieves weather information from Meteosource, a global weather API provider.

Step 1: Create a Meteosource Account

Visit: https://www.meteosource.com 

Creating an account is free and straightforward.

Step 2: Register

Go directly to the signup page: https://www.meteosource.com/client/sign-in 

After completing the registration, Meteosource will generate an API access key. Keep it accessible—you’ll enter it later in the app’s Settings.


2. YouTube Trends Setup (Google Cloud API)

To retrieve YouTube trending data, Brief uses the YouTube Data API v3, which requires a Google account.

Step 1: Sign in with Your Google Account

Most users already have one (especially if you use an Android device).

If you don’t, you can create one here: https://www.google.com 

Step 2: Open Google Cloud Console

Go to: https://console.cloud.google.com/apis/credentials 

Step 3: Create or Select a Project

If this is your first time using Google Cloud:

Click Create Project

Give it a name (e.g., BriefApp)

Click Create

Otherwise, select an existing project from the top-left dropdown.

Step 4: Enable the YouTube Data API v3

Open the left menu → APIs & Services → Library

Search for YouTube Data API v3

Click Enable

Step 5: Create API Credentials

Go to APIs & Services → Credentials

Click Create Credentials

Select API key

A new API key will be generated instantly.  Keep it accessible—you’ll enter it later in the app’s Settings.
