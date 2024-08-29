ENV DEBIAN_FRONTEND=noninteractive

# Install required packages
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    libssl-dev \
    && apt-get clean

# Set the working directory inside the container
WORKDIR /app/Memify
# Copy the project files into the container
COPY . .
# Run CMake to configure the project

RUN mv config.ini ../
RUN cmake .

# Build the project using make
RUN make

CMD ["./Memify"]