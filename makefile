RM := rm -rf

# All of the sources participating in the build are defined here
OBJ_SRCS := 
ASM_SRCS := 
C_SRCS := 
O_SRCS := 
S_UPPER_SRCS := 
EXECUTABLES := 
OBJS := 
C_DEPS := 

TARGET := LibDemo.exec

# Every subdirectory with source files must be described here
SUBDIRS := .

C_SRCS += ./UDPLibDemo/UDPLibDemo.cpp ./UDPLib/SimpleUDPLib.cpp

OBJS += ./UDPLibDemo/UDPLibDemo.o ./UDPLib/SimpleUDPLib.o

C_DEPS += ./UDPLibDemo/UDPLibDemo.d ./UDPLib/SimpleUDPLib.d

CF_FLAGS = -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -I$(shell pwd)/UDPLib -DUSE_UDP_LIB_SOURCE_CODE


# Each subdirectory must supply rules for building sources it contributes
%.o:%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	g++ $(CF_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	

USER_OBJS :=

LIBS :=

-include $(C_DEPS)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(TARGET)
	echo make all command
# Tool invocations
$(TARGET): $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	g++ -o "$(TARGET)" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES) $(OBJS) $(C_DEPS) $(TARGET)
	-@echo ' '


.PHONY: all clean dependents

debug:
	@echo MAKECMDGOALS = $(MAKECMDGOALS)
	@echo C_DEPS = $(C_DEPS)
	@echo EXECUTABLES = $(EXECUTABLES)
	@echo OBJS = $(OBJS)