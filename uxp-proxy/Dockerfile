FROM ubuntu:24.04 as token_login_builder
ENV DEBIAN_FRONTEND=noninteractive


RUN apt update &&  \
    apt install -y cmake gcc

RUN mkdir /token_login
COPY token_login /token_login
RUN cd /token_login/build && cmake .. && make

RUN chmod 111 /token_login/build/token_login # set rights only for execute


FROM ubuntu:24.04 as builder
ENV DEBIAN_FRONTEND=noninteractive

RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '' uxp

RUN mkdir "/deb"
COPY ./deb/ /deb/

RUN dpkg-deb -X /deb/uxp-proxy_1.22.7_all.deb                  /tmp/proxy
RUN dpkg-deb -X /deb/uxp-addon-trembita-crypto_1.22.7_all.deb  /tmp/crypto
RUN dpkg-deb -X /deb/uxp-addon-trembita-profile_1.22.7_all.deb /tmp/profile
RUN dpkg-deb -X /deb/uxp-addon-monitor_1.22.7_all.deb          /tmp/monitor
RUN dpkg-deb -X /deb/uxp-addon-metaservices_1.22.7_all.deb     /tmp/meta

FROM eclipse-temurin:17-jdk-jammy

LABEL authors="Kirill Shypachov (@kshypachov), on behalf of eGA Kyiv"
LABEL org.opencontainers.image.authors="Kirill Shypachov <kiril.shypachov@ega.ee> (eGA Kyiv)"
LABEL org.opencontainers.image.vendor="eGA Kyiv"
LABEL org.opencontainers.image.title="trembita proxy"
LABEL org.opencontainers.image.url="URI documentation"
LABEL org.opencontainers.image.source=""
LABEL org.opencontainers.image.version=""

RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '' uxp

RUN mkdir -p /app/jlib/addon/proxy/ciplus-jce/
RUN mkdir -p /deb/

COPY --from=builder /tmp/proxy/usr/share/uxp/jlib/proxy*                                      /app/

COPY --from=builder /tmp/monitor/usr/share/uxp/jlib/addon/proxy/monitoring-1.22.7.jar         /app/jlib/addon/proxy/
COPY --from=builder /tmp/meta/usr/share/uxp/jlib/addon/proxy/metaservice-1.22.7.jar           /app/jlib/addon/proxy/

COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/signature-xades.jar                        /app/jlib/
COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/addon/proxy/ciplus-jce/                    /app/jlib/addon/proxy/ciplus-jce/
COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar /app/jlib/addon/proxy/

COPY --from=builder /tmp/profile/usr/share/uxp/jlib/addon/certprofile-trembita.jar            /app/jlib/addon/
COPY --from=token_login_builder /token_login/build/token_login                                /app/

COPY libpasswordstore.so                                                                      /usr/share/uxp/lib/libpasswordstore.so
COPY proxy-logback.xml                                                                        /app/
COPY configs/proxy.ini                                                                        /etc/uxp/conf.d/

COPY deb/authbind_2.1.3build1_amd64.deb                                                       /deb/
RUN apt install -y /deb/authbind_2.1.3build1_amd64.deb

RUN touch /etc/authbind/byport/80 && chmod 102 /etc/authbind/byport/80 && chown 102 /etc/authbind/byport/80
RUN touch /etc/authbind/byport/443 && chmod 102 /etc/authbind/byport/443 && chown 102 /etc/authbind/byport/443

USER uxp
##RUN mkdir -p /etc/uxp/signer/0/
##COPY configs/db.properties      /etc/uxp/
#ENTRYPOINT ["authbind", "java", \
#            "-Xms150m", \
#            "-Xmx512m", \
#            "-Xshare:on", \
#            "-XX:MaxMetaspaceSize=256m", \
#            "-XX:+UseG1GC", \
#            "-Dfile.encoding=UTF-8", \
#            "-Dorg.bytedeco.javacpp.noPointerGC=true", \
#            "-Dorg.bytedeco.javacpp.maxBytes=0", \
#            "-Dorg.bytedeco.javacpp.maxPhysicalBytes=0", \
#            "-Djava.library.path=/usr/share/uxp/lib/", \
#            "-Dlogback.configurationFile=/app/proxy-logback.xml", \
#            "-Duxp.proxy.clientHandlers=ee.cyber.uxp.proxy.clientproxy.MetadataHandler", \
#            "-Duxp.proxy.serverServiceHandlers=ee.cyber.uxp.proxy.serverproxy.MetadataServiceSOAPHandlerImpl,ee.cyber.uxp.proxy.serverproxy.MonitoringServiceHandlerImpl", \
#            "-Duxp.proxy.serverRestApiHandlers=ee.cyber.uxp.proxy.serverproxy.MetadataServiceRESTHandlerImpl", \
#            "-cp", "/app/proxy.jar:/app/jlib/signature-xades.jar:/app/jlib/addon/certprofile-trembita.jar:/app/jlib/addon/proxy/metaservice-1.22.7.jar:/app/jlib/addon/proxy/monitoring-1.22.7.jar:/app/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/app/jlib/addon/proxy/ciplus-jce/*", \
#            "ee.cyber.uxp.proxy.ProxyMain"]