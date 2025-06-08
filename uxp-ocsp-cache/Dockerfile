FROM ubuntu:24.04 as builder
ENV DEBIAN_FRONTEND=noninteractive

RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '' uxp

RUN mkdir "/deb"
COPY ./deb/ /deb/

RUN dpkg-deb -X /deb/uxp-proxy_1.22.7_all.deb                  /tmp/proxy
RUN dpkg-deb -X /deb/uxp-addon-trembita-crypto_1.22.7_all.deb  /tmp/crypto

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

RUN rm /bin/bash && rm /bin/sh

#ENTRYPOINT ["top", "-b"]
USER uxp
#ENTRYPOINT ["java", \
#           "-Xmx128m", \
#           "-Xshare:on", \
#           "-XX:MaxMetaspaceSize=100m", \
#           "-XX:+UseG1GC", \
#           "-Dlogback.configurationFile=/app/ocsp-cache-logback.xml", \
#           "-Dfile.encoding=UTF-8", \
#           "-Djava.library.path=/usr/share/uxp/lib/", \
#           "-Dorg.bytedeco.javacpp.noPointerGC=true", \
#           "-Dorg.bytedeco.javacpp.maxBytes=0", \
#           "-Dorg.bytedeco.javacpp.maxPhysicalBytes=0", \
#           "-cp", "/app/jlib/ocsp-cache.jar:/app/jlib/signature-xades.jar:/app/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/app/jlib/addon/proxy/ciplus-jce/*" ,\
#           "-Dorg.bytedeco.javacpp.cachedir=/tmp/bc_java/", \
#           "-Duxp.ocsp-cache.admin-port=5766", \
#           "-Duxp.ocsp-cache.ocsp-responder-listen-port=5577", \
#           "-Duxp.ocsp-cache.update-interval=0 0 0/6 1/1 * ? *", \
#           "-Duxp.ocsp-cache.ocsp-responder-listen-address=0.0.0.0", \
#           "-Duxp.ocsp-cache.full-update-freshness-divisor=10", \
#           "-Duxp.ocsp-cache.client-connect-timeout=10000", \
#           "ee.cyber.uxp.ocsp.OcspCacheMain"]