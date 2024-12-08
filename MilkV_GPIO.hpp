/*
 * MilkV_GPIO
 * Author: Arkadiusz Szlanta
 * Date: 08 Dec 2024
 *
 * License:
 * This source code is provided for hobbyist and private use only.
 * Any commercial or industrial use, including distribution, reproduction, or
 * incorporation in commercial or industrial products or services is prohibited.
 * Use at your own risk. The author(s) hold no responsibility for any damages
 * or losses resulting from the use of this software.
 *
 */

#ifndef MILKV_GPIO_HPP
#define MILKV_GPIO_HPP

#include <stdint.h>

class MilkV_GPIO
{
public:
    enum GPIOPort
    {
        GPIOPort0 = 0,
        GPIOPort1,
        GPIOPort2,
        GPIOPort3,
        PowerGPIO
    };

    enum GPIODirection
    {
        GPIOInput = 0,
        GPIOOutput
    };

    enum IntLevelType
    {
        LevelSensitive = 0,
        EdgeSensitive
    };

    enum IntPolarity
    {
        ActiveLow = 0,
        ActiveHigh
    };

    MilkV_GPIO(GPIOPort port, uint32_t pinNumber);
    ~MilkV_GPIO(void);

    void init(GPIODirection pinDirection);

    void writePin(bool pinState);
    uint32_t readPin(void);

    void enableInterrupt(void);
    void disableInterrupt(void);

    void enableInterruptMask(void);
    void disableInterruptMask(void);

    void enableDebounce(void);
    void disableDebounce(void);

    void setInterruptLevelType(IntLevelType levelType = EdgeSensitive);

    void setInterruptPolarity(IntPolarity polarity = ActiveHigh);

    uint32_t getIntStatus();
    uint32_t getRawIntStatus();

private:
    GPIOPort mPort;
    uint32_t mPinNumber;
};

#endif // MILKV_GPIO_HPP
