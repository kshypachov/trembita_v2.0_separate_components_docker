FROM ubuntu:24.04 as builder
ENV DEBIAN_FRONTEND=noninteractive

RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '' uxp

RUN mkdir "/deb"
COPY ./deb/ /deb/

RUN dpkg-deb -X /deb/uxp-confclient_1.22.7_amd64.deb           /tmp/conf
RUN dpkg-deb -X /deb/uxp-addon-trembita-crypto_1.22.7_all.deb  /tmp/crypto


FROM eclipse-temurin:17-jdk-jammy

LABEL authors="Kirill Shypachov (@kshypachov), on behalf of eGA Kyiv"
LABEL org.opencontainers.image.authors="Kirill Shypachov <kiril.shypachov@ega.ee> (eGA Kyiv)"
LABEL org.opencontainers.image.vendor="eGA Kyiv"
LABEL org.opencontainers.image.title="trembita configuration client"
LABEL org.opencontainers.image.url="URI documentation"
LABEL org.opencontainers.image.source=""
LABEL org.opencontainers.image.version=""


RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '' uxp

RUN mkdir -p /app/jlib/addon/proxy/ciplus-jce/
RUN mkdir -p /deb/

COPY --from=builder /tmp/conf/usr/share/uxp/jlib/configuration-client.jar                     /app
COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/addon/proxy/ciplus-jce/                    /app/jlib/addon/proxy/ciplus-jce/
COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar /app/jlib/addon/proxy/
COPY confclient-logback.xml /app/
COPY deb/libgomp1_12.3.0-1ubuntu1~22.04_amd64.deb /deb/

RUN apt install /deb/libgomp1_12.3.0-1ubuntu1~22.04_amd64.deb && rm -rf /deb

ENTRYPOINT ["top"]
#RUN rm /bin/bash && rm /bin/sh
TODO add /usr/lib/x86_64-linux-gnu/libtcmalloc.so.4
#USER uxp
#ENTRYPOINT ["java", \
#            "-Xmx50m", \
#            "-XX:MaxMetaspaceSize=70m", \
#            "-XX:+UseG1GC", \
#            "-Xshare:on", \
#            "-Dfile.encoding=UTF-8", \
#            "-Djava.library.path=/app/lib", \
#            "-Dlogback.configurationFile=/app/confclient-logback.xml", \
#            "-Dorg.bytedeco.javacpp.noPointerGC=true", \
#            "-Dorg.bytedeco.javacpp.maxBytes=0", \
#            "-Duxp.configuration-client.port=5564", \
#            "-Duxp.common.temp-files-path=/etc/uxp/tmp/", \
#            "-Dorg.bytedeco.javacpp.maxPhysicalBytes=0", \
#            "-cp", "/app/configuration-client.jar:/app/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/app/jlib/addon/proxy/ciplus-jce/*", \
#            "ee.cyber.uxp.common.conf.globalconf.ConfigurationClientMain"]