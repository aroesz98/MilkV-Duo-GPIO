#ifndef MILKV_GPIO_HPP
#define MILKV_GPIO_HPP

#include <stdint.h>

class MilkV_GPIO
{
   public:
      enum GPIO_Port
      {
         GPIO_PORT_0 = 0,
         GPIO_PORT_1,
         GPIO_PORT_2,
         GPIO_PORT_3,
         PWR_GPIO
      };

      enum GPIO_Direction
      {
         GPIO_INPUT = 0,
         GPIO_OUTPUT
      };

      enum IntLevelType
      {
         LEVEL_SENSITIVE = 0,
         EDGE_SENSITIVE
      };

      enum IntPolarity
      {
         ACTIVE_LOW = 0,
         ACTIVE_HIGH
      };

      // Make a new GPIO pin instance.
      MilkV_GPIO(GPIO_Port port, uint32_t pinNumber);

      // Destructor.
      ~MilkV_GPIO(void);

      // Initialize GPIO pin.
      void init(GPIO_Direction pinDirection);

      // Write pin state.
      void writePin(bool pinState);

      // Read pin state.
      uint32_t readPin(void);
   
      // Enable / disable interrupts for GPIO pin.
      void enableInterrupt(void);
      void disableInterrupt(void);

      // Enable / disable interrupt mask for GPIO pin.
      void enableInterruptMask(void);
      void disableInterruptMask(void);

      // Enable / disable debouncing. Useful when button is connected.
      void enableDebounce(void);
      void disableDebounce(void);

      // Set interrupt type: Edge Sense / Level sense.
      void setInterruptLevelType(IntLevelType levelType = EDGE_SENSITIVE);

      // Set polarity of interrupt: Active Low / Active High.
      void setInterruptPolarity(IntPolarity polarity = ACTIVE_HIGH);

      uint32_t getIntStatus();
      uint32_t getRawIntStatus();

   private:
      GPIO_Port   _mPort;
      uint32_t    _mPinNumber;
};

#endif // MILKV_GPIO_HPP
