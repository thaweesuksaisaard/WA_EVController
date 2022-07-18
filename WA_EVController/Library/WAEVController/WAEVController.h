#ifndef WAEVCONTROLLER_H
#define WAEVCONTROLLER_H
#include <Arduino.h>
class WAEVCONTROLLER{
  public:
	//void init(Stream &serial, Stream &serial2);
	void init(Stream &serial);
	bool getVoltage(String &voltage); 
	bool getCurrent(String &current);
	bool getEn(uint8_t &en);
	bool setEn(uint8_t en);
	bool setCoil20000(String value);
	bool getCoil20000(String &value);
	bool getVehicleStatus(String &vehicleStatus);
	bool setSettingMaxCurrentPermissible(uint8_t maxCurrentPermissible);
	bool getSettingMaxCurrentPermissible(uint8_t &maxCurrentPermissible);
	bool getChargingCurrentViaCP(uint8_t &charginCurrentViaCP);
	bool getErrorStatus(uint8_t &errorStatus);
	bool getAllStatus(String &vehicleStatus_AllStatus, uint8_t &charginCurrentViaCP_AllStatus, uint8_t &statusInputs, uint8_t &statusOutputs, uint16_t &timeConnectHH , uint8_t &timeConnectMM, uint8_t &timeConnectSS
			,uint16_t &timeChargingHH , uint8_t &timeChargingMM, uint8_t &timeChargingSS, uint8_t &errorStatus_AllStatus);
	bool restartEVController();
	bool setPhaseMeter(uint8_t phase);
	bool readParameter1Phase(String &voltageA, String &currentA, String &powerA);
	bool readParameter1Phase(String &voltageA, String &currentA, String &powerA, String &energy);
	bool readParameter3Phase(String &voltageA, String &voltageB, String &voltageC, String &currentA, String &currentB, String &currentC, String &powerA, String &powerB, String &powerC);
	bool readParameter3Phase(String &voltageA, String &voltageB, String &voltageC, String &currentA, String &currentB, String &currentC, String &powerA, String &powerB, String &powerC, String &energy);
  private:
	void clear();
	uint8_t _buffer[20];
	Stream* _serial;
	Stream* _serial2;
	uint8_t _command_send[4];
	unsigned long _lastTime;
	float _voltage;
	float _current;
	uint16_t _result;
	byte c;
	bool _status;
	bool _readOk;	
	bool _return;
	uint8_t _secondsConnect;
	uint8_t _minutsConnect;
	uint16_t _hoursConnect;
	bool _firstCountConnect = false;
	unsigned long _lastTimeSecondsConnect = 0;
	unsigned long _lastTimeMinutesConnect = 0;
	unsigned long _lastTimeEnergy = 0;
	float _timeEnergy = 0;
	uint8_t _secondsCharging;
	uint8_t _minutsCharging;
	uint16_t _hoursCharging;
	bool _firstCountCharging = false;
	unsigned long _lastTimeSecondsCharging = 0;
	unsigned long _lastTimeMinutesCharging = 0;
	uint8_t _countTime = 0;
	uint16_t _valueVoltage = 0;
	uint16_t _valueCurrentA = 0;
	uint16_t _valueCurrentB = 0;
	uint16_t _valueCurrentC = 0;
	float _currentA = 0;
	float _currentB = 0;
	float _currentC = 0;
	double _energy = 0;
};

#endif
