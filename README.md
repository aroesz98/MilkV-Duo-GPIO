# MilkV-Duo-GPIO
Milk V Duo Bare Metal GPIO Driver

Basically I made that library to get access to GPIO peripheral from the OS.
The easiest way to compile that library is to clone duo-examples repository, follow the instruction to setup compiler and environment than add that project to examples.

Interface is very simple:

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

Example code of usage MilkV GPIO Driver inside blink.cpp file.
