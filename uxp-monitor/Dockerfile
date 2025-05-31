FROM ubuntu:24.04 as builder
ENV DEBIAN_FRONTEND=noninteractive

RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '' uxp

RUN mkdir "/deb"
COPY ./deb/ /deb/

RUN dpkg-deb -X /deb/uxp-addon-monitor_1.22.7_all.deb      /tmp/mon
RUN dpkg-deb -X /deb/uxp-addon-trembita-crypto_1.22.7_all.deb  /tmp/crypto

#ENTRYPOINT ["top", "-b"]

FROM eclipse-temurin:17-jdk-jammy

LABEL authors="Kirill Shypachov (@kshypachov), on behalf of eGA Kyiv"
LABEL org.opencontainers.image.authors="Kirill Shypachov <kiril.shypachov@ega.ee> (eGA Kyiv)"
LABEL org.opencontainers.image.vendor="eGA Kyiv"
LABEL org.opencontainers.image.title="trembita monitor agent"
LABEL org.opencontainers.image.url="URI documentation"
LABEL org.opencontainers.image.source=""
LABEL org.opencontainers.image.version=""

RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '' uxp

RUN mkdir -p /app/jlib/addon/proxy/ciplus-jce/
RUN mkdir -p /deb/

COPY monitor-agent.ini /app/

COPY --from=builder /tmp/mon/usr/share/uxp/jlib/                                              /app/jlib/
COPY --from=builder /tmp/mon/usr/share/uxp/jlib/addon/proxy/monitoring-1.22.7.jar             /app/jlib/addon/proxy/
COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/signature-xades.jar                        /app/jlib/

COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/addon/proxy/ciplus-jce/                    /app/jlib/addon/proxy/ciplus-jce/
COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar /app/jlib/addon/proxy/


COPY proxy-monitor-agent-logback.xml                                    /app/
COPY deb/libgoogle-perftools4_2.9.1-0ubuntu3_amd64.deb                  /deb/
COPY deb/libtcmalloc-minimal4_2.9.1-0ubuntu3_amd64.deb                  /deb/
COPY deb/libunwind8_1.3.2-2build2.1_amd64.deb                           /deb/
COPY deb/libgomp1_12.3.0-1ubuntu1~22.04_amd64.deb                       /deb/

RUN apt install /deb/libunwind8_1.3.2-2build2.1_amd64.deb
RUN apt install /deb/libtcmalloc-minimal4_2.9.1-0ubuntu3_amd64.deb
RUN apt install /deb/libgomp1_12.3.0-1ubuntu1~22.04_amd64.deb
RUN apt install /deb/libgoogle-perftools4_2.9.1-0ubuntu3_amd64.deb && rm -rf /deb

ENV LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libtcmalloc.so.4


#RUN rm -rf /bin/bash && rm -rf /bin/sh

USER uxp
#ENTRYPOINT ["top", "-b"]

ENTRYPOINT ["java", \
            "-Xmx50m", \
            "-Xmx256m", \
            "-XX:MaxMetaspaceSize=128m", \
            "-XX:+UseG1GC", \
            "-Xshare:on", \
            "-Dfile.encoding=UTF-8", \
            "-Djava.library.path=/usr/share/uxp/lib/", \
            "-Duxp.common.temp-files-path=/tmp/java/", \
            "-Duxp.center.allowed-certificate-profiles=ee.cyber.uxp.common.certificateprofile.ua.UaCertificateProfileInfoProvider", \
            "-Duxp.common.license-file=/etc/uxp/license.lic" ,\
            "-Dlogback.configurationFile=/app/proxy-monitor-agent-logback.xml", \
            "-cp", "/app/jlib/monitoring-proxy-agent.jar:/app/jlib/signature-xades.jar:/app/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/app/jlib/addon/proxy/ciplus-jce/*", \
            "-Dorg.bytedeco.javacpp.noPointerGC=true", \
            "-Dorg.bytedeco.javacpp.maxBytes=0", \
            "-Dorg.bytedeco.javacpp.maxPhysicalBytes=0", \
            "-Duxp.proxy-monitoring-agent.monitor-agent-conf-file=/app/monitor-agent.ini", \
            "-Duser.country=UA", \
            #admin port for connecting to monitor agent port = port + 1.
            "-Duxp.proxy-monitoring-agent.admin-port=5588", \
            # port for recieve statistics from uxp - proxy port = port + 1
             "-Duxp.status-service.listen-port=2082", \
             #Readed from /etc/uxp/conf.d/local.ini must be the same in all component configs
#            "-Duxp.identity-provider.security-server-client-secret=2DmVrz_VUQUhn3ePNgWm8Ur-TwMK0la_", \
            "ee.cyber.uxp.proxymonitoragent.ProxyMonitorAgentMain"]
