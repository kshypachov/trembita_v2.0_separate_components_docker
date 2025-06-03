FROM ubuntu:24.04 as builder
ENV DEBIAN_FRONTEND=noninteractive

RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '' uxp

RUN mkdir "/deb"
COPY ./deb/ /deb/

RUN dpkg-deb -X /deb/uxp-securityserver-rest-api_1.22.7_all.deb    /tmp/seg-api
RUN dpkg-deb -X /deb/uxp-addon-trembita-crypto_1.22.7_all.deb      /tmp/crypto
RUN dpkg-deb -X /deb/uxp-addon-trembita-profile_1.22.7_all.deb     /tmp/profile

FROM eclipse-temurin:17-jdk-jammy

LABEL authors="Kirill Shypachov (@kshypachov), on behalf of eGA Kyiv"
LABEL org.opencontainers.image.authors="Kirill Shypachov <kiril.shypachov@ega.ee> (eGA Kyiv)"
LABEL org.opencontainers.image.vendor="eGA Kyiv"
LABEL org.opencontainers.image.title="trembita SEG REST API service"
LABEL org.opencontainers.image.url="URI documentation"
LABEL org.opencontainers.image.source=""
LABEL org.opencontainers.image.version=""

RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '' uxp

RUN mkdir -p /app/jlib/addon/proxy/ciplus-jce/
RUN mkdir -p /deb/


COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/signature-xades.jar                        /app/jlib/
COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/addon/proxy/ciplus-jce/                    /app/jlib/addon/proxy/ciplus-jce/
COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar /app/jlib/addon/proxy/
COPY --from=builder /tmp/seg-api/usr/share/uxp/jlib/securityserver*                           /app/
COPY --from=builder /tmp/profile/usr/share/uxp/jlib/addon/certprofile-trembita.jar            /app/jlib/addon/

COPY securityserver-rest-api-logback.xml                                                      /app/
COPY libpasswordstore.so                                                                      /usr/share/uxp/lib/

COPY deb/libgomp1_12.3.0-1ubuntu1~22.04_amd64.deb                                             /deb/
RUN apt install /deb/libgomp1_12.3.0-1ubuntu1~22.04_amd64.deb && rm -rf /deb

COPY deb_secure_dev/*                                                                         /deb/
RUN apt install -y --allow-downgrades /deb/*.deb && rm -rf /deb

#Add driver for Шифр-HSM
COPY lib_secure_dev/libcihsm.so                                                               /usr/share/uxp/lib/
RUN chown root:root /usr/share/uxp/lib/libcihsm.so && chmod 644 /usr/share/uxp/lib/libcihsm.so
# Add env PKCS11_PROXY_SOCKET="tcp://<proxy-address>:<proxy-port>" with HSM address and port

#Add driver for ІІТ Гряда-301
COPY lib_secure_dev/NCMGryada301PKCS11Libs-Linux/64/*.so                                      /usr/share/uxp/lib/
RUN chown root:root /usr/share/uxp/lib/*.so && chmod 644 /usr/share/uxp/lib/*.so
# Additionaly need to create /usr/share/uxp/lib/osplm.ini with info about HSM

#only for test need valid db.properties, in prod, this file mounted as secret or config map or another way
#COPY test/db.properties                                                                       /etc/uxp/db.properties
#need only for test. in production it s shared volume between SEG and REST API pod
#RUN mkdir -p /etc/uxp/signer/0/


USER uxp

#ENTRYPOINT ["java", \
#            "-Xmx128m", \
#            "-Xshare:on", \
#            "-XX:MaxMetaspaceSize=256m", \
#            "-XX:+UseG1GC", \
#            "-Dlogging.config=/app/securityserver-rest-api-logback.xml",\
#            "-Dfile.encoding=UTF-8", \
#            "-Dorg.bytedeco.javacpp.noPointerGC=true", \
#            "-Dorg.bytedeco.javacpp.maxBytes=0", \
#            "-Dorg.bytedeco.javacpp.maxPhysicalBytes=0", \
#            "-Djava.library.path=/usr/share/uxp/lib/", \
#            "-Dloader.path=/app/jlib/signature-xades.jar,/app/jlib/addon/certprofile-trembita.jar,/app/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar,/app/jlib/addon/proxy/ciplus-jce/*", \
#            "-cp", "/app/securityserver-rest-api.jar", \
#            "-Duxp.common.configuration-anchor-file=/etc/uxp/configuration-anchor.xml", \
#            "-Duxp.common.global-conf-path=/etc/uxp/globalconf/", \
#            "-Duxp.proxy.software-token-key-dir=/etc/uxp/signer/", \
#            "-Duxp.proxy.server-port=5500", \
#            "-Dserver.port=8085", \
#            "-Duxp.identity-provider.security-server-client-id=pvoqbggvvzpon1r4v55b7z8cu0de18cj", \
#            "-Duxp.identity-provider.security-server-client-secret=2DmVrz_VUQUhn3ePNgWm8Ur-TwMK0la_", \
#            "-Duxp.proxy.database-properties=/etc/uxp/db.properties", \
#            "-Duxp.identity-provider.database-properties=/etc/uxp/db.properties", \
#            "org.springframework.boot.loader.PropertiesLauncher"]