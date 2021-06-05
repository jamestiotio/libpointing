FROM debian:buster-slim

ARG BUILD_DATE

LABEL maintainer="Géry Casiez <gery.casiez@univ-lille.fr>" \
	  description="Docker for compiling Libpointing for Linux" \
	  version="1.0" \
	  org.label-schema.build-date=$BUILD_DATE

RUN apt-get update && apt-get install -y \
	sudo \
	apt-utils \ 
	apt-transport-https \
	ca-certificates \
	net-tools \
	systemd \
	apt-utils \
	python3 \
	python3-pip \
	cron \
	wget \
	ssh \
	vim \
	git \
	qtbase5-dev \
	qt5-default \
	qt5-qmake \
	libxrandr-dev \
	libudev-dev \
	cython \
	dialog \
	apt-utils \
	libxi-dev

RUN pip3 --no-cache-dir install cython


