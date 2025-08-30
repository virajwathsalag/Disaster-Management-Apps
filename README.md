# Disaster Management System for Sri Lanka

## Overview

This project provides a disaster management system for Sri Lanka, focusing on early warning and response for **floods** and **landslides**. It leverages real-time data from sensors (e.g., water level, soil moisture, rainfall, seismic) to enable proactive disaster mitigation. The system comprises two applications:

1. **Villager Pre-Warning Mobile App**: An Android app delivering real-time alerts to villagers about potential floods and landslides, with an SOS feature for emergency rescue requests.
2. **Admin Disaster Command Center**: A desktop application built with Electron JS for authorities to monitor sensor activity, manage SOS calls, and generate reports for disaster response and research.

Data is processed in real-time using **Firebase Realtime Database** for the mobile app, while historical data is stored in a **MySQL database** for research and analysis. The system is designed to support Sri Lanka’s disaster-prone regions and align with national disaster management efforts.

## Features

### Common Features
- Real-time integration of sensor data for flood (water levels, rainfall) and landslide (soil stability, ground movement) monitoring.
- Persistent storage in MySQL for historical data analysis and research.
- Scalable architecture to support Sri Lanka’s disaster management needs.

### Villager Pre-Warning Mobile App (Android)
- Real-time push notifications for flood and landslide warnings (e.g., "Flood Alert: Evacuate Now").
- Simple UI showing current sensor readings and local risk levels.
- SOS button to send emergency rescue requests with GPS location.
- Powered by Firebase Realtime Database for low-latency alerts.
- Offline mode for basic alerts using cached data.
- Location-based alerts tailored to user’s region in Sri Lanka.

### Admin Disaster Command Center (Electron JS)
- Real-time dashboard with visualizations (maps, graphs, heatmaps) of sensor activity across Sri Lanka.
- Management of SOS calls from the mobile app, including location tracking and response coordination.
- Report generation (PDF/CSV) for sensor data, incident logs, and disaster analytics.
- Role-based access for admins (e.g., view-only vs. full control).
- MySQL integration for querying historical data for research.
- Alerts for critical sensor thresholds to trigger immediate action.

## Technologies Used
- **Mobile App**: Android (Java/Kotlin), Firebase Realtime Database, Firebase Authentication, Google Maps API.
- **Admin Command Center**: Electron JS (HTML/CSS/JS for UI, Node.js for backend).
- **Databases**:
  - **Firebase Realtime Database**: Real-time data syncing for mobile app alerts and SOS.
  - **MySQL**: Persistent storage for historical data and research queries.
- **Sensor Integration**: Supports API feeds from sensors (extendable with IoT protocols like MQTT).
- **Visualization**: Chart.js for admin dashboard graphs.
- **Other**: Secure data transmission, Firebase push notifications.

## Architecture
- **Data Flow**:
  1. Sensors send data to a central server via API.
  2. Firebase Realtime Database syncs data for mobile app alerts.
  3. MySQL stores all data for long-term analysis.
  4. Mobile app receives push notifications via Firebase.
  5. SOS requests from mobile app are routed to Firebase and displayed in the admin app.
  6. Admin app queries MySQL for reports and analytics.
- **Security**: Firebase Authentication for users/admins, encrypted API calls, MySQL access controls.

## Installation

### Prerequisites
- **Mobile App**: Android Studio, Firebase project with Realtime Database and Authentication.
- **Admin App**: Node.js, npm, Electron JS.
- **Database**: MySQL server (local or cloud, e.g., AWS RDS).
- **Optional**: Google Maps API key for location services, sensor API endpoints.

### Mobile App Setup
1. Clone the repository:
   ```bash
   git clone https://github.com/your-repo/disaster-management-sri-lanka.git
