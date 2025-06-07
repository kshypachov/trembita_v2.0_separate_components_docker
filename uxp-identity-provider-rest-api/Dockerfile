FROM ubuntu:24.04 as builder
ENV DEBIAN_FRONTEND=noninteractive

RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '' uxp

RUN mkdir "/deb"
COPY ./deb/ /deb/

RUN dpkg-deb -X /deb/uxp-identity-provider-rest-api_1.22.7_all.deb                  /tmp/identity


FROM eclipse-temurin:17-jdk-jammy

LABEL authors="Kirill Shypachov (@kshypachov), on behalf of eGA Kyiv"
LABEL org.opencontainers.image.authors="Kirill Shypachov <kiril.shypachov@ega.ee> (eGA Kyiv)"
LABEL org.opencontainers.image.vendor="eGA Kyiv"
LABEL org.opencontainers.image.title="trembita identity provider"
LABEL org.opencontainers.image.url="URI documentation"
LABEL org.opencontainers.image.source=""
LABEL org.opencontainers.image.version=""

RUN groupadd -g 104 uxp && \
    useradd -m -u 102 -g 104 -d /var/lib/uxp -s /usr/sbin/nologin -p '' uxp

RUN mkdir -p /deb/
RUN mkdir -p /app/jlib/

COPY identity-provider-rest-api-logback.xml                                                          /app/
COPY --from=builder /tmp/identity/usr/share/uxp/jlib/identity*                                       /app/jlib/

RUN rm /bin/bash && rm /bin/sh

USER uxp
#ENTRYPOINT ["java", \
#            "-Xmx128m", \
#            "-Xshare:on", \
#            "-XX:MaxMetaspaceSize=256m", \
#            "-XX:+UseG1GC", \
#            "-cp", "/app/jlib/identity-provider-rest-api.jar", \
#            "-Dfile.encoding=UTF-8", \
#            "-Djava.library.path=/usr/share/uxp/lib/", \
#            "-Dserver.port=8087", \
#            "-Dlogging.config=/app/identity-provider-rest-api-logback.xml", \
#            "-Duxp.identity-provider.security-server-client-id=pvoqbggvvzpon1r4v55b7z8cu0de18cj", \
#            "-Duxp.identity-provider.security-server-client-secret=2DmVrz_VUQUhn3ePNgWm8Ur-TwMK0la_", \
#            "-Duxp.identity-provider.database-properties=/etc/uxp/db.properties", \
#            "-Duxp.identity-provider.oauth2-issuer-location=", \
#            "-Duxp.identity-provider.bcrypt.log-rounds=10", \
#            "-Duxp.identity-provider.login-max-failed-attempts=5", \
#            "-Duxp.identity-provider.public-client-redirect-uris=https://trembita.office", \
#            "-Duxp.identity-provider.hostname=trembita.office", \
#            "-Duxp.identity-provider.public-client-id=uxp-ss-ui", \
#            "-Duxp.identity-provider.oauth2-introspect-uri=http://localhost:8087/auth-api/v1/oauth2/introspect", \
#            "-Duxp.identity-provider.login-lockout-time-duration=15", \
#            "-Duxp.identity-provider.public-client-access-token-time-to-live=180", \
#            "org.springframework.boot.loader.PropertiesLauncher"]