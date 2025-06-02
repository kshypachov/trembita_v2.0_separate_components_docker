FROM ubuntu:24.04 as builder
ENV DEBIAN_FRONTEND=noninteractive

RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '' uxp

RUN mkdir "/deb"
COPY ./deb/ /deb/

RUN dpkg-deb -X /deb/uxp-addon-trembita-crypto_1.22.7_all.deb  /tmp/crypto
RUN dpkg-deb -X /deb/uxp-verifier_1.22.7_all.deb               /tmp/verifier
RUN dpkg-deb -X /deb/uxp-addon-trembita-profile_1.22.7_all.deb /tmp/profile

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


COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/signature-xades.jar                        /app/jlib/
COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/addon/proxy/ciplus-jce/                    /app/jlib/addon/proxy/ciplus-jce/
COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar /app/jlib/addon/proxy/
COPY --from=builder /tmp/verifier/usr/share/uxp/jlib/verifier*                                /app/
COPY --from=builder /tmp/profile/usr/share/uxp/jlib/addon/certprofile-trembita.jar            /app/jlib/addon/

COPY deb/libgomp1_12.3.0-1ubuntu1~22.04_amd64.deb                                             /deb/
RUN apt install /deb/libgomp1_12.3.0-1ubuntu1~22.04_amd64.deb

COPY verifier-rest-api-logback.xml                                                            /app/

#COPY test/db.properties                                                                       /etc/uxp/db.properties

RUN mkdir -p /var/lib/uxp/messagelog/archive

USER uxp
#ENTRYPOINT ["top", "-b"]
#ENTRYPOINT ["java", \
#            "-Xmx128m", \
#            "-XX:MaxMetaspaceSize=128m", \
#            "-XX:+UseG1GC", \
#            "-Xshare:on", \
#            "-Dorg.bytedeco.javacpp.noPointerGC=true", \
#            "-Dorg.bytedeco.javacpp.maxBytes=0", \
#            "-Dorg.bytedeco.javacpp.maxPhysicalBytes=0", \
#            "-Dlogging.config=/app/verifier-rest-api-logback.xml", \
#            "-Djava.library.path=/usr/share/uxp/lib/", \
#            "-Dfile.encoding=UTF-8", \
#            "-cp", "/app/verifier-rest-api.jar", \
#            "-Duxp.center.allowed-certificate-profiles=ee.cyber.uxp.common.certificateprofile.ua.UaCertificateProfileInfoProvider", \
#            "-Dloader.path=/app/jlib/signature-xades.jar,/app/jlib/addon/certprofile-trembita.jar,/app/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar,/app/jlib/addon/proxy/ciplus-jce/*", \
#            "-Dserver.port=8086", \
#            "-Duxp.message-log.storage-path=/tmp", \
#            "-Duxp.identity-provider.security-server-client-id=pvoqbggvvzpon1r4v55b7z8cu0de18cj", \
#            "-Duxp.identity-provider.security-server-client-secret=2DmVrz_VUQUhn3ePNgWm8Ur-TwMK0la_", \
#            "-Duxp.proxy.database-properties=/etc/uxp/db.properties", \
#
#
#
##            "-Duxp.proxy.connector-host=0.0.0.0", \
##            "-Duxp.proxy.client-http-port=80", \
##            "-Duxp.proxy.client-https-port=443", \
##            "-Duxp.proxy.client-httpclient-idle-connection-eviction-period=1", \
##            "-Duxp.proxy.client-httpclient-connect-timeout=30000", \
##            "-Duxp.proxy.client-httpclient-read-timeout=300000", \
##            "-Duxp.proxy.client-httpclient-target-selection-strategy=round-robin", \
##            "-Duxp.proxy.client-httpclient-socket-buffer-size=16384", \
##            "-Duxp.proxy.client-connector-socket-buffer-size=16384", \
##            "-Duxp.proxy.client-jetty-thread-pool-max-size=45", \
##            "-Duxp.proxy.server-httpclient-socket-buffer-size=16384", \
##            "-Duxp.proxy.server-connector-socket-buffer-size=16384", \
##            "-Duxp.proxy.server-jetty-thread-pool-max-size=45", \
##            "-Duxp.proxy.server-port=5500", \
##            "-Duxp.proxy.server-listen-address=0.0.0.0", \
##            "-Duxp.proxy.server-listen-port=5500", \
##            "-Duxp.proxy.server-connection-accept-rate-limit=0", \
##            "-Duxp.proxy.server-connection-accept-rate-limit-period=1", \
##            "-Duxp.proxy.server-httpclient-idle-connection-eviction-period=1", \
##            "-Duxp.proxy.server-httpclient-connect-timeout=30000", \
##            "-Duxp.proxy.server-httpclient-read-timeout=10000", \
##            "-Duxp.proxy.server-stapling-ocsp-cache-lifetime=300", \
##            "-Duxp.proxy.round-robin-quarantine-time=60000", \
##            "-Duxp.proxy.ocsp-cache-path=/var/cache/uxp", \
##            "-Duxp.proxy.ocsp-responder-client-connect-timeout=20000", \
##            "-Duxp.proxy.ocsp-responder-client-read-timeout=30000", \
##            "-Duxp.proxy.ocsp-usage-safety-offset=2", \
##            "-Duxp.proxy.wsdl-download-connect-timeout=10000", \
##            "-Duxp.proxy.wsdl-download-read-timeout=5000", \
##            "-Duxp.proxy.openapi-download-connect-timeout=10000", \
##            "-Duxp.proxy.openapi-download-read-timeout=5000", \
###            "-Duxp.proxy.database-properties=/etc/uxp/db.properties", \
##            "-Duxp.proxy.software-token-batch-signatures=false", \
##            "-Duxp.proxy.batch-signatures-enabled=true", \
##            "-Duxp.proxy.log-signatures=true", \
##            "-Duxp.proxy.log-metaservice-signatures=true", \
##            "-Duxp.proxy.log-monitoring-signatures=true", \
###            "-Duxp.proxy.max-retained-soap-message-size-bytes=5242880", \
##            "-Duxp.proxy.max-retained-soap-attachment-size-bytes=0", \
##            "-Duxp.proxy.max-retained-rest-payload-size-bytes=5242880", \
##            "-Duxp.proxy.additional-forbidden-rest-http-headers=", \
###            "-Duxp.proxy.serverconf-reload-interval-seconds=60", \
##            "-Duxp.proxy.digest-algorithm-id=SHA-512", \
##            "-Duxp.proxy.cert-reg-signature-digest-algorithm-id=SHA-512", \
##            "-Duxp.proxy.csr-signature-digest-algorithm-id=SHA-256", \
##            "-Duxp.proxy.internal-cipher-suites=TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256,TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384,TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384,TLS_DHE_RSA_WITH_AES_128_GCM_SHA256,TLS_DHE_RSA_WITH_AES_128_CBC_SHA256,TLS_DHE_RSA_WITH_AES_256_CBC_SHA256,TLS_DHE_RSA_WITH_AES_256_GCM_SHA384,TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384,TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,TLS_AES_128_GCM_SHA256,TLS_AES_256_GCM_SHA384", \
##            "-Duxp.proxy.transport-cipher-suites=TLS_DHE_RSA_WITH_AES_256_CBC_SHA256,TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384,TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,TLS_AES_256_GCM_SHA384", \
##            "-Duxp.proxy.verify-signing-certificate-qualified=false", \
##            "-Duxp.proxy.signature-timestamp-required=false", \
##            "-Duxp.proxy.timestamp-verify-signer-chain=false", \
###            "-Duxp.proxy.timestamper-httpclient-idle-connection-eviction-period=1", \
##            "-Duxp.proxy.timestamper-httpclient-connect-timeout=30000", \
##            "-Duxp.proxy.timestamper-httpclient-read-timeout=300000", \
##            "-Duxp.proxy.software-token-key-dir=/etc/uxp/signer/", \
##            "-Duxp.pkcs11.signing-session-pool-wait-time-seconds=10", \
##            "-Duxp.anti-dos.enabled=true", \
##            "-Duxp.anti-dos.max-parallel-connections=5000", \
##            "-Duxp.anti-dos.max-cpu-load=1.1", \
##            "-Duxp.anti-dos.max-heap-usage=1.1", \
##            "-Duxp.anti-dos.min-free-file-handles=100", \
##            "-Duxp.op-monitor-buffer.size=20000", \
##            "-Duxp.op-monitor-buffer.max-records-in-message=500", \
##            "-Duxp.op-monitor-buffer.sending-interval-seconds=5", \
##            "-Duxp.op-monitor-buffer.httpclient-connect-timeout=30000", \
##            "-Duxp.op-monitor-buffer.httpclient-read-timeout=60000", \
##            "-Duxp.monitoring-service.httpclient-connect-timeout=30000", \
##            "-Duxp.monitoring-service.httpclient-read-timeout=60000", \
##            "-Duxp.proxy-status-check.clientproxy-listening-switch-enabled=false", \
##            "-Duxp.proxy-status-check.serverproxy-listening-switch-enabled=false", \
##            "-Duxp.proxy-status-check.interval-seconds=15", \
#
#            "org.springframework.boot.loader.PropertiesLauncher"]