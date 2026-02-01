C++/MFC application that uses https://github.com/flaviu22/domtree

![image](https://github.com/user-attachments/assets/0d6e9be7-1fb3-4c74-bef4-737c378a531b)

Brief is a lightweight application that displays essential information at a glance, combining time‑related, environmental, and current trending data in a single view. Since it retrieves information from the internet, it relies on two external data sources: weather data and Google & YouTube trends. Both services are free to use, but each requires a simple account setup:

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



Configuring the Brief Application

Once both API keys are ready, setting up the application is very simple.

Step 1: Open the Settings

In the Brief app, go to: Edit → Settings

Step 2: Enter Your API Keys

Paste the Meteosource API key into the Weather group section.

Paste the Google API key into the YouTube Trends group section.

Save. Now, you are good to go.



You can adjust several optional settings:

Weather settings: Enable or disable weather updates, choose how many forecast hours to display (3–20), and how many forecast days to show (1–6). You can enable or disable weather data retrieval by checking un un-checking the box "Stop getting weather data".

Google Trends: Enable or disable Google Trends retrieval by checking un un-checking the box "Stop getting Google trends", this option basically enable or disable the thread responsible with Google data retrieval.

YouTube Trends: Enable or disable YouTube Trends retrieval by checking un un-checking the box "Stop getting Youtube trends", this option basically enable or disable the thread responsible with Youtube data retrieval.

Time Zone: From Edit → Time Zone, select the time zone you want the clock to use.

Weather Location: From Edit → Weather Location, choose the location for weather reporting.

Trends Location: From Edit → Trends Location, select the country from which Google and YouTube trends should be retrieved.

The application can automatically minimize itself to the taskbar and restore back at a set interval (adjustable from Edit → Move Time). This periodic movement prevents the screen saver or automatic standby from activating on your machine, and it can be started or stopped through menu File → Move.
