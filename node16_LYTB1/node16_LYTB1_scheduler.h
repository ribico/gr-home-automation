
#define NTP_SERVER_IP "192.168.1.4"
#define AUTO_ON_STEPS   16

#define AQUARIUM_LIGHT_DEFAULT_START_HOURS   16 /* hours from midnight UTC+1*/
#define AQUARIUM_LIGHT_DEFAULT_DURATION     8 /* duration espressed in hours */

#define WATERING_ZONE1_DEFAULT_START_HOURS   5 /* hours from midnight UTC+1 */
#define WATERING_ZONE1_DEFAULT_START_MIN    0 

#define WATERING_ZONE1_DEFAULT_DURATION     5 /* duration espressed in minutes */
#define WATERING_ZONE2_DEFAULT_DURATION     5 /* duration espressed in minutes */
#define WATERING_ZONE3_DEFAULT_DURATION     5 /* duration espressed in minutes */
#define WATERING_ZONE4_DEFAULT_DURATION     5 /* duration espressed in minutes */
#define WATERING_ZONE5_DEFAULT_DURATION     5 /* duration espressed in minutes */
#define WATERING_ZONE6_DEFAULT_DURATION     5 /* duration espressed in minutes */

#define SANITARY_WATER_PRODUCTION_DEFAULT_START_HOURS   10 /* hours from midnight UTC+1 */
#define SANITARY_WATER_PRODUCTION_DEFAULT_START_MIN     0 
#define SANITARY_WATER_PRODUCTION_DEFAULT_DURATION      30 /* duration espressed in minutes */

#include "NTPClient.h"

WiFiUDP ntpUDP;

// By default 'time.nist.gov' is used with 60 seconds update interval and
// no offset
//NTPClient timeClient(ntpUDP);

// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
NTPClient timeClient(ntpUDP, NTP_SERVER_IP, 3600, 60000);

inline bool IsCurrentTimeInRange(unsigned start_h, unsigned start_m, unsigned end_h, unsigned end_m)
{
    unsigned current_h = timeClient.getHours();
    unsigned current_m = timeClient.getMinutes();

    if(end_h > start_h) // range not crossing midnight
    {
        if( current_h >= start_h && current_m >= start_m ) // after the start time
        {
            if ( current_h < end_h || ( (current_h == end_h && (current_m < end_m) ) ) )// before the end time
            {
                return true; // in the range
            }    
        }
    }
    else // range over midnight
    {
        if( current_h >= start_h && current_m >= start_m ) // after the start time
            return true;

        if( current_h < end_h && current_m < end_m ) // before end time
            return true; 
    }
 
    return false;
}


// helper macros
#define END_H(start_h,start_m,duration_h,duration_m) ( (start_h + duration_h + (start_m+duration_m)/60)%24 )
#define END_M(start_h,start_m,duration_h,duration_m) ( (start_m+duration_m)%60 )
#define DURATION(slot) ((int) mOutputAsFloat(slot))

#define AquariumLightStartHours() AQUARIUM_LIGHT_DEFAULT_START_HOURS
#define AquariumLightEndHours() END_H(AQUARIUM_LIGHT_DEFAULT_START_HOURS, 0, DURATION(LYTB1_AQUARIUM_LIGHT_DURATION), 0)
#define AquariumLightAutoOn() RemoteInput(RS485_ADDRESS_ROW2B4, ROW2B4_LIGHT_AQUARIUM, Souliss_T1n_AutoCmd + AUTO_ON_STEPS)


inline void AquariumLightScheduler()
{
    grhSendUDPMessage( (String("Aquarium: start: ") + String(AquariumLightStartHours()) + String(":00 stop: ") 
                        + String(AquariumLightEndHours()) + String(":00")).c_str());

    if(IsCurrentTimeInRange(AquariumLightStartHours(), 0, AquariumLightEndHours(), 0))
    {
        grhSendUDPMessage("AquariumLightAutoOn command");
        AquariumLightAutoOn();
    }        
}




#define WateringZone1StartHours() WATERING_ZONE1_DEFAULT_START_HOURS
#define WateringZone1StartMin() WATERING_ZONE1_DEFAULT_START_MIN
#define WateringZone1EndHours() END_H(WATERING_ZONE1_DEFAULT_START_HOURS, WATERING_ZONE1_DEFAULT_START_MIN, 0, DURATION(LYTB1_WATERING_ZONE1_DURATION))
#define WateringZone1EndMin()  END_M(WATERING_ZONE1_DEFAULT_START_HOURS, WATERING_ZONE1_DEFAULT_START_MIN, 0, DURATION(LYTB1_WATERING_ZONE1_DURATION))
#define WateringZone1AutoOn() RemoteInput(IP_ADDRESS_GARDB1, GARDB1_WATERING_ZONE1, Souliss_T1n_AutoCmd + AUTO_ON_STEPS)

inline void WateringZone1Scheduler()
{
    grhSendUDPMessage( (String("Watering Zone1: start: ") + 
                        String(WateringZone1StartHours()) + String(":") + String(WateringZone1StartMin()) +
                        String(" stop: ") +
                        String(WateringZone1EndHours()) + String(":") + String(WateringZone1EndMin())).c_str());

    if( IsCurrentTimeInRange(WateringZone1StartHours(), WateringZone1StartMin(), WateringZone1EndHours(), WateringZone1EndMin()) )
    {
        grhSendUDPMessage("WateringZone1AutoOn command");
        WateringZone1AutoOn();        
    }
}



#define WateringZone2StartHours() WateringZone1EndHours()
#define WateringZone2StartMin() WateringZone1EndMin()
#define WateringZone2EndHours() END_H(WateringZone1EndHours(), WateringZone1EndMin(), 0, DURATION(LYTB1_WATERING_ZONE2_DURATION))
#define WateringZone2EndMin() END_M(WateringZone1EndHours(), WateringZone1EndMin(), 0, DURATION(LYTB1_WATERING_ZONE2_DURATION))
#define WateringZone2AutoOn() RemoteInput(IP_ADDRESS_GARDB1, GARDB1_WATERING_ZONE2, Souliss_T1n_AutoCmd + AUTO_ON_STEPS)

inline void WateringZone2Scheduler()
{
    grhSendUDPMessage( (String("Watering Zone2: start: ") + 
                        String(WateringZone2StartHours()) + String(":") + String(WateringZone2StartMin()) +
                        String(" stop: ") +
                        String(WateringZone2EndHours()) + String(":") + String(WateringZone2EndMin())).c_str());

    if( IsCurrentTimeInRange(WateringZone2StartHours(), WateringZone2StartMin(), WateringZone2EndHours(), WateringZone2EndMin()) )
    {
        grhSendUDPMessage("WateringZone2AutoOn command");
        WateringZone2AutoOn();        
    }
}




#define WateringZone3StartHours() WateringZone2EndHours()
#define WateringZone3StartMin() WateringZone2EndMin()
#define WateringZone3EndHours() END_H(WateringZone2EndHours(), WateringZone2EndMin(), 0, DURATION(LYTB1_WATERING_ZONE3_DURATION))
#define WateringZone3EndMin() END_M(WateringZone2EndHours(), WateringZone2EndMin(), 0, DURATION(LYTB1_WATERING_ZONE3_DURATION))
#define WateringZone3AutoOn() RemoteInput(IP_ADDRESS_GARDB1, GARDB1_WATERING_ZONE3, Souliss_T1n_AutoCmd + AUTO_ON_STEPS)

inline void WateringZone3Scheduler()
{
    grhSendUDPMessage( (String("Watering Zone3: start: ") + 
                        String(WateringZone3StartHours()) + String(":") + String(WateringZone3StartMin()) +
                        String(" stop: ") +
                        String(WateringZone3EndHours()) + String(":") + String(WateringZone3EndMin())).c_str());

    if( IsCurrentTimeInRange(WateringZone3StartHours(), WateringZone3StartMin(), WateringZone3EndHours(), WateringZone3EndMin()) )
    {
        grhSendUDPMessage("WateringZone3AutoOn command");
        WateringZone3AutoOn();        
    }
}



#define WateringZone4StartHours() WateringZone3EndHours()
#define WateringZone4StartMin() WateringZone3EndMin()
#define WateringZone4EndHours() END_H(WateringZone3EndHours(), WateringZone3EndMin(), 0, DURATION(LYTB1_WATERING_ZONE4_DURATION))
#define WateringZone4EndMin() END_M(WateringZone3EndHours(), WateringZone3EndMin(), 0, DURATION(LYTB1_WATERING_ZONE4_DURATION))
#define WateringZone4AutoOn() RemoteInput(IP_ADDRESS_GARDB1, GARDB1_WATERING_ZONE4, Souliss_T1n_AutoCmd + AUTO_ON_STEPS)

inline void WateringZone4Scheduler()
{
    grhSendUDPMessage( (String("Watering Zone4: start: ") + 
                        String(WateringZone4StartHours()) + String(":") + String(WateringZone4StartMin()) +
                        String(" stop: ") +
                        String(WateringZone4EndHours()) + String(":") + String(WateringZone4EndMin())).c_str());

    if( IsCurrentTimeInRange(WateringZone4StartHours(), WateringZone4StartMin(), WateringZone4EndHours(), WateringZone4EndMin()) )
    {
        grhSendUDPMessage("WateringZone4AutoOn command");
        WateringZone4AutoOn();        
    }
}



#define WateringZone5StartHours() WateringZone4EndHours()
#define WateringZone5StartMin() WateringZone4EndMin()
#define WateringZone5EndHours() END_H(WateringZone4EndHours(), WateringZone4EndMin(), 0, DURATION(LYTB1_WATERING_ZONE5_DURATION))
#define WateringZone5EndMin() END_M(WateringZone4EndHours(), WateringZone4EndMin(), 0, DURATION(LYTB1_WATERING_ZONE5_DURATION))
#define WateringZone5AutoOn() RemoteInput(IP_ADDRESS_GARDB1, GARDB1_WATERING_ZONE5, Souliss_T1n_AutoCmd + AUTO_ON_STEPS)

inline void WateringZone5Scheduler()
{
    grhSendUDPMessage( (String("Watering Zone5: start: ") + 
                        String(WateringZone5StartHours()) + String(":") + String(WateringZone5StartMin()) +
                        String(" stop: ") +
                        String(WateringZone5EndHours()) + String(":") + String(WateringZone5EndMin())).c_str());

    if( IsCurrentTimeInRange(WateringZone5StartHours(), WateringZone5StartMin(), WateringZone5EndHours(), WateringZone5EndMin()) )
    {
        grhSendUDPMessage("WateringZone5AutoOn command");
        WateringZone5AutoOn();        
    }
}



#define WateringZone6StartHours() WateringZone5EndHours()
#define WateringZone6StartMin() WateringZone5EndMin()
#define WateringZone6EndHours() END_H(WateringZone5EndHours(), WateringZone5EndMin(), 0, DURATION(LYTB1_WATERING_ZONE6_DURATION))
#define WateringZone6EndMin() END_M(WateringZone5EndHours(), WateringZone5EndMin(), 0, DURATION(LYTB1_WATERING_ZONE6_DURATION))
#define WateringZone6AutoOn() RemoteInput(RS485_ADDRESS_ROW2B4, ROW2B4_WATERING_ZONE6, Souliss_T1n_AutoCmd + AUTO_ON_STEPS)

inline void WateringZone6Scheduler()
{
    grhSendUDPMessage( (String("Watering Zone6: start: ") + 
                        String(WateringZone6StartHours()) + String(":") + String(WateringZone6StartMin()) +
                        String(" stop: ") +
                        String(WateringZone6EndHours()) + String(":") + String(WateringZone6EndMin())).c_str());

    if( IsCurrentTimeInRange(WateringZone6StartHours(), WateringZone6StartMin(), WateringZone6EndHours(), WateringZone6EndMin()) )
    {
        grhSendUDPMessage("WateringZone6AutoOn command");
        WateringZone6AutoOn();        
    }
}

#define SanitaryWaterStartHours() SANITARY_WATER_PRODUCTION_DEFAULT_START_HOURS
#define SanitaryWaterStartMin() SANITARY_WATER_PRODUCTION_DEFAULT_START_MIN
#define SanitaryWaterEndHours() END_H(SANITARY_WATER_PRODUCTION_DEFAULT_START_HOURS, SANITARY_WATER_PRODUCTION_DEFAULT_START_MIN, 0, DURATION(LYTB1_SANITARY_WATER_PRODUCTION_DURATION))
#define SanitaryWaterEndMin() END_M(SANITARY_WATER_PRODUCTION_DEFAULT_START_HOURS, SANITARY_WATER_PRODUCTION_DEFAULT_START_MIN, 0, DURATION(LYTB1_SANITARY_WATER_PRODUCTION_DURATION))
#define SanitaryWaterAutoOn() RemoteInput(IP_ADDRESS_LOFTB1, LOFTB1_HEATPUMP_SANITARY_WATER, Souliss_T1n_AutoCmd + AUTO_ON_STEPS)

inline void SanitaryWaterScheduler()
{

    grhSendUDPMessage( (String("Sanitary Water Production: start: ") + 
                        String(SanitaryWaterStartHours()) + String(":") + String(SanitaryWaterStartMin()) +
                        String(" stop: ") +
                        String(SanitaryWaterEndHours()) + String(":") + String(SanitaryWaterEndMin())).c_str());

    if( IsCurrentTimeInRange(SanitaryWaterStartHours(), SanitaryWaterStartMin(), SanitaryWaterEndHours(), SanitaryWaterEndMin()) )
    {
        grhSendUDPMessage("SanitaryWaterAutoOn command");
        SanitaryWaterAutoOn();        
    }
}



#define SCHEDULER_BEGIN         timeClient.begin()
#define SCHEDULER_TIME_UPDATE   timeClient.update()
