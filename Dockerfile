FROM arm64v8/debian:bookworm

#Install requirements for raspios gpg key
RUN apt update
RUN apt install wget gnupg -y
#Add Raspios apt repository
RUN echo "deb http://archive.raspberrypi.com/debian/ bookworm main" >> /etc/apt/sources.list.d/raspi.list
RUN echo "deb-src http://archive.raspberrypi.com/debian/ bookworm main" >> /etc/apt/sources.list.d/raspi.list
RUN wget -q http://archive.raspberrypi.com/debian/raspberrypi.gpg.key -O- | apt-key add -
RUN apt-get update
RUN apt install git devscripts zip -y

#Get libcamera build requirements
RUN apt build-dep libcamera -y
