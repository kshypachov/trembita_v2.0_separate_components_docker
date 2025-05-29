FROM ubuntu:22.04 AS bilder
ENV DEBIAN_FRONTEND=noninteractive

LABEL authors="Kirill Shypachov @kshypachov"
COPY uxp-securityserver-ui_1.4.6_all.deb /uxp-securityserver-ui_1.4.6_all.deb
RUN mkdir /tmp/ui
RUN dpkg-deb -X /uxp-securityserver-ui_1.4.6_all.deb /tmp/ui
RUN chmod 755 -R /tmp/ui

COPY uxp-identity-provider-ui_1.4.6_all.deb /uxp-identity-provider-ui_1.4.6_all.deb
RUN mkdir /tmp/identity-ui
RUN dpkg-deb -X /uxp-identity-provider-ui_1.4.6_all.deb /tmp/identity-ui
RUN chmod 755 -R /tmp/identity-ui

COPY uxp-securityserver-locale-uk_1.4.6_all.deb /uxp-securityserver-locale-uk_1.4.6_all.deb
RUN mkdir /tmp/uk
RUN dpkg-deb -X /uxp-securityserver-locale-uk_1.4.6_all.deb /tmp/uk
RUN chmod 755 -R /tmp/uk

COPY uxp-securityserver-locale-en_1.4.6_all.deb /uxp-securityserver-locale-en_1.4.6_all.deb
RUN mkdir /tmp/en
RUN dpkg-deb -X /uxp-securityserver-locale-en_1.4.6_all.deb /tmp/en
RUN chmod 755 -R /tmp/en

#ENTRYPOINT ["top", "-b"]

FROM ubuntu:22.04
RUN apt update &&  \
    apt install -qq --no-install-recommends -y nginx && \
    rm -rf /var/lib/apt/lists/* &&  \
    apt clean && rm /etc/nginx/sites-enabled/default

COPY nginx.conf /etc/nginx/nginx.conf

RUN rm -fr /var/log/nginx && \
    mkdir -p /var/log/nginx && \
    ln -sf /dev/null /var/log/nginx/error.log

COPY default-uxp.conf /etc/nginx/sites-enabled/default-uxp
COPY conf.d/* /etc/nginx/conf.d/

RUN mkdir -p /usr/share/uxp/identity-provider-ui
RUN mkdir -p /usr/share/uxp/securityserver-uihtml/locales/uk/
RUN mkdir -p /usr/share/uxp/securityserver-uihtml/locales/en/
COPY --from=bilder /tmp/ui/usr/share/uxp/securityserver-ui/                 /usr/share/uxp/securityserver-ui/
COPY --from=bilder /tmp/identity-ui/usr/share/uxp/identity-provider-ui/     /usr/share/uxp/identity-provider-ui/
COPY --from=bilder /tmp/en/usr/share/uxp/securityserver-ui/html/locales/en/              /usr/share/uxp/securityserver-ui/html/locales/en/
COPY --from=bilder /tmp/uk/usr/share/uxp/securityserver-ui/html/locales/uk/               /usr/share/uxp/securityserver-ui/html/locales/uk/

RUN rm /bin/sh && rm /bin/bash

ENTRYPOINT ["nginx", "-g", "daemon off;"]