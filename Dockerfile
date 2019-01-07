FROM reaverproject/llvm-boost:70-1.69.0

RUN apt-get update
RUN apt-get -y install valgrind cmake
RUN apt-get -y install g++ curl python
RUN apt-get -y install git

RUN curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
RUN python get-pip.py

RUN pip install conan

# Build/Install libraries
WORKDIR /dependencies
# Install yomm2
RUN git clone https://github.com/jll63/yomm2.git \
    && cd yomm2 \
    && mkdir build \
    && cd build \
    && cmake ../ \
    && make \
    && make install

COPY . /code

ENV CONAN_COMPILER=clang
# Install conan dependencies
WORKDIR /code
RUN conan install .

WORKDIR /code
