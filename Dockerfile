FROM gcc:13

# Install CMake
RUN apt-get update && apt-get install -y cmake

WORKDIR /app
COPY . .

# Build
RUN mkdir build && cd build && \
    cmake .. && \
    make

CMD ["./build/ddns-cpp"]
