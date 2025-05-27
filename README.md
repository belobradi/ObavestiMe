### **Project Roadmap: Automated Outage Notifications**

This roadmap outlines the key phases and major components needed to establish a system where users can subscribe to email notifications for power outages affecting specific streets, starting from your core C++ scraper.

**Phase 1: Core Data Collection & Basic API (Foundational Work)**

1.  **Develop Core C++ Web Scraper Logic:**
    
    *   Implement C++ code to fetch the HTML outage page.
        
    *   Parse the HTML and extract OutageRecord data.
        
    *   (Initial Output): Print parsed data to console.
        
    *   **Status:** _Implied initial work, pre-Docker._
        
2.  **Dockerize C++ Web Scraper Application:**
    
    *   Create a Dockerfile to build and run the C++ scraper in a container.
        
    *   Define a basic docker-compose.yml for the scraper service.
        
    *   **Status:** _Planned, next immediate task._
        
3.  **Setup SQLite Database for Outage Data Persistence:**
    
    *   Integrate SQLite with the C++ scraper.
        
    *   Define the planned\_outages table schema.
        
    *   Implement C++ logic to insert/update scraped data into SQLite.
        
    *   Configure a Docker volume for persistent SQLite data.
        
    *   **Status:** _Planned, after Dockerizing scraper._
        
4.  **Implement Python Flask API for Outage Data (GET Endpoints):**
    
    *   Develop a Flask application to run in its own Docker container.
        
    *   Configure Flask to read data from the shared SQLite planned\_outages table.
        
    *   Expose GET endpoints: GET /api/outages (all) and GET /api/outages/search?street= (filtered).
        
    *   **Status:** _Planned, after SQLite setup._
        

**Phase 2: User Subscription & Notification System**

1.  **Implement Subscription Management API (Flask Extension):**
    
    *   **Extend Flask API:** Add a new POST /api/subscribe endpoint to receive user email and street name.
        
    *   **Extend SQLite Database:** Create a new subscriptions table (id, email, street\_name, created\_at, last\_notified\_date).
        
    *   Implement validation and insertion logic for new subscriptions into the subscriptions table.
        
    *   **Status:** _New major feature._
        
2.  **Develop Outage Notification Service (Python Worker):**
    
    *   Create a separate Python script or module dedicated to sending notifications.
        
    *   This service will run periodically (e.g., in a separate Docker container, or as a scheduled task).
        
    *   **Logic:**
        
        *   Connect to the SQLite database.
            
        *   Compare new outages (from planned\_outages) with active subscriptions (from subscriptions).
            
        *   Implement logic to prevent duplicate notifications for the same outage on the same day (using last\_notified\_date in subscriptions table).
            
    *   **Status:** _New major feature._
        
3.  **Integrate Email Sending Functionality:**
    
    *   Choose and integrate an email sending library (e.g., Python's smtplib for direct SMTP, or an API client for SendGrid/Mailgun/etc. for better deliverability).
        
    *   Implement the actual email composition and sending within the notification service.
        
    *   **Status:** _Part of notification service._
        
4.  **Schedule Notification Service Execution:**
    
    *   Determine how the notification worker will run periodically on your server. Options include:
        
        *   cron job configured within the Docker container of the worker.
            
        *   A dedicated scheduler within a Python framework (e.g., APScheduler).
            
        *   A cloud-native scheduling service (if you transition beyond a single VPS).
            
    *   **Status:** _Deployment consideration for notification._
        

**Phase 3: Initial Frontend (Future - when you decide to build a UI)**

1.  **Develop Simple Web Frontend for Subscription:**
    
    *   Create a basic HTML page with a form for email and street name input.
        
    *   Use JavaScript to send form data to your POST /api/subscribe endpoint.
        
    *   **Status:** _Future UI consideration._
        

This roadmap provides a clear path forward, breaking down the complex notification feature into manageable, dependent steps.
