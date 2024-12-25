#ifndef WINPOWER_3FS_h
#define WINPOWER_3FS_h

#include <Arduino.h>

struct Data3FS
{
    uint16_t oxygenConcentration;
    uint8_t gear;
    uint8_t mode;
    uint8_t flowRate;
    uint8_t temperature;
    uint8_t respiratorySign;
    uint8_t respiratoryRate;
    uint8_t abc;
    uint8_t sensivity;
};

class Winpower_3FS
{
private:
    struct Data3FS data;
    uint8_t rawData[14];

    HardwareSerial *_serial;

    uint8_t crc(uint8_t *data, uint8_t len);
    void sendData(uint8_t *data, uint8_t len);

public:
    Winpower_3FS();

    /**
     * @brief   if pin is negative, it won't be set/changed and will be kept as is
     *
     * @param   o2serial Pointer to used Serial
     *
     * @return
     */
    Winpower_3FS(HardwareSerial *o2serial);

    /**
     * @brief   if pin is negative, it won't be set/changed and will be kept as is
     *
     * @param   o2serial Pointer to used Serial
     * @param   RX UART RX port number for ESP32
     * @param   TX UART TX port number for ESP32
     *
     * @return
     */
    Winpower_3FS(HardwareSerial *o2serial, int8_t RX, int8_t TX);

    ~Winpower_3FS(); //basic destructor

    /**
     * @brief   if pin is negative, it won't be set/changed and will be kept as is
     *
     * @param   o2serial Pointer to used Serial
     * @param   RX UART RX port number for ESP32
     * @param   TX UART TX port number for ESP32
     *
     * @return
     */
    void init(HardwareSerial *o2serial, int8_t RX = -1, int8_t TX = -1);

    /**
     * @brief   Set operation mode
     *
     * @param   gasSupplyGear 1..6 – The gas supply gear setting will automativally supply gas according to the single gas supply volime in the table.
     * @param   gasSupplyMode 0..3 – Off: 0, Pulse: 1, DirectSupply: 2, Fixed pulse: 3.
     * @param   ABC1 4..40 – The frequency of gas supply per minute in Fixed mode.
     * @param   sensivity 2..12 – Sensivity of breath detection.
     *
     * @return
     */
    void sendCommand(uint8_t gasSupplyGear, uint8_t gasSupplyMode, uint8_t ABC1, uint8_t sensivity);

    /**
     * @brief   if pin is negative, it won't be set/changed and will be kept as is
     *
     * @param   wait Wait buffer fullifies,
     *
     * @return
     */
    uint8_t updateData(bool wait = true);
    
    Data3FS getData();

    
    uint16_t getOxygenConcentration();
    uint8_t getGear();
    uint8_t getMode();
    uint8_t getFlowRate();
    uint8_t getTemperature();
    uint8_t getRespiratorySign();
    uint8_t getRespiratoryRate();
    uint8_t getAbc();
    uint8_t getSensivity();

};

#endif
