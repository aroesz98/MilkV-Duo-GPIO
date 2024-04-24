/*
 *  MilkV_GPIO.cpp
 *
 *  Created on: Apr 24, 2024
 *  Updated on: Apr 24, 2024
 * 
 *  Author: Arkadiusz Szlanta
 */

#include <MilkV_GPIO.hpp>

#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

typedef struct
{
    uint32_t SWPORTA_DR;      // 0x00
    uint32_t SWPORTA_DDR;     // 0x04
    uint32_t reserved1;       // 0x08
    uint32_t reserved2;       // 0x0C
    uint32_t reserved3;       // 0x10
    uint32_t reserved4;       // 0x14
    uint32_t reserved5;       // 0x18
    uint32_t reserved6;       // 0x1C
    uint32_t reserved7;       // 0x20
    uint32_t reserved8;       // 0x24
    uint32_t reserved9;       // 0x28
    uint32_t reserved10;      // 0x2C
    uint32_t INTEN;           // 0x30
    uint32_t INTMASK;         // 0x34
    uint32_t INTTYPE_LEVEL;   // 0x38
    uint32_t INT_POLARITY;    // 0x3C
    uint32_t INT_STATUS;      // 0x40
    uint32_t RAW_INT_STATUS;  // 0x44
    uint32_t DEBOUNCE;        // 0x48
    uint32_t PORTA_EOI;       // 0x4C
    uint32_t PORTA_EXT;       // 0x50
    uint32_t reserved11;      // 0x54
    uint32_t reserved12;      // 0x58
    uint32_t reserved13;      // 0x5C
    uint32_t LS_SYNC;         // 0x60
} __gpio_reg;

#define GPIO0_BASE      (0x03020000ul)
#define GPIO1_BASE      (0x03021000ul)
#define GPIO2_BASE      (0x03022000ul)
#define GPIO3_BASE      (0x03023000ul)
#define PWR_GPIO_BASE   (0x05021000ul)

#define GPIO0       ((__gpio_reg *)GPIO0_BASE)
#define GPIO1       ((__gpio_reg *)GPIO1_BASE)
#define GPIO2       ((__gpio_reg *)GPIO2_BASE)
#define GPIO3       ((__gpio_reg *)GPIO3_BASE)
#define PWR_GPIO    ((__gpio_reg *)PWR_GPIO_BASE)

std::vector<__gpio_reg*> _gpio_list;

static void getGPIOAccess(void)
{
    int fdMem = open("/dev/mem", O_RDWR | O_SYNC);

    __gpio_reg *gp0_reg     = (__gpio_reg*)(mmap(0, sizeof(__gpio_reg), PROT_READ | PROT_WRITE, MAP_SHARED, fdMem, GPIO0_BASE));
    __gpio_reg *gp1_reg     = (__gpio_reg*)(mmap(0, sizeof(__gpio_reg), PROT_READ | PROT_WRITE, MAP_SHARED, fdMem, GPIO1_BASE));
    __gpio_reg *gp2_reg     = (__gpio_reg*)(mmap(0, sizeof(__gpio_reg), PROT_READ | PROT_WRITE, MAP_SHARED, fdMem, GPIO2_BASE));
    __gpio_reg *gp3_reg     = (__gpio_reg*)(mmap(0, sizeof(__gpio_reg), PROT_READ | PROT_WRITE, MAP_SHARED, fdMem, GPIO3_BASE));
    __gpio_reg *pwr_gp_reg  = (__gpio_reg*)(mmap(0, sizeof(__gpio_reg), PROT_READ | PROT_WRITE, MAP_SHARED, fdMem, PWR_GPIO_BASE));

    close(fdMem);

    _gpio_list.push_back(gp0_reg);
    _gpio_list.push_back(gp1_reg);
    _gpio_list.push_back(gp2_reg);
    _gpio_list.push_back(gp3_reg);
    _gpio_list.push_back(pwr_gp_reg);
}

MilkV_GPIO::MilkV_GPIO(GPIO_Port port, uint32_t pinNumber)
{
    _mPort = port;
    _mPinNumber = pinNumber;

    getGPIOAccess();
}

MilkV_GPIO::~MilkV_GPIO(void)
{
    _gpio_list.clear();
    _gpio_list.~vector();
}

void MilkV_GPIO::init(GPIO_Direction pinDirection)
{
    switch (pinDirection)
    {
        case GPIO_INPUT:
            _gpio_list[_mPort]->SWPORTA_DDR &= ~(1u << _mPinNumber);
            break;

        case GPIO_OUTPUT:
            _gpio_list[_mPort]->SWPORTA_DDR |=  (1u << _mPinNumber);
            break;
    }
}

void MilkV_GPIO::writePin(bool pinState)
{
    switch(pinState)
    {
        case 0:
            _gpio_list[_mPort]->SWPORTA_DR  &= ~(1u << _mPinNumber);
            break;
        
        case 1:
            _gpio_list[_mPort]->SWPORTA_DR  |=  (1u << _mPinNumber);
            break;
    }
}

uint32_t MilkV_GPIO::readPin(void)
{
    uint32_t pinState = ((_gpio_list[_mPort]->SWPORTA_DR) & (1 << _mPinNumber)) > 0u ? 1u : 0u;
    return pinState;
}

void MilkV_GPIO::enableInterrupt(void)
{
    _gpio_list[_mPort]->INTEN    |=  (1u << _mPinNumber);
}

void MilkV_GPIO::disableInterrupt(void)
{
    _gpio_list[_mPort]->INTEN    &= ~(1u << _mPinNumber);
}

void MilkV_GPIO::enableInterruptMask(void)
{
    _gpio_list[_mPort]->INTMASK  |=  (1u << _mPinNumber);
}

void MilkV_GPIO::disableInterruptMask(void)
{
    _gpio_list[_mPort]->INTMASK  &= ~(1u << _mPinNumber);
}

void MilkV_GPIO::enableDebounce(void)
{
    _gpio_list[_mPort]->DEBOUNCE |=  (1u << _mPinNumber);
}

void MilkV_GPIO::disableDebounce(void)
{
    _gpio_list[_mPort]->DEBOUNCE &= ~(1u << _mPinNumber);
}

void MilkV_GPIO::setInterruptLevelType(IntLevelType levelType)
{
    switch(levelType)
    {
        case LEVEL_SENSITIVE:
            _gpio_list[_mPort]->INTTYPE_LEVEL &= ~(1u << _mPinNumber);
            break;
        
        case EDGE_SENSITIVE:
            _gpio_list[_mPort]->INTTYPE_LEVEL |=  (1u << _mPinNumber);
            break;
    }
}

void MilkV_GPIO::setInterruptPolarity(IntPolarity polarity)
{
    switch(polarity)
    {
        case ACTIVE_LOW:
            _gpio_list[_mPort]->INT_POLARITY &= ~(1u << _mPinNumber);
            break;
        
        case ACTIVE_HIGH:
            _gpio_list[_mPort]->INT_POLARITY |=  (1u << _mPinNumber);
            break;
    }
}
