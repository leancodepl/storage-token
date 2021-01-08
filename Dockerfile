FROM alpine:3.12 AS build

RUN apk add --no-cache cmake curl-dev g++ git libxml2-dev make && \
    apk add nlohmann-json --allow-untrusted --update-cache --repository 'http://dl-3.alpinelinux.org/alpine/edge/community/'

WORKDIR /code
COPY azure-sdk-for-cpp/ ./azure-sdk-for-cpp
COPY main.cpp ./
COPY CMakeLists.txt ./

RUN cmake . -DCMAKE_BUILD_TYPE=Release && \
    make -j && \
    ls -la && \
    strip --strip-unneeded ./storagetoken

FROM alpine:3.12

RUN apk add --no-cache libxml2 libcurl libgcc libstdc++
COPY --from=build /code/storagetoken /usr/bin/storagetoken

ENTRYPOINT ["/usr/bin/storagetoken"]
