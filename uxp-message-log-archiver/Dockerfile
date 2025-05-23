FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive

LABEL authors="Kirill Shypachov @kshypachov"

ARG REPO_KEY=http://192.168.99.247/key.gpg
ARG REPO_URI="http://192.168.99.247/ testing-1.22.7 trembita2-1-22-7"

RUN echo "uxp-proxy uxp-common/username string uxpadmin" | debconf-set-selections
#RUN echo "uxp-identity-provider-rest-api  uxp-identity-provider/password  password uxpadminp" | debconf-set-selections
#RUN useradd -m uxpadmin -s /usr/sbin/nologin -p '$6$7rx.CcTn$lkhsqW3zu6BrKbnQbOMaIFsZWv.DgH5LxtsXuxDftj8yF2e/KgxTOUQFozkYfcf1H.HSyxEtECMF8P7vy4M1b/'

RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '$6$7rx.CcTn$lkhsqW3zu6BrKbnQbOMaIFsZWv.DgH5LxtsXuxDftj8yF2e/KgxTOUQFozkYfcf1H.HSyxEtECMF8P7vy4M1b/' uxp

RUN apt-get update  \
    && apt-get install -y equivs  \
    && equivs-control fake-systemd \
    && sed -i 's/^Package:.*/Package: systemd/' fake-systemd \
    && echo 'Version: 255.0\nArchitecture: all\nDescription: dummy systemd for containers' >> fake-systemd \
    && equivs-build fake-systemd \
    && dpkg -i systemd_255.0_all.deb

#RUN apt-get update
#RUN apt-get install -y nano

RUN apt-get -qq update && apt-get -qq --no-install-recommends -y install \
      locales ca-certificates perl bzip2 libc6-dev lsb-release gnupg2 \
      ca-certificates gnupg supervisor net-tools iproute2 locales \
      rlwrap ca-certificates-java debconf-utils \
      crudini adduser expect curl rsyslog dpkg-dev \
      python3-requests \
    && echo "LC_ALL=en_US.UTF-8" >>/etc/environment \
    && locale-gen en_US.UTF-8 \
    && apt-get clean  \
    && rm -rf /var/lib/apt/lists/*

ADD ["$REPO_KEY","/tmp/repokey.gpg"]
RUN apt-key add '/tmp/repokey.gpg'

RUN sed -i 's/^[A-Za-z0-9]/#&/' /etc/apt/sources.list \
#    && rm -rf /etc/apt/sources.list.d/* \
    && echo "deb $REPO_URI" | tee -a /etc/apt/sources.list

RUN printf  '#!/bin/sh\nexit 101\n' > /usr/sbin/policy-rc.d && chmod +x /usr/sbin/policy-rc.d \
    && printf  '#!/bin/sh\nexit 0\n' > /usr/sbin/service && chmod +x /usr/sbin/service \
    && printf '#!/bin/sh\nexit 0\n' > /bin/systemctl && chmod +x /bin/systemctl

#RUN apt update
#RUN apt install -y uxp-confclient
RUN mkdir "/deb"
COPY ./deb/*.deb /deb/
COPY uxp-proxy_1.22.7_all.deb uxp-proxy_1.22.7_all.deb

RUN apt update
RUN apt -qq -y --no-install-recommends install /deb/*.deb
RUN dpkg-deb -X /uxp-proxy_1.22.7_all.deb /

#COPY messagelog-archiver-logback.xml /etc/uxp/conf.d/messagelog-archiver-logback.xml

#ENTRYPOINT ["top", "-b"]
USER uxp
ENTRYPOINT ["/usr/share/uxp/bin/messagelog-archiver.sh"]