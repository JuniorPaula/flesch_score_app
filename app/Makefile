APP_NAME := app
SRC := main.go

LIB_PATH := ../readability/target/release
LIB_CPP_PATH := ../text_embedder
LD_PATH := $(LIB_PATH):$(LIB_CPP_PATH)

.PHONY: build run clean

build:
	go build -o $(APP_NAME) $(SRC)

run: build
	LD_LIBRARY_PATH=$(LD_PATH) ./$(APP_NAME)

clean:
	rm -f $(APP_NAME)
