# Stage 1: Build
FROM --platform=linux/amd64 ghcr.io/yadoms/build_for_linux:latest as builder
LABEL authors="oussama DAHMAZ"

WORKDIR /yadoms

# Copy the source code into the container
COPY . .

# Copy the build configuration file
RUN echo "Copy build config file" && \
    cp $YADOMS_DEPS_PATH/CMakeListsUserConfig.txt sources/

# Display the config content
RUN echo "Display config content" && \
    cat sources/CMakeListsUserConfig.txt

# Create the makefile
RUN echo "Create makefile" && \
    sh cmake_linux.sh r

# Build Yadoms
WORKDIR /yadoms/projects
RUN echo "Build Yadoms" && \
    make all_unity && \
    echo "Build Yadoms package" && \
    make package

# Stage 2: Final runtime image
FROM --platform=linux/amd64 debian:stable as runtime

RUN echo "deb http://deb.debian.org/debian bullseye main" > /etc/apt/sources.list.d/bullseye.list \
    && echo "deb http://deb.debian.org/debian bullseye-updates main" >> /etc/apt/sources.list.d/bullseye.list \
    && echo "deb http://security.debian.org bullseye-security main" >> /etc/apt/sources.list.d/bullseye.list

RUN apt-get update \
	&& apt-get install -y g++ libbz2-dev python2.7-dev python3-dev wget build-essential libssl-dev \
	autoconf automake libtool curl make g++ unzip \
	libpcre3 libpcre3-dev libpq5 \
	postgresql-server-dev-all git rpm zip \
	libreadline-dev libffi-dev build-essential \
	libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev \
	python3-numpy libtbbmalloc2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev \
	libudev-dev usbutils

WORKDIR /yadoms/builds

# Copy the built Yadoms binaries from the builder stage
COPY --from=builder /yadoms/builds/ /yadoms/builds/

# Set the entry point
ENTRYPOINT ["./yadoms"]
