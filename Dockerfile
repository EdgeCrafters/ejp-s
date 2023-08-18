FROM mcr.microsoft.com/devcontainers/base:ubuntu-22.04

RUN apt-get clean && \
    apt-get update --fix-missing && \
    apt-get install -y \
    cmake \
    ninja-build \
    clang-14 \
    clang++-14 \
    && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN rm -rf ~/.ssh \
    && ssh-keygen -t rsa -f /root/.ssh/id_rsa -N ""

ENV CC=clang
ENV CXX=clang++

CMD ["/bin/bash"]