BUILD_DIR = build

define windows_compile
	GOOS=windows GOARCH=amd64 go build -mod=vendor -ldflags="-s -w" -o ${BUILD_DIR}/omicron-windows.exe main.go
endef

define linux_compile
	GOOS=linux GOARCH=amd64 go build -mod=vendor -ldflags="-s -w" -o ${BUILD_DIR}/omicron-linux main.go
endef

define macos_compile
	GOOS=darwin GOARCH=amd64 go build -mod=vendor -ldflags="-s -w" -o ${BUILD_DIR}/omicron-darwin main.go
endef

clean:
	rm -rf ${BUILD_DIR}

install:
	cp ${BUILD_DIR}/* $(GOBIN)

windows:
	$(call windows_compile)

linux:
	$(call linux_compile)

macos:
	$(call macos_compile)

all: windows linux macos

run:
	./${BUILD_DIR}/omicron-linux
