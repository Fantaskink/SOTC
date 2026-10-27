FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

ARG TARGETARCH
RUN if [ "$TARGETARCH" = "arm64" ]; then \
    echo "DEBUG: On arm64 please run 'docker buildx build --platform linux/amd64 -t sotc .' when building."; \
    exit 1; \
    fi


RUN dpkg --add-architecture i386 \
 && apt-get update && apt-get install -y --no-install-recommends \
    gcc-mipsel-linux-gnu \
    binutils-mips-linux-gnu \
    ninja-build \
    python3 \
    python3-pip \
    python3-venv \
    cmake \
    cpp \
    perl \
    libc6-i386 \
    libc6:i386 \
    libstdc++6:i386 \
    build-essential \
    git \
    rsync \
    curl \
 && rm -rf /var/lib/apt/lists/*

RUN dpkg --add-architecture i386 \
      && apt-get update \
      && apt-get install -y libc6:i386 libstdc++6:i386 \
      && rm -rf /var/lib/apt/lists/*;

COPY requirements.txt /tmp/requirements.txt
RUN python3 -m venv /opt/sotc-venv \
 && /opt/sotc-venv/bin/pip install -U pip \
 && /opt/sotc-venv/bin/pip install --no-cache-dir -r /tmp/requirements.txt \
 && rm /tmp/requirements.txt

ENV DECOMP_PYTHON=/opt/sotc-venv/bin/python3
ENV PATH="/opt/sotc-venv/bin:$PATH"

ENV DECOMP_EEGCC_URL=https://github.com/decompme/compilers/releases/download/compilers/ee-gcc2.96.tar.xz

RUN mkdir -p /opt/sotc-cc/ee-gcc2.96 \
 && curl -fL -o /tmp/ee-gcc2.96.tar.xz "$DECOMP_EEGCC_URL" \
 && tar -xf /tmp/ee-gcc2.96.tar.xz -C /opt/sotc-cc/ee-gcc2.96 \
 && chmod -R a+x /opt/sotc-cc/ee-gcc2.96 \
 && rm /tmp/ee-gcc2.96.tar.xz

RUN curl -L -o /usr/local/bin/objdiff-cli https://github.com/encounter/objdiff/releases/latest/download/objdiff-cli-linux-x86_64 \
    && chmod +x /usr/local/bin/objdiff-cli

ENV DECOMP_CC=/opt/sotc-cc/ee-gcc2.96/bin/ee-gcc


WORKDIR /workspace

COPY docker-entrypoint.sh /usr/local/bin/docker-entrypoint.sh
RUN chmod +x /usr/local/bin/docker-entrypoint.sh

ENTRYPOINT ["/usr/local/bin/docker-entrypoint.sh"]