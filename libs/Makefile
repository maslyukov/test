OUT := ./bin

all: DIR EXCEPTION SOCKET
DIR:
	@mkdir -p $(OUT)
	@rm -f $(OUT)/*


EXCEPTION:
	make -f ./Makefile.exception TARGET=libexcept.so

SOCKET:
	make -f ./Makefile.socket TARGET=libsocket.so

#----------------------------------------------------------------------
#$(TARGET): $(OBJ_C) $(OBJ_CPP)
#	@$(CC) -o $(TARGET)  $(OBJ_C) $(OBJ_CPP) $(LIBS) $(LDFLAGS)
#	@mkdir -p $(OUT) 
#	@rm -f $(OUT)/*
#	@mv $(TARGET) $(OUT)
#	@echo "Installed => $(OUT)/"$@


#$(OBJ_CPP): %.o : %.cpp
#	@$(CC) $(CFLAGS) -c $< -o $@
#	@echo "Compiled  => "$<
	
#$(OBJ_C): %.o : %.c
#	@$(CC) $(CFLAGS) -c $< -o $@
#	@echo "Compiled  => "$<
#----------------------------------------------------------------------
#clean:
#	@rm -rf $(OBJ_C) $(OBJ_CPP) $(TARGET)
#	@echo "Clean"
	