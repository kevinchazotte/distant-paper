FROM ubuntu:24.04

RUN apt-get update && \
	DEBIAN_FRONTEND=noninteractive apt-get install -y \
	cmake=3.28.* \
	software-properties-common \
	build-essential \
	ninja-build \
	clang \
	nasm \
	wget \
	git \
	pkg-config \
	libxrandr-dev libxcursor-dev xorg openbox \
	libxi-dev libudev-dev libflac-dev libvorbis-dev \
	libgl1-mesa-dev libegl1-mesa-dev libdrm-dev \
	libgbm-dev libfreetype6-dev libpthread-stubs0-dev \
	vorbis-tools libogg-dev libogg0 \
	ca-certificates

WORKDIR /home/dev/distant-paper
RUN git clone https://github.com/kevinchazotte/distant-paper.git .

RUN chmod +x cmake/buildPrompt.sh cmake/buildLinux.sh

RUN cd cmake && ./buildPrompt.sh

EXPOSE 50052

CMD ["bash"]
