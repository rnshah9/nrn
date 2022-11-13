FROM --platform=linux/amd64 ubuntu:22.04 as builder

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential bison cmake flex git libncurses-dev libopenmpi-dev libx11-dev libxcomposite-dev openmpi-bin python3-dev libreadline-dev cython3

COPY . /repo
WORKDIR /repo/build
RUN cmake .. -DCMAKE_INSTALL_PREFIX=/install
RUN make -j8
RUN make install

FROM ubuntu:22.04 as package
RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y libreadline-dev python3-dev libopenmpi-dev
COPY --from=builder /install /install
ENV LD_LIBRARY_PATH=/install/lib
