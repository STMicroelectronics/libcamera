FROM arm64v8/debian:trixie

ENV DEBIAN_FRONTEND=noninteractive

#Install requirements for raspios gpg key
RUN apt-get update
#Add Raspios apt repository
RUN echo "deb [trusted=yes] http://archive.raspberrypi.com/debian/ trixie main" >> /etc/apt/sources.list.d/raspi.list
RUN echo "deb-src [trusted=yes] http://archive.raspberrypi.com/debian/ trixie main" >> /etc/apt/sources.list.d/raspi.list
RUN apt-get update
#Install libcamera build requirements
RUN apt-get build-dep libcamera -y
#Install packaging requirements
RUN apt-get install git devscripts zip curl -y
