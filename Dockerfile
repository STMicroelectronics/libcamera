FROM arm64v8/debian:trixie

ENV DEBIAN_FRONTEND=noninteractive

#Install requirements for raspios gpg key
RUN apt-get update
RUN apt-get install curl gnupg -y
#Add Raspios apt repository
RUN curl -fsSL http://archive.raspberrypi.com/debian/raspberrypi.gpg.key | gpg --dearmor -o /etc/apt/keyrings/raspberrypi.gpg
RUN echo "deb [signed-by=/etc/apt/keyrings/raspberrypi.gpg] http://archive.raspberrypi.com/debian/ trixie main" >> /etc/apt/sources.list.d/raspi.list
RUN echo "deb-src [signed-by=/etc/apt/keyrings/raspberrypi.gpg] http://archive.raspberrypi.com/debian/ trixie main" >> /etc/apt/sources.list.d/raspi.list
RUN apt-get update
#Install libcamera build requirements
RUN apt-get build-dep libcamera -y
#Install packaging requirements
RUN apt-get install git devscripts zip -y
