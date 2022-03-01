FROM ubuntu

MAINTAINER Ethan Seiber

RUN apt update -y
RUN apt install -y emacs
RUN apt install -y cmake
RUN apt install -y build-essential
RUN apt install -y git-all
RUN apt install -y clang
RUN apt install -y htop
RUN apt install -y libmicrohttpd-dev
RUN apt install -y gnutls-dev
RUN apt install -y cmake
RUN apt install -y automake
RUN apt install -y autoconf
RUN apt install -y libtool
RUN apt install -y libjsoncpp-dev
RUN apt install -y locate
RUN apt install -y sudo
RUN git clone https://github.com/etr/libhttpserver.git

RUN mkdir libhttpserver/build
RUN cd libhttpserver/ && ./bootstrap
RUN cd /libhttpserver/build/ && ../configure
RUN cd libhttpserver/build/ && make
RUN cd libhttpserver/build/ && sudo make install

RUN ldconfig

RUN mv /usr/include/jsoncpp/* /usr/include/
RUN rm -r /usr/include/jsoncpp

#ENV LIBRARY_PATH=$LIBRARY_PATH:/usr/local/lib
ENV port=$port

#COPY src/server/* src/server/
#COPY src/pki/* src/pki/
#COPY src/makefile src
#COPY src/start.sh src
#COPY src/main.cpp src
#COPY src/server/resources src/server/resources
COPY src src

WORKDIR src/