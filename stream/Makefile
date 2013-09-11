ifndef TARGER
	TARGET := client
endif

LIBS		:=
			
OPTIMIZATION	:= -O0
CFLAGS		+= -fpermissive -Wall -std=c++11
CFLAGS		+= $(OPTIMIZATION) $(INCLUDES)
LDFLAGS		+= -ldl 
CC		:= g++ 
#----------------------------------------------------------------------
INCLUDES	:= 	-I include \
			

#----------------------------------------------------------------------
OUT		:= 	./bin
#----------------------------------------------------------------------
#SRC_C		:=	
SRC_CPP		:=	./src/socket/Socket.cpp \
			./src/client.cpp \
				
			
#----------------------------------------------------------------------
OBJ_C		+= $(SRC_C:%.c=%.o)
OBJ_CPP		+= $(SRC_CPP:%.cpp=%.o)
#OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
#----------------------------------------------------------------------
all: $(TARGET) clean
#----------------------------------------------------------------------
$(TARGET): $(OBJ_C) $(OBJ_CPP)
	@$(CC) -o $(TARGET)  $(OBJ_C) $(OBJ_CPP) $(LIBS) $(LDFLAGS)
	@mkdir -p $(OUT) 
	@rm -f $(OUT)/*
	@mv $(TARGET) $(OUT)
	@echo "Installed => $(OUT)/"$@

$(OBJ_CPP): %.o : %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled  => "$<
	
$(OBJ_C): %.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled  => "$<
#----------------------------------------------------------------------
clean:
	@rm -rf $(OBJ_C) $(OBJ_CPP) $(TARGET)
	@echo "Clean"
	