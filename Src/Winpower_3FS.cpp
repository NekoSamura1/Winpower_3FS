#include "Winpower_3FS.h"

uint8_t Winpower_3FS::crc(uint8_t *data, uint8_t len)
{
    uint8_t result = 0;
    for (uint_fast8_t i = 0; i < len; i++)
    {
        result -= data[i];
    }
    return result;
}

void Winpower_3FS::sendData(uint8_t *data, uint8_t len)
{
    for (uint_fast8_t i = 0; i < len; i++)
    {
        _serial->write(data[i]);
    }
    _serial->write(crc(data, len));
}

Winpower_3FS::Winpower_3FS(/* no args */)
{
}

Winpower_3FS::Winpower_3FS(HardwareSerial *o2serial)
{
    init(o2serial);
}

Winpower_3FS::Winpower_3FS(HardwareSerial *o2serial, int8_t RX, int8_t TX)
{
    init(o2serial, RX, TX);
}

Winpower_3FS::~Winpower_3FS()
{
}

void Winpower_3FS::init(HardwareSerial *o2serial, int8_t RX, int8_t TX)
{
    _serial = o2serial;
#ifdef ESP32
    _serial->begin(115200, SERIAL_8N1, RX, TX);
#else
    _serial->begin(115200, SERIAL_8N1);
#endif
}

void Winpower_3FS::sendCommand(uint8_t gasSupplyGear, uint8_t gasSupplyMode, uint8_t ABC1, uint8_t sensivity)
{
    uint8_t command[] = {0x16, 0x09, 0x01, 0x00, 0x00, 0x00, 0x00};
    command[3] = gasSupplyGear;
    command[4] = gasSupplyMode;
    command[5] = ABC1;
    command[6] = sensivity;
    sendData(command, sizeof(command));
}

uint8_t Winpower_3FS::updateData(bool wait)
{
    if (wait or _serial->available() >= 14)
    {
        while (_serial->available() < 3 and wait) // wait until buffer fullifies
            delay(1);

        do // find a start of a packet
        {
            rawData[3] = _serial->read();
            for (size_t i = 0; i < 3; i++)
            {
                rawData[i] = rawData[i + 1];
            }
        } while (rawData[0] != 0x16 or rawData[1] != 0x09 or rawData[2] != 0x01);
        while (_serial->available() < 11 and wait) // wait until buffer fullifies
            ;
        _serial->readBytes(&rawData[3], 11);

        if (crc(rawData, 13) == rawData[13]) // Parse packet
        {
            data.oxygenConcentration = (uint16_t)rawData[3] << 8 | rawData[4];
            data.gear = rawData[5];
            data.mode = rawData[6];
            data.flowRate = rawData[7];
            data.temperature = rawData[8];
            data.respiratorySign = rawData[9];
            data.respiratoryRate = rawData[10];
            data.abc = rawData[11];
            data.sensivity = rawData[12];
        }
        else
        {
            _serial->flush(false); // Clear buffer to start from totaly new packet
            return 1;              // bad packet
        }
        _serial->flush(false);
        return 0; // good packet
    }
    else
    {
        return 2; // Not enough data
    }
}

Data3FS Winpower_3FS::getData()
{
    return data;
}

uint16_t Winpower_3FS::getOxygenConcentration()
{
    return data.oxygenConcentration;
}

uint8_t Winpower_3FS::getGear()
{
    return data.gear;
}

uint8_t Winpower_3FS::getMode()
{
    return data.mode;
}

uint8_t Winpower_3FS::getFlowRate()
{
    return data.flowRate;
}

uint8_t Winpower_3FS::getTemperature()
{
    return data.temperature;
}

uint8_t Winpower_3FS::getRespiratorySign()
{
    return data.respiratorySign;
}

uint8_t Winpower_3FS::getRespiratoryRate()
{
    return data.respiratoryRate;
}

uint8_t Winpower_3FS::getAbc()
{
    return data.abc;
}

uint8_t Winpower_3FS::getSensivity()
{
    return data.sensivity;
}
