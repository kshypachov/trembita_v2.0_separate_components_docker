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
LABEL org.opencontainers.image.title="trembita message log archiver"
LABEL org.opencontainers.image.url="URI documentation"
LABEL org.opencontainers.image.source=""
LABEL org.opencontainers.image.version=""

RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '' uxp

RUN mkdir -p /app/jlib/addon/proxy/ciplus-jce/
RUN mkdir -p /deb/

COPY --from=builder /tmp/proxy/usr/share/uxp/jlib/messagelog*                                   /app/

COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/signature-xades.jar                          /app/jlib/
COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/addon/proxy/ciplus-jce/                      /app/jlib/addon/proxy/ciplus-jce/
COPY --from=builder /tmp/crypto/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar   /app/jlib/addon/proxy/

COPY messagelog-archiver-logback.xml                                    /app/

COPY deb/libgoogle-perftools4_2.9.1-0ubuntu3_amd64.deb                  /deb/
COPY deb/libtcmalloc-minimal4_2.9.1-0ubuntu3_amd64.deb                  /deb/
COPY deb/libunwind8_1.3.2-2build2.1_amd64.deb                           /deb/
COPY deb/libgomp1_12.3.0-1ubuntu1~22.04_amd64.deb                       /deb/

RUN apt install /deb/libunwind8_1.3.2-2build2.1_amd64.deb
RUN apt install /deb/libtcmalloc-minimal4_2.9.1-0ubuntu3_amd64.deb
RUN apt install /deb/libgomp1_12.3.0-1ubuntu1~22.04_amd64.deb
RUN apt install /deb/libgoogle-perftools4_2.9.1-0ubuntu3_amd64.deb && rm -rf /deb

ENV LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libtcmalloc.so.4

#COPY test/db.properties                                                 /etc/uxp/
USER uxp
#ENTRYPOINT ["java", \
#            "-Xmx256m", \
#            "-Xshare:on", \
#            "-XX:MaxMetaspaceSize=128m", \
#            "-XX:+UseG1GC", \
#            "-Dfile.encoding=UTF-8", \
#            "-Djava.library.path=/usr/share/uxp/lib/", \
#            "-Dorg.bytedeco.javacpp.noPointerGC=true", \
#            "-Dorg.bytedeco.javacpp.maxBytes=0", \
#            "-Dorg.bytedeco.javacpp.maxPhysicalBytes=0", \
#            "-Dlogback.configurationFile=/app/messagelog-archiver-logback.xml", \
#            "-cp", "/app/messagelog-archiver.jar:/app/jlib/signature-xades.jar:/app/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/app/jlib/addon/proxy/ciplus-jce/*", \
#            "-Duxp.message-log.archiver-admin-port=5765", \
#            "-Duxp.message-log.metadata-persistence-unit=messagelog-metadata", \
#            "-Duxp.message-log.metadata-record-lifetime-minutes=1576800", \
#            "-Duxp.message-log.metadata-record-cleanup-operation=DELETE", \
#            "-Duxp.message-log.metadata-cleaner-batch-size=1000", \
#            "-Duxp.message-log.metadata-cleanup-interval=0 0 0 * * ? *", \
#            "-Duxp.message-log.hash-algo-id=SHA-512", \
#            "-Duxp.message-log.timestamper-client-connect-timeout=10000", \
#            "-Duxp.message-log.timestamper-batch-size=10000", \
#            "-Duxp.message-log.timestamper-client-read-timeout=5000", \
#            "-Duxp.message-log.acceptable-timestamp-failure-period=14400", \
#            "-Duxp.message-log.timestamp-certificate-chain-request=true", \
#            "-Duxp.message-log.timestamp-provider-round-robin=true", \
#            "-Duxp.message-log.timestamp-immediately=true", \
#            "-Duxp.message-log.metadata-record-extract-path=/var/lib/uxp/messagelog/old-metadata", \
#            "-Duxp.message-log.storage-path=/var/lib/uxp/messagelog", \
#            "-Duxp.message-log.archive-path=/var/lib/uxp/messagelog/archive", \
#            "-Duxp.message-log.temp-files-path=/var/lib/uxp/messagelog/tmp", \
#            "-Duxp.message-log.archive-interval=0 0 0/1 1/1 * ? *", \
#            "-Duxp.message-log.archiver-batch-size=100", \
#            "-Duxp.message-log.archive-transfer-command=''", \
#            "-Duxp.message-log.archive-storage-type=fs", \
#            "-Duxp.message-log-s3.trusted-certificate=''", \
#            "-Duxp.message-log-s3.region=us-west-1", \
#            "-Duxp.message-log-s3.bucket-name=''", \
#            "-Duxp.message-log-s3.storage-class=''", \
#            "-Duxp.message-log-s3.access-key=''", \
#            "-Duxp.message-log-s3.secret-key=''", \
#            "-Duxp.message-log-s3.request-timeout=0", \
#            "-Duxp.message-log-s3.address=''", \
#            "ee.cyber.uxp.messagelog.archiving.MessageLogArchiverMain"]