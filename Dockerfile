FROM debian:9

#CONFIGURATION OF APT-GET ENVIRONMENT
#RUN   echo 'deb http://archive.debian.org/debian stretch main' > /etc/apt/sources.list && \
  #echo 'deb http://archive.debian.org/debian-security stretch/updates main' >> /etc/apt/sources.list
#ESSENTIAL
RUN   apt-get -o Acquire::Check-Valid-Until=false update && \
      apt-get -y install libssl-dev && \
      apt-get -y install build-essential && \
      apt-get -y install cmake && \
      apt-get -y install git && \
      apt-get -y install libcgicc-dev && \
      apt-get -y install curl && \
      apt-get -y install dpkg
#ENVIRONMENT
RUN   cd /opt && \
      git clone -b poco-1.9.0-release https://github.com/pocoproject/poco.git && \
      cd poco && \
      mkdir cmake-build && \
      cd cmake-build && \
      cmake .. && cmake --build . --target install
RUN   apt-get -y install libzmq3-dev
RUN   cd /opt && \
      git clone https://github.com/zeromq/zmqpp.git && \
      cd zmqpp && \
      mkdir .build && \
      cd .build && \
      cmake .. && \
      make && make install
ENV   LD_LIBRARY_PATH "/usr/local/lib;/usr/lib"
