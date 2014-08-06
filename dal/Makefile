include ../../lightframe/Makefile_public

GATESERVER_DIR = $(BASE_DIR)/gateserver

GATESERVER_INCLUDE_DIR = $(GATESERVER_DIR)

GATESERVER_OBJ_DIR = $(GATESERVER_DIR)/.objs

SRC = $(wildcard *.cpp)
OBJS = $(addprefix $(GATESERVER_OBJ_DIR)/, $(subst .cpp,.o,$(SRC)))

INC = -I$(LIGHTFRAME_INCLUDE_DIR) -I$(GATESERVER_INCLUDE_DIR) -I$(MYSQL_INCLUDE_DIR) -I$(PUBLIC_INCLUDE_DIR)

all : $(OBJS)

$(GATESERVER_OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJS)
