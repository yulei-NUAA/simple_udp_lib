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

TARGET := MakefileDemo

# Every subdirectory with source files must be described here
SUBDIRS := .

C_SRCS += ./TimeMeasureDemoC.c 

OBJS += ./TimeMeasureDemoC.o

C_DEPS += ./TimeMeasureDemoC.d

# vpath .c/.h/.o/.d

# Each subdirectory must supply rules for building sources it contributes
%.o: ./%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

USER_OBJS :=

LIBS :=

-include $(C_DEPS)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(TARGET)

# Tool invocations
$(TARGET): $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "$(TARGET)" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) $(TARGET)
	-@echo ' '


.PHONY: all clean dependents

debug:
	@echo MAKECMDGOALS = $(MAKECMDGOALS)
	@echo C_DEPS = $(C_DEPS)
	@echo EXECUTABLES = $(EXECUTABLES)