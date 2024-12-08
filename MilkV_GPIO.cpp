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

#include <MilkV_GPIO.hpp>

#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

struct GPIOReg
{
    uint32_t SWPORTA_DR;
    uint32_t SWPORTA_DDR;
    uint32_t reserved1;
    uint32_t reserved2;
    uint32_t reserved3;
    uint32_t reserved4;
    uint32_t reserved5;
    uint32_t reserved6;
    uint32_t reserved7;
    uint32_t reserved8;
    uint32_t reserved9;
    uint32_t reserved10;
    uint32_t INTEN;
    uint32_t INTMASK;
    uint32_t INTTYPE_LEVEL;
    uint32_t INT_POLARITY;
    uint32_t INT_STATUS;
    uint32_t RAW_INT_STATUS;
    uint32_t DEBOUNCE;
    uint32_t PORTA_EOI;
    uint32_t PORTA_EXT;
    uint32_t reserved11;
    uint32_t reserved12;
    uint32_t reserved13;
    uint32_t LS_SYNC;
};

#define GPIO0_BASE (0x03020000ul)
#define GPIO1_BASE (0x03021000ul)
#define GPIO2_BASE (0x03022000ul)
#define GPIO3_BASE (0x03023000ul)
#define PWR_GPIO_BASE (0x05021000ul)

#define GPIO0 (reinterpret_cast<GPIOReg *>(GPIO0_BASE))
#define GPIO1 (reinterpret_cast<GPIOReg *>(GPIO1_BASE))
#define GPIO2 (reinterpret_cast<GPIOReg *>(GPIO2_BASE))
#define GPIO3 (reinterpret_cast<GPIOReg *>(GPIO3_BASE))
#define PWR_GPIO (reinterpret_cast<GPIOReg *>(PWR_GPIO_BASE))

static std::vector<GPIOReg *> sGPIOList;

static void getGPIOAccess(void)
{
    int fdMem = open("/dev/mem", O_RDWR | O_SYNC);

    GPIOReg *gp0Reg = reinterpret_cast<GPIOReg *>(mmap(0, sizeof(GPIOReg), PROT_READ | PROT_WRITE, MAP_SHARED, fdMem, GPIO0_BASE));
    GPIOReg *gp1Reg = reinterpret_cast<GPIOReg *>(mmap(0, sizeof(GPIOReg), PROT_READ | PROT_WRITE, MAP_SHARED, fdMem, GPIO1_BASE));
    GPIOReg *gp2Reg = reinterpret_cast<GPIOReg *>(mmap(0, sizeof(GPIOReg), PROT_READ | PROT_WRITE, MAP_SHARED, fdMem, GPIO2_BASE));
    GPIOReg *gp3Reg = reinterpret_cast<GPIOReg *>(mmap(0, sizeof(GPIOReg), PROT_READ | PROT_WRITE, MAP_SHARED, fdMem, GPIO3_BASE));
    GPIOReg *pwrGpReg = reinterpret_cast<GPIOReg *>(mmap(0, sizeof(GPIOReg), PROT_READ | PROT_WRITE, MAP_SHARED, fdMem, PWR_GPIO_BASE));

    close(fdMem);

    sGPIOList.push_back(gp0Reg);
    sGPIOList.push_back(gp1Reg);
    sGPIOList.push_back(gp2Reg);
    sGPIOList.push_back(gp3Reg);
    sGPIOList.push_back(pwrGpReg);
}

MilkV_GPIO::MilkV_GPIO(GPIOPort port, uint32_t pinNumber)
{
    mPort = port;
    mPinNumber = pinNumber;

    getGPIOAccess();
}

MilkV_GPIO::~MilkV_GPIO(void)
{
    sGPIOList.clear();
    sGPIOList.~vector();
}

void MilkV_GPIO::init(GPIODirection pinDirection)
{
    switch (pinDirection)
    {
    case GPIOInput:
        sGPIOList[mPort]->SWPORTA_DDR &= ~(1u << mPinNumber);
        break;

    case GPIOOutput:
        sGPIOList[mPort]->SWPORTA_DDR |= (1u << mPinNumber);
        break;
    }
}

void MilkV_GPIO::writePin(bool pinState)
{
    switch (pinState)
    {
    case 0:
        sGPIOList[mPort]->SWPORTA_DR &= ~(1u << mPinNumber);
        break;

    case 1:
        sGPIOList[mPort]->SWPORTA_DR |= (1u << mPinNumber);
        break;
    }
}

uint32_t MilkV_GPIO::readPin(void)
{
    uint32_t pinState = ((sGPIOList[mPort]->SWPORTA_DR) & (1u << mPinNumber)) > 0u ? 1u : 0u;
    return pinState;
}

void MilkV_GPIO::enableInterrupt(void)
{
    sGPIOList[mPort]->INTEN |= (1u << mPinNumber);
}

void MilkV_GPIO::disableInterrupt(void)
{
    sGPIOList[mPort]->INTEN &= ~(1u << mPinNumber);
}

void MilkV_GPIO::enableInterruptMask(void)
{
    sGPIOList[mPort]->INTMASK |= (1u << mPinNumber);
}

void MilkV_GPIO::disableInterruptMask(void)
{
    sGPIOList[mPort]->INTMASK &= ~(1u << mPinNumber);
}

void MilkV_GPIO::enableDebounce(void)
{
    sGPIOList[mPort]->DEBOUNCE |= (1u << mPinNumber);
}

void MilkV_GPIO::disableDebounce(void)
{
    sGPIOList[mPort]->DEBOUNCE &= ~(1u << mPinNumber);
}

void MilkV_GPIO::setInterruptLevelType(IntLevelType levelType)
{
    switch (levelType)
    {
    case LevelSensitive:
        sGPIOList[mPort]->INTTYPE_LEVEL &= ~(1u << mPinNumber);
        break;

    case EdgeSensitive:
        sGPIOList[mPort]->INTTYPE_LEVEL |= (1u << mPinNumber);
        break;
    }
}

void MilkV_GPIO::setInterruptPolarity(IntPolarity polarity)
{
    switch (polarity)
    {
    case ActiveLow:
        sGPIOList[mPort]->INT_POLARITY &= ~(1u << mPinNumber);
        break;

    case ActiveHigh:
        sGPIOList[mPort]->INT_POLARITY |= (1u << mPinNumber);
        break;
    }
}
