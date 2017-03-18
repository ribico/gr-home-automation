
#define NTP_SERVER_IP "192.168.1.4"
#define AUTO_ON_STEPS   16


#include "NTPClient.h"

WiFiUDP ntpUDP;

// By default 'time.nist.gov' is used with 60 seconds update interval and
// no offset
//NTPClient timeClient(ntpUDP);

// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
NTPClient timeClient(ntpUDP, NTP_SERVER_IP, 3600, 60000);


#define AQUARIUM_LIGHT_DEFAULT_START_TIME   15
#define AQUARIUM_LIGHT_DEFAULT_DURATION     8

#define AquariumLightAutoOn() RemoteInput(RS485_ADDRESS_ROW2B4, ROW2B4_LIGHT_AQUARIUM, Souliss_T1n_AutoCmd + AUTO_ON_STEPS)
#define AquariumLightStart() AQUARIUM_LIGHT_DEFAULT_START_TIME
#define AquariumLightDuration() ((int) mOutputAsFloat(LYTB1_AQUARIUM_LIGHT_DURATION))

inline void AquariumLightScheduler(int start_hours, int end_hours)
{
    grhSendUDPMessage( (String("Aquarium: start: ") + String(start_hours) + String(" stop: ") 
                        + String(end_hours)).c_str());

    int current_hours = timeClient.getHours();

    if(end_hours > start_hours) // ending before midnight
        if( current_hours >= start_hours && current_hours < end_hours )
        {
            grhSendUDPMessage("AquariumLightAutoOn command");
            AquariumLightAutoOn();
        }
    else // ending after midnight
        if( current_hours >= start_hours || current_hours < end_hours )
        {
            grhSendUDPMessage("AquariumLightAutoOn command");
            AquariumLightAutoOn();
        }
              
}






#define SCHEDULER_BEGIN   timeClient.begin()

inline void SchedulerRun()
{
    timeClient.update();
    grhSendUDPMessage(timeClient.getFormattedTime().c_str());

    AquariumLightScheduler(AquariumLightStart(), (AquariumLightStart()+AquariumLightDuration())%24 );
}