FROM ubuntu:24.04 as builder
ENV DEBIAN_FRONTEND=noninteractive

RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '' uxp

RUN mkdir "/deb"
COPY ./deb/ /deb/

RUN dpkg-deb -X /deb/uxp-proxy_1.22.7_all.deb                  /tmp/proxy
RUN dpkg-deb -X /deb/uxp-addon-trembita-crypto_1.22.7_all.deb  /tmp/crypto

#ENTRYPOINT ["top", "-b"]

FROM eclipse-temurin:17-jdk-jammy

LABEL authors="Kirill Shypachov (@kshypachov), on behalf of eGA Kyiv"
LABEL org.opencontainers.image.authors="Kirill Shypachov <kiril.shypachov@ega.ee> (eGA Kyiv)"
LABEL org.opencontainers.image.vendor="eGA Kyiv"
LABEL org.opencontainers.image.title="trembita ocsp cache"
LABEL org.opencontainers.image.url="URI documentation"
LABEL org.opencontainers.image.source=""
LABEL org.opencontainers.image.version=""

RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '' uxp

RUN mkdir -p /app/jlib/addon/proxy/ciplus-jce/
RUN mkdir -p /deb/

COPY --from=builder /tmp/proxy/usr/share/uxp/jlib/ocsp*                                       /app/jlib/

COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/signature-xades.jar                        /app/jlib/
COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/addon/proxy/ciplus-jce/                    /app/jlib/addon/proxy/ciplus-jce/
COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar /app/jlib/addon/proxy/

COPY ocsp-cache-logback.xml                                             /app/
COPY deb/libgoogle-perftools4_2.9.1-0ubuntu3_amd64.deb                  /deb/
COPY deb/libtcmalloc-minimal4_2.9.1-0ubuntu3_amd64.deb                  /deb/
COPY deb/libunwind8_1.3.2-2build2.1_amd64.deb                           /deb/
COPY deb/libgomp1_12.3.0-1ubuntu1~22.04_amd64.deb                       /deb/

RUN apt install /deb/libunwind8_1.3.2-2build2.1_amd64.deb
RUN apt install /deb/libtcmalloc-minimal4_2.9.1-0ubuntu3_amd64.deb
RUN apt install /deb/libgomp1_12.3.0-1ubuntu1~22.04_amd64.deb
RUN apt install /deb/libgoogle-perftools4_2.9.1-0ubuntu3_amd64.deb && rm -rf /deb

ENV LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libtcmalloc.so.4


#ENTRYPOINT ["top", "-b"]
USER uxp
ENTRYPOINT ["java", \
           "-Xmx128m", \
           "-Xshare:on", \
           "-XX:MaxMetaspaceSize=100m", \
           "-XX:+UseG1GC", \
           "-Dlogback.configurationFile=/app/ocsp-cache-logback.xml", \
           "-Dfile.encoding=UTF-8", \
           "-Djava.library.path=/usr/share/uxp/lib/", \
           "-Dorg.bytedeco.javacpp.noPointerGC=true", \
           "-Dorg.bytedeco.javacpp.maxBytes=0", \
           "-Dorg.bytedeco.javacpp.maxPhysicalBytes=0", \
           "-cp", "/app/jlib/ocsp-cache.jar:/app/jlib/signature-xades.jar:/app/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/app/jlib/addon/proxy/ciplus-jce/*" ,\
           "-Dorg.bytedeco.javacpp.cachedir=/tmp/bc_java/", \
           "-Duxp.ocsp-cache.admin-port=5766", \
           "-Duxp.ocsp-cache.ocsp-responder-listen-port=5577", \
           "-Duxp.ocsp-cache.update-interval=0 0 0/6 1/1 * ? *", \
           "-Duxp.ocsp-cache.ocsp-responder-listen-address=0.0.0.0", \
           "-Duxp.ocsp-cache.full-update-freshness-divisor=10", \
           "-Duxp.ocsp-cache.client-connect-timeout=10000", \
           "ee.cyber.uxp.ocsp.OcspCacheMain"]

#FROM ubuntu:24.04
#ENV DEBIAN_FRONTEND=noninteractive
#
#LABEL authors="Kirill Shypachov @kshypachov"
#
#ARG REPO_KEY=http://192.168.99.247/key.gpg
#ARG REPO_URI="http://192.168.99.247/ testing-1.22.7 trembita2-1-22-7"
#
#RUN echo "uxp-proxy uxp-common/username string uxpadmin" | debconf-set-selections
##RUN echo "uxp-identity-provider-rest-api  uxp-identity-provider/password  password uxpadminp" | debconf-set-selections
##RUN useradd -m uxpadmin -s /usr/sbin/nologin -p '$6$7rx.CcTn$lkhsqW3zu6BrKbnQbOMaIFsZWv.DgH5LxtsXuxDftj8yF2e/KgxTOUQFozkYfcf1H.HSyxEtECMF8P7vy4M1b/'
#
#RUN groupadd -g 104 uxp && \
#    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '$6$7rx.CcTn$lkhsqW3zu6BrKbnQbOMaIFsZWv.DgH5LxtsXuxDftj8yF2e/KgxTOUQFozkYfcf1H.HSyxEtECMF8P7vy4M1b/' uxp
#
#RUN apt-get update  \
#    && apt-get install -y equivs  \
#    && equivs-control fake-systemd \
#    && sed -i 's/^Package:.*/Package: systemd/' fake-systemd \
#    && echo 'Version: 255.0\nArchitecture: all\nDescription: dummy systemd for containers' >> fake-systemd \
#    && equivs-build fake-systemd \
#    && dpkg -i systemd_255.0_all.deb
#
##RUN apt-get update
##RUN apt-get install -y nano
#
#RUN apt-get -qq update && apt-get -qq --no-install-recommends -y install \
#      locales ca-certificates perl bzip2 libc6-dev lsb-release gnupg2 \
#      ca-certificates gnupg supervisor net-tools iproute2 locales \
#      rlwrap ca-certificates-java debconf-utils \
#      crudini adduser expect curl rsyslog dpkg-dev \
#      python3-requests \
#    && echo "LC_ALL=en_US.UTF-8" >>/etc/environment \
#    && locale-gen en_US.UTF-8 \
#    && apt-get clean  \
#    && rm -rf /var/lib/apt/lists/*
#
#ADD ["$REPO_KEY","/tmp/repokey.gpg"]
#RUN apt-key add '/tmp/repokey.gpg'
#
#RUN sed -i 's/^[A-Za-z0-9]/#&/' /etc/apt/sources.list \
##    && rm -rf /etc/apt/sources.list.d/* \
#    && echo "deb $REPO_URI" | tee -a /etc/apt/sources.list
#
#RUN printf  '#!/bin/sh\nexit 101\n' > /usr/sbin/policy-rc.d && chmod +x /usr/sbin/policy-rc.d \
#    && printf  '#!/bin/sh\nexit 0\n' > /usr/sbin/service && chmod +x /usr/sbin/service \
#    && printf '#!/bin/sh\nexit 0\n' > /bin/systemctl && chmod +x /bin/systemctl
#
##RUN apt update
##RUN apt install -y uxp-confclient
#RUN mkdir "/deb"
#COPY ./deb/*.deb /deb/
#COPY uxp-proxy_1.22.7_all.deb uxp-proxy_1.22.7_all.deb
#
#RUN apt update
#RUN apt -qq -y --no-install-recommends install /deb/*.deb
#RUN dpkg-deb -X /uxp-proxy_1.22.7_all.deb /
#
#COPY ocsp-cache-logback.xml /etc/uxp/conf.d/ocsp-cache-logback.xml
#
#RUN mkdir /var/cache/uxp && chown uxp:uxp /var/cache/uxp && chmod 750 /var/cache/uxp
#
##USER uxp
##ENTRYPOINT ["top", "-b"]
#ENTRYPOINT ["/usr/share/uxp/bin/ocsp-cache.sh"]