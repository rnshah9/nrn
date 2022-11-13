FROM --platform=linux/amd64 ubuntu:22.04 as builder

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential bison cmake flex git libncurses-dev libopenmpi-dev libx11-dev libxcomposite-dev openmpi-bin python3-dev libreadline-dev cython3
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y clang

COPY . /repo
WORKDIR /repo/build
RUN cmake .. -DCMAKE_INSTALL_PREFIX=/install
RUN make -j8
RUN make install
RUN clang++ /repo/mayhem/fuzz_vtrap.cpp -fsanitize=fuzzer,address -lrxdmath -L/install/lib -o /fuzz

FROM ubuntu:22.04 as package
RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y libreadline-dev python3-dev libopenmpi-dev
COPY --from=builder /install /install
COPY --from=builder /fuzz /fuzz_vtrap
ENV LD_LIBRARY_PATH=/install/lib
