FROM ghcr.io/yadoms/build_for_linux:latest
LABEL authors="oussama DAHMAZ"

WORKDIR yadoms
COPY . .

RUN echo "Copy build config file"
RUN cp $YADOMS_DEPS_PATH/CMakeListsUserConfig.txt sources/

RUN echo "Display config content"
RUN cat sources/CMakeListsUserConfig.txt

RUN echo "Create makefile"
RUN sh cmake_linux.sh r

RUN echo "Build Yadoms"

WORKDIR /yadoms/projects

RUN make all_unity
RUN echo "Build Yadoms package"
RUN make package

WORKDIR /yadoms/builds

ENTRYPOINT ["./yadoms"]