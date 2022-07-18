#include <WAEVController.h>

//void WAEVCONTROLLER::init(Stream &serial, Stream &serial2)
void WAEVCONTROLLER::init(Stream &serial)
{
	_serial = &serial;
	//_serial2 = &serial2;
	//_serial->println("Initialize ok");
}

void WAEVCONTROLLER::clear()
{
	while(_serial->available())
	{
		c = _serial->read();
	}
}

bool WAEVCONTROLLER::getVoltage(String &voltage)
{
	_return = false;
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x30;
	_command_send[1] = 0x32;
	_command_send[2] = 0x36;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	uint8_t i = 0;
	bool startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			//_serial2->print(_buffer[i], HEX);	_serial2->print(" ");
			
			if(_buffer[i] == 0xBC && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xBC && _buffer[3] == 0x00)
		{	
			_result = _buffer[1]<<8 | _buffer[2];

			_voltage = (0.274 *_result) + 29.29;
			voltage = String(_voltage);
			_return = true;
		}		
	}
	
    return _return;
}

bool WAEVCONTROLLER::getCurrent(String &current)
{
	_return = false;
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x3B;
	_command_send[1] = 0x32;
	_command_send[2] = 0x30;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	uint8_t i = 0;
	bool startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xB9 && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xB9 && _buffer[3] == 0x00)
		{	
			_result = _buffer[1]<<8 | _buffer[2];

			_current = (_result/2.14) - 0.434;
			current = String(_current);
			_return = true;
		}		
	}
	
    return _return;
}

bool WAEVCONTROLLER::getCoil20000(String &value)
{
	_return = false;
	memset(_buffer, 0, sizeof(_buffer));

	_command_send[0] = 0x31;
	_command_send[1] = 0x33;
	_command_send[2] = 0x31;
	_command_send[3] = 0x00;	
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	uint8_t i = 0;
	bool startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xB3 && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xB3 && _buffer[1] == 0x32 && _buffer[3] == 0x00)
		{	
			if(_buffer[2] == 0x30)
				value = "OFF";
			else if(_buffer[2] == 0x31)
				value = "ON";
			
			_return = true;
		}		
	}
	
    return _return;
}

bool WAEVCONTROLLER::setCoil20000(String value)
{
	_return = false;
	memset(_buffer, 0, sizeof(_buffer));
	
	if(value == "ON" || value == "OFF")
	{	
		if(value == "ON")
		{
			_command_send[0] = 0x31;
			_command_send[1] = 0x31;
			_command_send[2] = 0x31;
			_command_send[3] = 0x00;
		}	
		else if(value == "OFF")
		{
			_command_send[0] = 0x31;
			_command_send[1] = 0x31;
			_command_send[2] = 0x30;
			_command_send[3] = 0x00;
		}
	
		clear();
		_serial->write(_command_send,4);
	
		_status = true;
		_readOk = true;
		_lastTime = millis();
	
		uint8_t i = 0;
		bool startbyte = false;
		while(_status&&_readOk)
		{
			if(_serial->available())
			{
				_buffer[i] = _serial->read();
			
				if(_buffer[i] == 0x06 && startbyte == false)
				{
					_buffer[0] = _buffer[i];
					startbyte = true;
					i = 0;
				} 
			
				if(i == 1)
				{
					_readOk = false;
				
				}
		    
				i++;
			}
			if((millis() - _lastTime) > 500)
			{
				_status = false;
			}
		}
	
		if(_readOk == false)
		{	
			if(_buffer[0] == 0x06 && _buffer[2] == 0x00)
			{	
				_return = true;
			}		
		}
	}
	
    return _return;
}

bool WAEVCONTROLLER::setEn(uint8_t en)
{
	_return = false;
	memset(_buffer, 0, sizeof(_buffer));
	
	if(en == 0x00 || en == 0x03)
	{	
		if(en == 0x00)
		{
			_command_send[0] = 0x31;
			_command_send[1] = 0x30;
			_command_send[2] = 0x30;
			_command_send[3] = 0x00;
		}	
		else if(en == 0x03)
		{
			_command_send[0] = 0x31;
			_command_send[1] = 0x30;
			_command_send[2] = 0x31;
			_command_send[3] = 0x00;
		}
	
		clear();
		_serial->write(_command_send,4);
	
		_status = true;
		_readOk = true;
		_lastTime = millis();
	
		uint8_t i = 0;
		bool startbyte = false;
		while(_status&&_readOk)
		{
			if(_serial->available())
			{
				_buffer[i] = _serial->read();
			
				if(_buffer[i] == 0x06 && startbyte == false)
				{
					_buffer[0] = _buffer[i];
					startbyte = true;
					i = 0;
				} 
			
				if(i == 1)
				{
					_readOk = false;
				
				}
		    
				i++;
			}
			if((millis() - _lastTime) > 500)
			{
				_status = false;
			}
		}
	
		if(_readOk == false)
		{	
			if(_buffer[0] == 0x06 && _buffer[2] == 0x00)
			{	
				_return = true;
			}		
		}
	}
	
    return _return;
}
	
bool WAEVCONTROLLER::getEn(uint8_t &en)
{
	_return = false;
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x31;
	_command_send[1] = 0x32;
	_command_send[2] = 0x30;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	uint8_t i = 0;
	bool startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xB3 && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xB3 && _buffer[1] == 0x30 && _buffer[3] == 0x00)
		{	
			if(_buffer[2] == 0x30)
				en = 0;
			else if(_buffer[2] == 0x31)
				en = 3;
			
			_return = true;
		}		
	}
	
    return _return;
}

bool WAEVCONTROLLER::getChargingCurrentViaCP(uint8_t &charginCurrentViaCP)
{
	_return = false;
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x3A;
	_command_send[1] = 0x32;
	_command_send[2] = 0x31;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	uint8_t i = 0;
	bool startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xB2 && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xB2 && _buffer[1] == 0x32 && _buffer[3] == 0x00)
		{	
			charginCurrentViaCP = _buffer[2];
			_return = true;
		}		
	}
	
    return _return;
}

bool WAEVCONTROLLER::getSettingMaxCurrentPermissible(uint8_t &maxCurrentPermissible)
{
	_return = false;
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x3A;
	_command_send[1] = 0x32;
	_command_send[2] = 0x30;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	uint8_t i = 0;
	bool startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xB2 && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xB2 && _buffer[1] == 0x31 && _buffer[3] == 0x00)
		{	
			maxCurrentPermissible = _buffer[2];
			_return = true;
		}		
	}
	
    return _return;
}

bool WAEVCONTROLLER::setSettingMaxCurrentPermissible(uint8_t maxCurrentPermissible)
{
	_return = false;
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x3A;
	_command_send[1] = 0x30;
	_command_send[2] = maxCurrentPermissible;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	uint8_t i = 0;
	bool startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0x06 && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 1)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0x06 && _buffer[2] == 0x00)
		{	
			_return = true;
		}		
	}
	
    return _return;
}

bool WAEVCONTROLLER::getVehicleStatus(String &vehicleStatus)
{
	_return = false;
	
	uint8_t i = 0;
	bool startbyte = false;
	memset(_buffer, 0, sizeof(_buffer));
	_readOk = true;
	while(_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			if(_buffer[i] == 0xA0 && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
		
			if(i == 7)
			{
				if(_buffer[0] == 0xA0 && _buffer[1] == 0x31 && _buffer[3] == 0x00)
				{	
					switch(_buffer[2])
					{
						case 0x30 :
							vehicleStatus = "NA";
							break;
						case 0x31 :
							vehicleStatus = "A1";
							break;
						case 0x32 :
							vehicleStatus = "A2";
							break;
						case 0x33 :
							vehicleStatus = "B1";
							break;
						case 0x34 :
							vehicleStatus = "B2";
							break;
						case 0x35 :
							vehicleStatus = "C1";
							break;
						case 0x36 :
							vehicleStatus = "C2";
							break;
						case 0x37 :
							vehicleStatus = "D1";
							break;
						case 0x38 :
							vehicleStatus = "D2";
							break;
						case 0x39 :
							vehicleStatus = "E";
							break;
						case 0x3A :
							vehicleStatus = "F";
							break;
					}
					_return = true;
				}

				clear();
				
				_readOk = false;
				_return = true;
			}
			
			i++;
		}
		else{
			clear();
			_readOk = false;
			_return = false;
		}
	}

	if(_return == false)
	{
		memset(_buffer, 0, sizeof(_buffer));
		_command_send[0] = 0x33;
		_command_send[1] = 0x32;
		_command_send[2] = 0x31;
		_command_send[3] = 0x00;
	
		clear();
		_serial->write(_command_send,4);
	
		_status = true;
		_readOk = true;
		_lastTime = millis();
	
		i = 0;
		startbyte = false;
		while(_status&&_readOk)
		{
			if(_serial->available())
			{
				_buffer[i] = _serial->read();
			
				if(_buffer[i] == 0xB1 && startbyte == false)
				{
					_buffer[0] = _buffer[i];
					startbyte = true;
					i = 0;
				} 
			
				if(i == 3)
				{
					_readOk = false;
				
				}
		    
				i++;
			}
			if((millis() - _lastTime) > 500)
			{
				_status = false;
			}
		}
	
		if(_readOk == false)
		{	
			if(_buffer[0] == 0xB1 && _buffer[1] == 0x30 && _buffer[3] == 0x00)
			{	
				switch(_buffer[2])
				{
					case 0x30 :
						vehicleStatus = "NA";
						break;
					case 0x31 :
						vehicleStatus = "A1";
						break;
					case 0x32 :
						vehicleStatus = "A2";
						break;
					case 0x33 :
						vehicleStatus = "B1";
						break;
					case 0x34 :
						vehicleStatus = "B2";
						break;
					case 0x35 :
						vehicleStatus = "C1";
						break;
					case 0x36 :
						vehicleStatus = "C2";
						break;
					case 0x37 :
						vehicleStatus = "D1";
						break;
					case 0x38 :
						vehicleStatus = "D2";
						break;
					case 0x39 :
						vehicleStatus = "E";
						break;
					case 0x3A :
						vehicleStatus = "F";
						break;
				}
				_return = true;
			}		
		}
	}

    return _return;
	
}

bool WAEVCONTROLLER::getErrorStatus(uint8_t &errorStatus)
{
	_return = false;
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x35;
	_command_send[1] = 0x32;
	_command_send[2] = 0x31;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	uint8_t i = 0;
	bool startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xB0 && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xB0 && _buffer[1] == 0x30 && _buffer[3] == 0x00)
		{	
			errorStatus = _buffer[2];			
			_return = true;
		}		
	}
	
    return _return;
}

bool WAEVCONTROLLER::getAllStatus(String &vehicleStatus_AllStatus, uint8_t &charginCurrentCP_AllStatus, uint8_t &statusInputs, uint8_t &statusOutputs, uint16_t &timeConnectHH , uint8_t &timeConnectMM, uint8_t &timeConnectSS
	,uint16_t &timeChargingHH , uint8_t &timeChargingMM, uint8_t &timeChargingSS, uint8_t &errorStatus_AllStatus)
{ 
	_return = false;
	
	if(getVehicleStatus(vehicleStatus_AllStatus))
	{
		_return |= true;
		
		if(vehicleStatus_AllStatus == "A1" || vehicleStatus_AllStatus == "A2")
		{
			_secondsConnect = 0;
			_minutsConnect = 0;
			_hoursConnect = 0;
			_firstCountConnect = false;
			_secondsCharging = 0;
			_minutsCharging = 0;
			_hoursCharging = 0;
			_firstCountCharging = false;
		}
		
		if(vehicleStatus_AllStatus == "B1" || vehicleStatus_AllStatus == "B2" || vehicleStatus_AllStatus == "C1" || vehicleStatus_AllStatus == "C2")
		{
			if(vehicleStatus_AllStatus == "B1" || vehicleStatus_AllStatus == "B2" )
			{
				_firstCountCharging = false;
			}
			
			if(_firstCountConnect == false)
			{
				_firstCountConnect = true;
				_lastTimeSecondsConnect = millis();
				_lastTimeMinutesConnect = _lastTimeSecondsConnect;
				_secondsConnect = (millis() - _lastTimeSecondsConnect)/1000;
			}
			else if(_firstCountConnect == true)
			{
				_secondsConnect = (millis() - _lastTimeSecondsConnect)/1000;
				_minutsConnect = ((millis() - _lastTimeMinutesConnect)/1000)/60;
				
				if(_secondsConnect >= 60)
				{
					_secondsConnect = 0;
					_lastTimeSecondsConnect = millis();
				}
				
				if(_minutsConnect >= 60)
				{
					_hoursConnect += 1;
					_minutsConnect = 0;
					_lastTimeMinutesConnect = millis();
				}				
			}
			// debug
			/*
			if(vehicleStatus_AllStatus == "C1" || vehicleStatus_AllStatus == "C2")
			{
			
				
				if(_secondsCharging >= 60)
				{
					_secondsCharging = 0;
					_minutsCharging += 1;
					_lastTimeSecondsCharging = millis();
				}
				
				if(_minutsCharging >= 60)
				{
					_hoursCharging += 1;
					_minutsCharging = 0;
					_lastTimeMinutesCharging = millis();
				}			
			}
			*/
		}
		
		if(vehicleStatus_AllStatus == "C1" || vehicleStatus_AllStatus == "C2")
		{
			if(_firstCountCharging == false)
			{
				_firstCountCharging = true;
				//_lastTimeSecondsCharging = millis();
				//_lastTimeMinutesCharging = _lastTimeSecondsCharging;
				if(_secondsCharging == 0)
				{
					_lastTimeSecondsCharging = millis();
					//_lastTimeMinutesCharging = _lastTimeSecondsCharging;
				}
				else
				{
					_lastTimeSecondsCharging = (millis() - (_secondsCharging * 1000));
				}
				
				if(_minutsCharging == 0)
				{
					_lastTimeMinutesCharging = millis();
				}
				else
				{
					_lastTimeMinutesCharging = (millis() - (_minutsCharging*60)*1000);
				}
			}
			else if(_firstCountCharging == true)
			{
				_secondsCharging = (millis() - _lastTimeSecondsCharging)/1000;
				_minutsCharging  = ((millis() - _lastTimeMinutesCharging)/1000)/60;
			
				if(_secondsCharging >= 60)
				{
					_secondsCharging = 0;
					_lastTimeSecondsCharging = millis();
				}
			
				if(_minutsCharging >= 60)
				{
					_hoursCharging += 1;
					_minutsCharging = 0;
					_lastTimeMinutesCharging = millis();
				}				
			}
		}	
	}
	
	if(getChargingCurrentViaCP(charginCurrentCP_AllStatus))
	{
		_return |= true;
	}
	
	if(getErrorStatus(errorStatus_AllStatus))
	{
		_return |= true;
	}
	
	timeConnectSS = _secondsConnect;
	timeConnectMM = _minutsConnect;
	timeConnectHH = _hoursConnect;
	
	timeChargingSS = _secondsCharging;
	timeChargingMM = _minutsCharging;
	timeChargingHH = _hoursCharging;
	
	return _return;
}

bool WAEVCONTROLLER::restartEVController()
{
	_return = false;
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x36;
	_command_send[1] = 0x31;
	_command_send[2] = 0x31;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	uint8_t i = 0;
	bool startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0x06 && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 1)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0x06 && _buffer[3] == 0x00)
	    {
			_return = true;
		}		
	}
	
    return _return;
}

bool WAEVCONTROLLER::setPhaseMeter(uint8_t phase)
{
	_return = false;
	
	if(phase == 0x01 || phase == 0x03)
	{
		memset(_buffer, 0, sizeof(_buffer));
		_command_send[0] = 0x3A;
		_command_send[1] = 0x33;
		
		if(phase == 0x01)
			_command_send[2] = 0x30;
		else if(phase == 0x03)
			_command_send[2] = 0x31;
		
		_command_send[3] = 0x00;
	
		clear();
		_serial->write(_command_send,4);
	
		_status = true;
		_readOk = true;
		_lastTime = millis();
	
		uint8_t i = 0;
		bool startbyte = false;
		while(_status&&_readOk)
		{
			if(_serial->available())
			{
				_buffer[i] = _serial->read();
			
				if(_buffer[i] == 0x06 && startbyte == false)
				{
					_buffer[0] = _buffer[i];
					startbyte = true;
					i = 0;
				} 
			
				if(i == 1)
				{
					_readOk = false;
				
				}
		    
				i++;
			}
			if((millis() - _lastTime) > 500)
			{
				_status = false;
			}
		}
	
		if(_readOk == false)
		{	
			if(_buffer[0] == 0x06 && _buffer[3] == 0x00)
			{
				_return = true;
			}		
		}
	}
	
    return _return;
}

bool WAEVCONTROLLER::readParameter1Phase(String &voltageA, String &currentA, String &powerA)
{
	_return = false;
	// Voltage
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x30;
	_command_send[1] = 0x32;
	_command_send[2] = 0x36;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	uint8_t i = 0;
	bool startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xBC && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xBC && _buffer[3] == 0x00)
		{	
			if(_buffer[1] != 0x00 && _buffer[2] != 0x00)
			{
				_valueVoltage = _buffer[1]<<8 | _buffer[2];
				_voltage = (0.274 *_valueVoltage) + 29.29;
			
			}
			else
			{
				_voltage = 0;
			}
			
			voltageA =  String(_voltage);
			
			_return |= true;
		}		
	}
	
	// Current A
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x3B;
	_command_send[1] = 0x32;
	_command_send[2] = 0x30;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	i = 0;
	startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xB9 && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xB9 && _buffer[3] == 0x00)
		{				
			_valueCurrentA =  _buffer[1]<<8 | _buffer[2];
			if(_buffer[1] == 0x00 && _buffer[2] <= 0x02)
			{
				_currentA = 0;
				currentA = "0";		
			}
			else
			{
				_currentA = (_valueCurrentA/2.14) - 0.434;
				currentA = String(_currentA);
			}
			
			_return |= true;
		}		
	}
	
	powerA = String(_currentA * _voltage);
	
    return _return;
}

bool WAEVCONTROLLER::readParameter1Phase(String &voltageA, String &currentA, String &powerA, String &energy)
{
	_return = false;
	// Voltage
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x30;
	_command_send[1] = 0x32;
	_command_send[2] = 0x36;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	uint8_t i = 0;
	bool startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xBC && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xBC && _buffer[3] == 0x00)
		{	
			if(_buffer[1] != 0x00 && _buffer[2] != 0x00)
			{
				_valueVoltage = _buffer[1]<<8 | _buffer[2];
				_voltage = (0.274 *_valueVoltage) + 29.29;
			
			}
			else
			{
				_voltage = 0;
			}
			
			voltageA =  String(_voltage);
			
			_return |= true;
		}		
	}
	
	// Current A
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x3B;
	_command_send[1] = 0x32;
	_command_send[2] = 0x30;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	i = 0;
	startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xB9 && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xB9 && _buffer[3] == 0x00)
		{				
			_valueCurrentA =  _buffer[1]<<8 | _buffer[2];
			if(_buffer[1] == 0x00 && _buffer[2] <= 0x02)
			{
				_currentA = 0;
				currentA = "0";		
			}
			else
			{
				_currentA = (_valueCurrentA/2.14) - 0.434;
				currentA = String(_currentA);
			}
			
			_return |= true;
		}		
	}
	
	powerA = String(_currentA * _voltage);
	_timeEnergy = ((millis() - _lastTimeEnergy)/1000.00);
	_lastTimeEnergy = millis();
	//_currentA = 100;
	_energy += ((_currentA * _voltage)*(_timeEnergy/3600.00))/1000;
	
	energy = String(_energy,7);
	
    return _return;
}

bool WAEVCONTROLLER::readParameter3Phase(String &voltageA, String &voltageB, String &voltageC, String &currentA, String &currentB, String &currentC, String &powerA, String &powerB, String &powerC)
{
	_return = false;
	// Voltage
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x30;
	_command_send[1] = 0x32;
	_command_send[2] = 0x36;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	uint8_t i = 0;
	bool startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xBC && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xBC && _buffer[3] == 0x00)
		{	
			if(_buffer[1] != 0x00 && _buffer[2] != 0x00)
			{
				_valueVoltage = _buffer[1]<<8 | _buffer[2];
				_voltage = (0.274 *_valueVoltage) + 29.29;		
			}
			else
			{
				_voltage = 0;
			}
			
			voltageA =  String(_voltage);
			voltageB = voltageA;
			voltageC = voltageA;
			
			_return |= true;
		}		
	}
	
	// Current A
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x3B;
	_command_send[1] = 0x32;
	_command_send[2] = 0x30;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	i = 0;
	startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xB9 && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xB9 && _buffer[3] == 0x00)
		{				
			_valueCurrentA =  _buffer[1]<<8 | _buffer[2];
			if(_buffer[1] == 0x00 && _buffer[2] <= 0x02)
			{
				_currentA = 0;
				currentA = "0";			
			}
			else
			{
				_currentA = (_valueCurrentA/2.14) - 0.434;
				currentA = String(_currentA);
			}
			
			_return |= true;
		}		
	}
	
	// Current B
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x3B;
	_command_send[1] = 0x32;
	_command_send[2] = 0x31;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	i = 0;
	startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xBA && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xBA && _buffer[3] == 0x00)
		{				
			_valueCurrentB =  _buffer[1]<<8 | _buffer[2];
			if(_buffer[1] == 0x00 && _buffer[2] <= 0x02)
			{
				_currentB = 0;
				currentB = "0";				
			}
			else
			{
				_currentB = (_valueCurrentB/2.14) - 0.434;
				currentB = String(_currentB);
			}
			
			_return |= true;
		}		
	}
	
	// Current C
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x3B;
	_command_send[1] = 0x32;
	_command_send[2] = 0x32;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	i = 0;
	startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xBB && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xBB && _buffer[3] == 0x00)
		{				
			_valueCurrentC =  _buffer[1]<<8 | _buffer[2];
			if(_buffer[1] == 0x00 && _buffer[2] <= 0x02)
			{
				_currentC = 0;
				currentC = "0";				
			}
			else
			{
				_currentC = (_valueCurrentC/2.14) - 0.434;
				currentC = String(_currentC);
			}
			
			_return |= true;
		}		
	}
	
	powerA = String(_currentA * _voltage);
	powerB = String(_currentB * _voltage);
	powerC = String(_currentC * _voltage);
	
    return _return;
}

bool WAEVCONTROLLER::readParameter3Phase(String &voltageA, String &voltageB, String &voltageC, String &currentA, String &currentB, String &currentC, String &powerA, String &powerB, String &powerC, String &energy)
{
	_return = false;
	// Voltage
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x30;
	_command_send[1] = 0x32;
	_command_send[2] = 0x36;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	uint8_t i = 0;
	bool startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xBC && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xBC && _buffer[3] == 0x00)
		{	
			if(_buffer[1] != 0x00 && _buffer[2] != 0x00)
			{
				_valueVoltage = _buffer[1]<<8 | _buffer[2];
				_voltage = (0.274 *_valueVoltage) + 29.29;		
			}
			else
			{
				_voltage = 0;
			}
			
			voltageA =  String(_voltage);
			voltageB = voltageA;
			voltageC = voltageA;
			
			_return |= true;
		}		
	}
	
	// Current A
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x3B;
	_command_send[1] = 0x32;
	_command_send[2] = 0x30;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	i = 0;
	startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xB9 && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xB9 && _buffer[3] == 0x00)
		{				
			_valueCurrentA =  _buffer[1]<<8 | _buffer[2];
			if(_buffer[1] == 0x00 && _buffer[2] <= 0x02)
			{
				_currentA = 0;
				currentA = "0";			
			}
			else
			{
				_currentA = (_valueCurrentA/2.14) - 0.434;
				currentA = String(_currentA);
			}
			
			_return |= true;
		}		
	}
	
	// Current B
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x3B;
	_command_send[1] = 0x32;
	_command_send[2] = 0x31;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	i = 0;
	startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xBA && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xBA && _buffer[3] == 0x00)
		{				
			_valueCurrentB =  _buffer[1]<<8 | _buffer[2];
			if(_buffer[1] == 0x00 && _buffer[2] <= 0x02)
			{
				_currentB = 0;
				currentB = "0";				
			}
			else
			{
				_currentB = (_valueCurrentB/2.14) - 0.434;
				currentB = String(_currentB);
			}
			
			_return |= true;
		}		
	}
	
	// Current C
	memset(_buffer, 0, sizeof(_buffer));
	_command_send[0] = 0x3B;
	_command_send[1] = 0x32;
	_command_send[2] = 0x32;
	_command_send[3] = 0x00;
	
	clear();
	_serial->write(_command_send,4);
	
	_status = true;
	_readOk = true;
	_lastTime = millis();
	
	i = 0;
	startbyte = false;
	while(_status&&_readOk)
	{
		if(_serial->available())
		{
			_buffer[i] = _serial->read();
			
			if(_buffer[i] == 0xBB && startbyte == false)
			{
				_buffer[0] = _buffer[i];
				startbyte = true;
				i = 0;
			} 
			
			if(i == 3)
			{
				_readOk = false;
				
			}
		    
			i++;
		}
		 if((millis() - _lastTime) > 500)
		{
			_status = false;
		}
	}
	
	if(_readOk == false)
	{	
		if(_buffer[0] == 0xBB && _buffer[3] == 0x00)
		{				
			_valueCurrentC =  _buffer[1]<<8 | _buffer[2];
			if(_buffer[1] == 0x00 && _buffer[2] <= 0x02)
			{
				_currentC = 0;
				currentC = "0";				
			}
			else
			{
				_currentC = (_valueCurrentC/2.14) - 0.434;
				currentC = String(_currentC);
			}
			
			_return |= true;
		}		
	}
	
	powerA = String(_currentA * _voltage);
	powerB = String(_currentB * _voltage);
	powerC = String(_currentC * _voltage);
	
	_timeEnergy = ((millis() - _lastTimeEnergy)/1000.00);
	_lastTimeEnergy = millis();
	
	_energy += ((_currentA * _voltage)*(_timeEnergy/3600.00))/1000;
	_energy += ((_currentB * _voltage)*(_timeEnergy/3600.00))/1000;
	_energy += ((_currentC * _voltage)*(_timeEnergy/3600.00))/1000;
	
	energy = String(_energy,7);
	
    return _return;
}